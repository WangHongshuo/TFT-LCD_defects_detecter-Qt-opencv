#include "detecter.h"
#include <QDebug>
#include <QTime>
#include <opencv2/highgui.hpp>
#include <vector>

Detecter::Detecter()
{
}

Detecter::~Detecter()
{
}

Mat& Detecter::setParametersAndProc(const Mat& data, int filter_size, int R, int r1, int r2, int bw_t)
{
    spatialDomainImg = data.clone();
    setParameters(filter_size, R, r1, r2, bw_t);
    mainProc();
    return defectsInfo;
}

Mat& Detecter::setParametersAndProc(const Mat* data, int filter_size, int R, int r1, int r2, int bw_t)
{
    spatialDomainImg = data->clone();
    setParameters(filter_size, R, r1, r2, bw_t);
    mainProc();
    return defectsInfo;
}

Mat& Detecter::setParametersAndProc(int filter_size, int R, int r1, int r2, int bw_t)
{
    setParameters(filter_size, R, r1, r2, bw_t);
    procFrequencyDomainImg();
    return defectsInfo;
}

Mat& Detecter::getSpatialDomainImg()
{
    return spatialDomainImg;
}

Mat& Detecter::getFreqDomainImg()
{
    return freqDomainImg;
}

Mat& Detecter::getFreqDomainMask()
{
    return freqDomainMask;
}

Mat& Detecter::getLowPowerImg()
{
    return lowPowerImg;
}

Mat& Detecter::getBinaryImg()
{
    return binaryImg;
}

Mat& Detecter::getInvBinaryImg()
{
    return invBinaryImg;
}

void Detecter::setParameters(int filter_size, int R, int r1, int r2, int bw_t)
{
    meanFilterSize = filter_size;
    RValue = R;
    r1Value = r1;
    r2Value = r2;
    inputBinaryImgThreshold = bw_t;
}

void Detecter::mainProc()
{
    complexMat = createComplexMat(spatialDomainImg);
    dftFunction(complexMat, amplitude, cosine, sine);
    freqDomainImg = getEnergyMap(amplitude);
    procFrequencyDomainImg();
}

void Detecter::procFrequencyDomainImg()
{
    Mat mask = getSailencyMap(freqDomainImg);
    mask = 1 - mask;
    multiply(freqDomainImg, mask, lowPowerImg);
    ishift(mask);
    mask.convertTo(mask, CV_32F);
    multiply(mask, amplitude, mask);
    idftFunction(binaryImg, mask, cosine, sine);
    innerBinarizeThreshold = getBinaryImgThreshold(binaryImg, inputBinaryImgThreshold);
    threshold(binaryImg, invBinaryImg, innerBinarizeThreshold, UINT8_MAX, THRESH_BINARY);
    defectsInfo = bwareaopen(invBinaryImg, MIN_VALID_DEFECTS_SIZE);
    invBinaryImg = (invBinaryImg) < INT8_MAX;
}

Mat Detecter::createComplexMat(Mat& data)
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

Mat Detecter::createComplexMat(Mat& R, Mat& I)
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

void Detecter::dftFunction(Mat& data, Mat& Am, Mat& Cosine, Mat& Sine)
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

void Detecter::idftFunction(Mat& output, Mat Am, Mat Cosine, Mat Sine)
{
    exp(Am, Am);
    Mat R, I;
    multiply(Am, Cosine, R);
    multiply(Am, Sine, I);
    Mat complex = createComplexMat(R, I);
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

Mat Detecter::getEnergyMap(Mat& Am)
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

Mat Detecter::getSailencyMap(Mat& data)
{
    Mat complex = createComplexMat(data);
    Mat A, C, S;
    dftFunction(complex, A, C, S);
    Mat A_avg;
    blur(A, A_avg, Size(meanFilterSize, meanFilterSize), Point(-1, -1), BORDER_REPLICATE);
    A = A - A_avg;
    idftFunction(A, A, C, S);
    threshold(A, A, 0, 1, THRESH_OTSU);
    bwareaopen(A, 4);
    Mat kern = getStructuringElement(MORPH_CROSS, Size(3, 3));
    dilate(A, A, kern, Point(-1, -1), RValue);
    if (r2Value >= 0)
        circle(A, cv::Point((spatialDomainImg.cols / 2), (spatialDomainImg.rows / 2)), r2Value, 0, -1);
    if (r1Value >= 0)
        circle(A, cv::Point((spatialDomainImg.cols / 2), (spatialDomainImg.rows / 2)), r1Value, 1, -1);
    return A;
}

Mat Detecter::bwareaopen(Mat& data, int n)
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
                    if (labels.ptr<int>(j)[k] == i)
                        data.ptr<uchar>(j)[k] = 0;
                }
            }
            stats.at<int>(i, 4) = 0;
        }
    }
    return stats;
}

double Detecter::getBinaryImgThreshold(const Mat& input, double t)
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
