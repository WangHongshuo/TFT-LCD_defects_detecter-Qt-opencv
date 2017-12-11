#ifndef DETECTER_H
#define DETECTER_H
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

class Detecter
{
public:
    Detecter();
    ~Detecter();
    void set_img(const Mat &data);
    void set_img(const Mat* data);

    Mat* ori_img;
    Mat* DFT_img;
    Mat* p_DFT_img;
    Mat* p_img;
    Mat* bw_img;
    int img_width;
    int img_height;

private:
    void main_task();
    Mat create_complex_Mat(Mat &data);
    Mat create_complex_Mat(Mat &R, Mat &I);
    void DFT_function(Mat &data, Mat &Am, Mat &Cosine, Mat &Sine);
    void IDFT_function(Mat &output, Mat Am, Mat Cosine, Mat Sine);
    Mat get_energyMap(Mat Am);
    void shift(Mat &data);
    void ishift(Mat &data);
    Mat get_sailencyMap(Mat &data);
    void bwareaopen(Mat &data, int n);
    double get_bw_value(const Mat &input, double t);
    Mat complex_Mat;

    Mat Am;
    Mat Cosine;
    Mat Sine;
    double bw_t;

};

#endif // DETECTER_H
