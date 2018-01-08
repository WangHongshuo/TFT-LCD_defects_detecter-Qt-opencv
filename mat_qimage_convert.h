#ifndef MAT_QIMAGE_CONVERT_H
#define MAT_QIMAGE_CONVERT_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <QImage>

// ==================================================================================+
// Warning!
// When the src Mat channels is 3 or the src QImage format is RGB,
// the Mat2QImage_with_pointer and QImage2Mat_with_pointer will change the src image.
// When use Mat2QImage_with_pointer or Mat2QImage_with_pointer,
// the src Mat::data (src QImage::bits()) address
// and dst QImage::bits() (dst Mat::data) address are the same!
// ===================================================================================
QImage Mat2QImage_with_pointer(cv::Mat &src);
cv::Mat QImage2Mat_with_pointer(QImage &src);

QImage Mat2QImage_with_data(cv::Mat &src);
cv::Mat QImage2Mat_with_data(QImage &src);

#endif // MAT_QIMAGE_CONVERT_H
