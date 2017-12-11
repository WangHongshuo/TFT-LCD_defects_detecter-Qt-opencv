#ifndef MAT_QIMAGE_CONVERT_H
#define MAT_QIMAGE_CONVERT_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <QImage>

QImage Mat2QImage(cv::Mat &mat);
cv::Mat QImage2Mat(QImage image);

#endif // MAT_QIMAGE_CONVERT_H
