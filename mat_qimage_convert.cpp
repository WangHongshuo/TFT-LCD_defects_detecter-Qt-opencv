#include "mat_qimage_convert.h"


QImage Mat2QImage_with_pointer(cv::Mat &mat)
{
    // 单通道
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.data,mat.cols, mat.rows,mat.step, QImage::Format_Indexed8);

        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        return image;
    }
    // 3通道
    else if(mat.type() == CV_8UC3)
    {
        cv::cvtColor(mat,mat,CV_BGR2RGB);
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image;
    }
    // 4通道
    else if(mat.type() == CV_8UC4)
    {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image;
    }
    else
    {
        return QImage();
    }
}

cv::Mat QImage2Mat_with_pointer(QImage &image)
{
    cv::Mat mat;
//    qDebug() << image.format();
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}

QImage Mat2QImage_with_data(cv::Mat &mat)
{
    // 单通道
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.data,mat.cols, mat.rows,mat.step, QImage::Format_Indexed8);

        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        return image.copy();
    }
    // 3通道
    else if(mat.type() == CV_8UC3)
    {
        cv::Mat temp;
        cv::cvtColor(mat,temp,CV_BGR2RGB);
        QImage image(temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
        return image.copy();
    }
    // 4通道
    else if(mat.type() == CV_8UC4)
    {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        return QImage();
    }
}
cv::Mat QImage2Mat_with_data(QImage &image)
{
    cv::Mat mat;
//    qDebug() << image.format();
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat.clone();
}
