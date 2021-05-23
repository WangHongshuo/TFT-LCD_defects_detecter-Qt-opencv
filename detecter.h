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
    /** @brief                                  设置参数并处理数据
     *  @param data                             图像
     *  @param filter_size                      中值滤波器窗口大小
     *  @param R                                峰值点邻域大小
     *  @param r1                               中心去除半径大小
     *  @param r2                               中心保护半径大小
     *  @param bw_t                             二值化阈值参数
     *  @return                                 缺陷相关信息Mat的引用
     */
    Mat& setParametersAndProc(const Mat& data, int filter_size, int R, int r1, int r2, int binarizeThreshold);
    Mat& setParametersAndProc(const Mat* data, int filter_size, int R, int r1, int r2, int binarizeThreshold);
    Mat& setParametersAndProc(int filter_size, int R, int r1, int r2, int binarizeThreshold);
    Mat& getSpatialDomainImg();
    Mat& getFreqDomainImg();
    Mat& getFreqDomainMask();
    Mat& getLowPowerImg();
    Mat& getBinaryImg();
    Mat& getInvBinaryImg();

private:
    const static int MIN_VALID_DEFECTS_SIZE = 5; // 最小缺陷有效面积
    Mat spatialDomainImg;
    Mat freqDomainImg;
    Mat freqDomainMask;
    Mat lowPowerImg;
    Mat binaryImg;
    Mat invBinaryImg;
    Mat defectsInfo;

    Mat complexMat;

    Mat amplitude;
    Mat cosine;
    Mat sine;
    double innerBinarizeThreshold;

    int meanFilterSize;
    int RValue;
    int r1Value;
    int r2Value;
    int inputBinaryImgThreshold;

    /** @brief                                  主逻辑
     *  @return                                 void
     */
    void mainProc();
    /** @brief                                  处理频域图像
     *  @return                                 void
     */
    void procFrequencyDomainImg();
    /** @brief                                  创建复数Mat
     *  @param data                             输入Mat
     *  @return                                 复数Mat
     */
    Mat createComplexMat(Mat& data);
    /** @brief                                  创建复数Mat
     *  @param R                                实部Mat
     *  @param I                                虚部Mat
     *  @return                                 复数Mat
     */
    Mat createComplexMat(Mat& R, Mat& I);
    /** @brief                                  DFT变换
     *  @param data                             输入Mat
     *  @param Am                               振幅Mat
     *  @param Cosine                           欧拉公式中的Cos(theta)
     *  @param Sine                             欧拉公式中的Sin(theta)
     *  @return                                 void
     */
    void dftFunction(Mat& data, Mat& amplitude, Mat& cosine, Mat& sine);
    /** @brief                                  IDFT变换
     *  @param data                             输出Mat
     *  @param Am                               振幅Mat
     *  @param Cosine                           欧拉公式中的Cos(theta)
     *  @param Sine                             欧拉公式中的Sin(theta)
     *  @return                                 void
     */
    void idftFunction(Mat& output, Mat amplitude, Mat cosine, Mat sine);
    /** @brief                                  获取能量谱
      * @param Am                               振幅谱Mat
      * @return                                 能量谱Mat
     **/
    Mat getEnergyMap(Mat& amplitude);
    /** @brief                                  把能量谱的零频点平移到正中心
     *  @param data                             未平移的能量谱Mat
     *  @return                                 void
     */
    void shift(Mat& data);
    /** @brief                                  把平移后的能量谱复原
     *  @param data                             后平移的能量谱Mat
     *  @return                                 void
     */
    void ishift(Mat& data);
    /** @brief                                  获取显著图
     *  @param data                             平移后的频谱图Mat
     *  @return                                 显著图Mat
     */
    Mat getSailencyMap(Mat& data);
    /** @brief                                  去除二值图像中面积小于n的连通区域
     *  @param data                             二值图像Mat
     *  @param n                                阈值int
     *  @return                                 缺陷信息
     */
    Mat bwareaopen(Mat& data, int n);
    /** @brief                                  获取二值化阈值算法
     *  @param data                             灰度图像Mat
     *  @param t                                算法中的阈值int
     *  @return                                 二值化阈值int
     */
    double getBinaryImgThreshold(const Mat& input, double t);
    /** @brief                                  设置参数并更新数据
     *  @param filter_size                      中值滤波器窗口大小
     *  @param R                                峰值点邻域大小
     *  @param r1                               中心去除半径大小
     *  @param r2                               中心保护半径大小
     *  @param bw_t                             二值化阈值参数
     *  @return                                 void
     */
    void setParameters(int filter_size, int R, int r1, int r2, int binarizeThreshold);
};

#endif // DETECTER_H
