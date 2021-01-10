/**     @file       detecter.h
 *      @author     Wang Hongshuo
 *      @date       2018/08/04
 *      @email      iswanghongshuo@foxmail.com
 *      @brief      利用改进后的显著性模型去检测能量谱中的高能量部分并将其去除，达到去除TFT-LCD面板背景纹理保留缺陷的效果
 */
#ifndef DETECTER_H
#define DETECTER_H
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

class Detecter {
public:
    Detecter();
    ~Detecter();
    /** @fn                                     设置参数
     *  @param  data                            图像
     *  @param  filter_size                     中值滤波器窗口大小
     *  @param  R                               峰值点邻域大小
     *  @param  r1                              中心去除半径大小
     *  @param  r2                              中心保护半径大小
     *  @param  bw_t                            二值化阈值参数
     *  @return                                 void
     */
    void setParameters(const Mat& data, int filter_size, int R, int r1, int r2, int binarizeThreshold);
    void setParameters(const Mat* data, int filter_size, int R, int r1, int r2, int binarizeThreshold);
    void setParameters(int filter_size, int R, int r1, int r2, int binarizeThreshold);
    const Mat* getSpatialDomainImg();
    const Mat* getFreqDomainImg();
    const Mat* getFreqDomainMask();
    const Mat* getLowPowerImg();
    const Mat* getBinaryImg();
    const Mat* getInvBinaryImg();

private:
    Mat* spatialDomainImg;
    Mat* freqDomainImg;
    Mat* freqDomainMask;
    Mat* lowPowerImg;
    Mat* binaryImg;
    Mat* invBinaryImg;
    int imgWidth;
    int imgHeight;

    Mat complexMat;

    Mat amplitude;
    Mat cosine;
    Mat sine;
    double binarizeThreshold;

    int meanFilterSize;
    int RValue;
    int r1Value;
    int r2Value;
    int bw_t_value;

    /** @fn                                     主逻辑
     *  @return                                 void
     */
    void main_task();
    /** @fn                                     处理频域图像
     *  @return                                 void
     */
    void proc_frequency_domain_img();
    /** @fn                                     创建复数Mat
     *  @param  data                            输入Mat
     *  @return                                 复数Mat
     */
    Mat create_complex_Mat(Mat& data);
    /** @fn                                     创建复数Mat
     *  @param  R                               实部Mat
     *  @param  I                               虚部Mat
     *  @return                                 复数Mat
     */
    Mat create_complex_Mat(Mat& R, Mat& I);
    /** @fn                                     DFT变换
     *  @param  data                            输入Mat
     *  @param  Am                              振幅Mat
     *  @param  Cosine                          欧拉公式中的Cos(theta)
     *  @param  Sine                            欧拉公式中的Sin(theta)
     *  @return                                 void
     */
    void DFT_function(Mat& data, Mat& amplitude, Mat& cosine, Mat& sine);
    /** @fn                                     IDFT变换
     *  @param  data                            输出Mat
     *  @param  Am                              振幅Mat
     *  @param  Cosine                          欧拉公式中的Cos(theta)
     *  @param  Sine                            欧拉公式中的Sin(theta)
     *  @return                                 void
     */
    void IDFT_function(Mat& output, Mat amplitude, Mat cosine, Mat sine);
    /** @fn                                     获取能量谱
     *  @param  Am                              振幅谱Mat
     *  @return                                 能量谱Mat
     */
    Mat get_energyMap(Mat& amplitude);
    /** @fn                                     把能量谱的零频点平移到正中心
     *  @param  data                            未平移的能量谱Mat
     *  @return                                 void
     */
    void shift(Mat& data);
    /** @fn                                     把平移后的能量谱复原
     *  @param  data                            后平移的能量谱Mat
     *  @return                                 void
     */
    void ishift(Mat& data);
    /** @fn                                     获取显著图
     *  @param  data                            平移后的频谱图Mat
     *  @return                                 显著图Mat
     */
    Mat get_sailencyMap(Mat& data);
    /** @fn                                     去除二值图像中面积小于n的连通区域
     *  @param  data                            二值图像Mat
     *  @param  n                               阈值int
     *  @return                                 void
     */
    void bwareaopen(Mat& data, int n);
    /** @fn                                     获取二值化阈值算法
     *  @param  data                            灰度图像Mat
     *  @param  t                               算法中的阈值int
     *  @return                                 二值化阈值int
     */
    double get_bw_value(const Mat& input, double t);
    /** @fn                                     设置参数并更新数据
     *  @param  filter_size                     中值滤波器窗口大小
     *  @param  R                               峰值点邻域大小
     *  @param  r1                              中心去除半径大小
     *  @param  r2                              中心保护半径大小
     *  @param  bw_t                            二值化阈值参数
     *  @return                                 void
     */
    void set_parameters_con(int filter_size, int R, int r1, int r2, int binarizeThreshold);
};

#endif // DETECTER_H
