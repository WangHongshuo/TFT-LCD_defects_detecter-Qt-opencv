#include "detecter.h"
#include <QDebug>
#include <QTime>
#include <opencv2/highgui.hpp>
#include <vector>

Detecter::Detecter()
{
    spatialDomainImg = new Mat;
    freqDomainImg = new Mat;
    lowPowerImg = new Mat;
    binaryImg = new Mat;
    invBinaryImg = new Mat;
}

Detecter::~Detecter()
{
    delete spatialDomainImg;
    delete freqDomainImg;
    delete lowPowerImg;
    delete binaryImg;
    delete invBinaryImg;
}

void Detecter::setParameters(const Mat& data, int filter_size, int R, int r1, int r2, int bw_t)
{
    *spatialDomainImg = data.clone();
    set_parameters_con(filter_size, R, r1, r2, bw_t);
    main_task();
}

void Detecter::setParameters(const Mat* data, int filter_size, int R, int r1, int r2, int bw_t)
{
    *spatialDomainImg = data->clone();
    set_parameters_con(filter_size, R, r1, r2, bw_t);
    main_task();
}

void Detecter::setParameters(int filter_size, int R, int r1, int r2, int bw_t)
{
    set_parameters_con(filter_size, R, r1, r2, bw_t);
    proc_frequency_domain_img();
}

const Mat* Detecter::getSpatialDomainImg()
{
    return spatialDomainImg;
}

const Mat* Detecter::getFreqDomainImg()
{
    return freqDomainImg;
}

const Mat* Detecter::getFreqDomainMask()
{
    return freqDomainMask;
}

const Mat* Detecter::getLowPowerImg()
{
    return lowPowerImg;
}

const Mat* Detecter::getBinaryImg()
{
    return binaryImg;
}

const Mat* Detecter::getInvBinaryImg()
{
    return invBinaryImg;
}

void Detecter::set_parameters_con(int filter_size, int R, int r1, int r2, int bw_t)
{
    imgWidth = spatialDomainImg->cols;
    imgHeight = spatialDomainImg->rows;
    meanFilterSize = filter_size;
    RValue = R;
    r1Value = r1;
    r2Value = r2;
    bw_t_value = bw_t; 
}

void Detecter::main_task()
{
    complexMat = create_complex_Mat(*spatialDomainImg);
    DFT_function(complexMat, amplitude, cosine, sine);
    *freqDomainImg = get_energyMap(amplitude);
    proc_frequency_domain_img();
}

void Detecter::proc_frequency_domain_img()
{
    Mat mask = get_sailencyMap(*freqDomainImg);
    mask = 1 - mask;
    multiply(*freqDomainImg, mask, *lowPowerImg);
    ishift(mask);
    mask.convertTo(mask, CV_32F);
    multiply(mask, amplitude, mask);
    IDFT_function(*binaryImg, mask, cosine, sine);
    binarizeThreshold = get_bw_value(*binaryImg, bw_t_value);
    threshold(*binaryImg, *invBinaryImg, binarizeThreshold, 255, THRESH_BINARY);
    bwareaopen(*invBinaryImg, 1);
    *invBinaryImg = (*invBinaryImg) < 100;
}

Mat Detecter::create_complex_Mat(Mat& data)
{
    Mat output;
    Mat r = data.clone();
    Mat i = Mat::zeros(r.size(), CV_32F);
    if (r.type() != CV_32F)
        r.convertTo(r, CV_32F);
    Mat temp[] = { r, i };
    merge(temp, 2, output);
    return output;
}

Mat Detecter::create_complex_Mat(Mat& R, Mat& I)
{
    Mat r = R.clone();
    Mat i = I.clone();
    if (r.type() != CV_32F)
        r.convertTo(r, CV_32F);
    if (i.type() != CV_32F)
        i.convertTo(i, CV_32F);
    Mat output;
    Mat temp[] = { r, i };
    merge(temp, 2, output);
    return output;
}

void Detecter::DFT_function(Mat& data, Mat& Am, Mat& Cosine, Mat& Sine)
{
    dft(data, data);
    Mat temp[2];
    split(data, temp);
    // 浅拷贝
    Mat R = temp[0];
    Mat I = temp[1];
    magnitude(R, I, Am);
    divide(R, Am, Cosine);
    divide(I, Am, Sine);
    Am += 1;
    log(Am, Am);
}

void Detecter::IDFT_function(Mat& output, Mat Am, Mat Cosine, Mat Sine)
{
    exp(Am, Am);
    Mat R, I;
    multiply(Am, Cosine, R);
    multiply(Am, Sine, I);
    Mat complex = create_complex_Mat(R, I);
    idft(complex, complex);
    Mat temp[2];
    split(complex, temp);
    R = temp[0];
    I = temp[1];
    magnitude(R, I, output);
    multiply(output, output, output);
    normalize(output, output, 0, 255, NORM_MINMAX);
    output.convertTo(output, CV_8UC1);
}

Mat Detecter::get_energyMap(Mat& Am)
{
    Mat output = Am.clone();
    shift(output);
    normalize(output, output, 0, 255, NORM_MINMAX);
    output.convertTo(output, CV_8UC1);
    return output;
}

void Detecter::shift(Mat& data)
{
    int cx = data.cols / 2;
    int cy = data.rows / 2;
    Mat temp;
    Mat q0(data, Rect(0, 0, cx, cy));
    Mat q1(data, Rect(cx, 0, cx, cy));
    Mat q2(data, Rect(0, cy, cx, cy));
    Mat q3(data, Rect(cx, cy, cx, cy));

    q0.copyTo(temp);
    q3.copyTo(q0);
    temp.copyTo(q3);

    q1.copyTo(temp);
    q2.copyTo(q1);
    temp.copyTo(q2);
}

void Detecter::ishift(Mat& data)
{
    int cx = data.cols / 2;
    int cy = data.rows / 2;
    Mat temp;
    Mat q0(data, Rect(0, 0, cx, cy));
    Mat q1(data, Rect(cx, 0, cx, cy));
    Mat q2(data, Rect(0, cy, cx, cy));
    Mat q3(data, Rect(cx, cy, cx, cy));

    q0.copyTo(temp);
    q3.copyTo(q0);
    temp.copyTo(q3);

    q1.copyTo(temp);
    q2.copyTo(q1);
    temp.copyTo(q2);
}

Mat Detecter::get_sailencyMap(Mat& data)
{
    Mat complex = create_complex_Mat(data);
    Mat A, C, S;
    DFT_function(complex, A, C, S);
    Mat A_avg;
    blur(A, A_avg, Size(meanFilterSize, meanFilterSize), Point(-1, -1), BORDER_REPLICATE);
    A = A - A_avg;
    IDFT_function(A, A, C, S);
    threshold(A, A, 0, 1, THRESH_OTSU);
    bwareaopen(A, 4);
    Mat kern = getStructuringElement(MORPH_CROSS, Size(3, 3));
    dilate(A, A, kern, Point(-1, -1), RValue);
    if (r2Value >= 0)
        circle(A, cv::Point((imgWidth / 2), (imgHeight / 2)), r2Value, 0, -1);
    if (r1Value >= 0)
        circle(A, cv::Point((imgWidth / 2), (imgHeight / 2)), r1Value, 1, -1);
    return A;
}

void Detecter::bwareaopen(Mat& data, int n)
{
    Mat labels, stats, centroids;
    connectedComponentsWithStats(data, labels, stats, centroids, 8, CV_32S);
    int regions_count = stats.rows - 1;
    int regions_size, regions_x1, regions_y1, regions_x2, regions_y2;

    for (int i = 1; i <= regions_count; i++) {
        regions_size = stats.ptr<int>(i)[4];
        if (regions_size < n) {
            regions_x1 = stats.ptr<int>(i)[0];
            regions_y1 = stats.ptr<int>(i)[1];
            regions_x2 = regions_x1 + stats.ptr<int>(i)[2];
            regions_y2 = regions_y1 + stats.ptr<int>(i)[3];

            for (int j = regions_y1; j < regions_y2; j++) {
                for (int k = regions_x1; k < regions_x2; k++) {
                    if (labels.ptr<ushort>(j)[k] == i)
                        data.ptr<uchar>(j)[k] = 0;
                }
            }
        }
    }
}

double Detecter::get_bw_value(const Mat& input, double t)
{
    int channels[] = { 0 };
    int histSize[] = { 256 };
    float my_ranges[] = { 0, 256 };
    const float* ranges[] = { my_ranges };
    cv::MatND hist;
    calcHist(&input, 1, channels, cv::Mat(), hist, 1, histSize, ranges);
    Mat mean_value, std_value;
    int i;
    for (i = 0; i < 256; i++) {
        meanStdDev(hist.rowRange(i, 255), mean_value, std_value);
        if (std_value.ptr<double>(0)[0] < t)
            break;
    }
    return double(i);
}
