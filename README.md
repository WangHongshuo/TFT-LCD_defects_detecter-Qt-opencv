# TFT-LCD defects detecter based on Qt and opencv
## Introduction: ##

This project started in 2017.11.16 and finished in 2017.12.11. I translated Matlab script into C++ after proving it in Matlab. It's not very hard for me to do this with the experience of the licence plate recognition project, but there are still a lot of space to improve.

该项目C++版本开始于2017.11.16，结束于2017.12.11。是先在Matlab上验证算法后又翻到C++的，有了以前车牌识别的经验后，做起来感觉不那么吃力了，不过还是有很多不足的地方。 

## Algorithm Introduction: 

The improved saliency model was applied to the detection of high-energy frequency components in the spectrum which was obtained from the original image by two-dimensional discrete Fourier transform (DFT). The neighborhood of those areas associated with high-energy frequency components inthespectrum that represented background texture was set to zero, and then aspatial domain image was reconstructed by the two-dimensional inverse discrete Fourier transform (IDFT) in order to remove the background texture and retain the defects.

用改进的显著性模型检测由面板图像二维离散傅里叶变换获得的能量谱中的高能量成分并将该部分消除，再经过二维离散傅里叶逆变换对图像重构达到去除空域图像周期性纹理和保留缺陷的目的。

## Screenshots:  ##
![img](https://github.com/WangHongshuo/Readme_Images_Repository/blob/master/TFT-LCD_defects_detecter-Qt-opencv/Demo.gif)

## Change Log: ##

- 2023.04.02

添加二值化算法处理结果做对比

binarization algorithm was added for comparison

- 2018.09.16:

added algorithm introduction and reduced deepcopy.

添加原理介绍，减少了处理过程中的深拷贝操作。

- 2018.08.04:

updated comments and fixed errors in UI.

更新了注释，修复UI错误。

- 2018.01.09:

set `ImageWidget` to submodule.

将 `ImageWidget` 设置为子模块。

- 2017.12.16 #2：

updated `ImageWidget`, `selectrect` and `Mat_QImage_convert` synchronously.

同步更新了`ImageWidget`, `selectrect`和`Mat_QImage_convert`.

- 2017.12.16 #1：

designed the UI.

设计了UI。

- 2017.12.11：

finished basic function.

完成基本功能。

- 2017.12.09：

can't upload due to some issues.

暂时无法公开。