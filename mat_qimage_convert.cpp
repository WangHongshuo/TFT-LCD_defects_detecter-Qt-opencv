#include "mat_qimage_convert.h"
#include <QDebug>

// cv::Mat::data and QImage::bits() are the same
QImage Mat2QImage_with_pointer(cv::Mat &src)
{
    // 1 channel
    if(src.type() == CV_8UC1)
    {
        QImage dst(src.data,src.cols, src.rows,src.step, QImage::Format_Indexed8);

        dst.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            dst.setColor(i, qRgb(i, i, i));
        }
        return dst;
    }
    // 3 channels
    else if(src.type() == CV_8UC3)
    {
        // warning: pay attention to this:
        cv::cvtColor(src,src,CV_BGR2RGB);
        // it will change BRG to RBG in src QImage
        QImage dst(src.data, src.cols, src.rows, src.step, QImage::Format_RGB888);
        return dst;
    }
    // 4 channels
    else if(src.type() == CV_8UC4)
    {
        QImage dst(src.data, src.cols, src.rows, src.step, QImage::Format_ARGB32);
        return dst;
    }
    else
    {
        return QImage();
    }
}

// cv::Mat::data and QImage::bits() are the same
cv::Mat QImage2Mat_with_pointer(QImage &src)
{
    cv::Mat dst;
//    qDebug() << src.format();
    switch(src.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        dst = cv::Mat(src.height(), src.width(), CV_8UC4, (void*)src.constBits(), src.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        dst = cv::Mat(src.height(), src.width(), CV_8UC3, (void*)src.constBits(), src.bytesPerLine());
        // warning: pay attention to this:
        cv::cvtColor(dst, dst, CV_BGR2RGB);
        // it will change BRG to RBG in src QImage
        break;
    case QImage::Format_Indexed8:
        dst = cv::Mat(src.height(), src.width(), CV_8UC1, (void*)src.constBits(), src.bytesPerLine());
        break;
    }
    return dst;
}

// cv::Mat::data and QImage::bits() are different
QImage Mat2QImage_with_data(cv::Mat &src)
{
    // 1 channel
    if(src.type() == CV_8UC1)
    {
        QImage dst(src.data,src.cols, src.rows,src.step, QImage::Format_Indexed8);
        dst.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            dst.setColor(i, qRgb(i, i, i));
        }
        return dst.copy();
    }
    // 3 channels
    else if(src.type() == CV_8UC3)
    {
        cv::Mat temp;
        cv::cvtColor(src,temp,CV_BGR2RGB);
        QImage dst(temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
        return dst.copy();
    }
    // 4 channels
    else if(src.type() == CV_8UC4)
    {
        QImage dst(src.data, src.cols, src.rows, src.step, QImage::Format_ARGB32);
        return dst.copy();
    }
    else
    {
        return QImage();
    }
}

// cv::Mat::data and QImage::bits() are different
cv::Mat QImage2Mat_with_data(QImage &src)
{
    cv::Mat dst;
//    qDebug() << src.format();
    switch(src.format())
    {
    // 4 channels
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
    {
        dst = cv::Mat(src.height(), src.width(), CV_8UC4, (void*)src.constBits(), src.bytesPerLine());
        return dst.clone();
    }
    // 3 channels
    case QImage::Format_RGB888:
    {
        dst = cv::Mat(src.height(), src.width(), CV_8UC3, (void*)src.constBits(), src.bytesPerLine());
        cv::Mat dstTemp;
        cv::cvtColor(dst, dstTemp, CV_BGR2RGB);
        return  dstTemp;
    }
    // 1 channel
    case QImage::Format_Indexed8:
    {
        dst = cv::Mat(src.height(), src.width(), CV_8UC1, (void*)src.constBits(), src.bytesPerLine());
        return dst.clone();
    }
    }
}
