#ifndef MAT_QIMAGE_CONVERT_H
#define MAT_QIMAGE_CONVERT_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <QImage>

QImage Mat2QImage_with_pointer(cv::Mat &mat);
cv::Mat QImage2Mat_with_pointer(QImage &image);

QImage Mat2QImage_with_data(cv::Mat &mat);
cv::Mat QImage2Mat_with_data(QImage &image);

#endif // MAT_QIMAGE_CONVERT_H
