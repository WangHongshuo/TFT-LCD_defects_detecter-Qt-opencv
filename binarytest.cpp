#include "binarytest.h"
#include "ui_binarytest.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "ImageWidget-Qt/ImageWidget.h"
#include "QImageMatConvert/mat_qimage_convert.h"

BinaryTest::BinaryTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BinaryTest)
{
    ui->setupUi(this);
}

BinaryTest::~BinaryTest()
{
    delete ui;
}

void BinaryTest::setImg(Mat &img)
{
    if (img.empty()) {
        return;
    }
    mInputImg = img.clone();
    normalize(mInputImg, mInputImg, 0, UINT8_MAX, NORM_MINMAX);
    procImg();
}

void BinaryTest::closeEvent(QCloseEvent *event)
{
    emit sendWindowsCloseEvent();
}

void BinaryTest::procImg()
{
    if (mInputImg.empty()) {
        return;
    }
    threshold(mInputImg, mOutputImg, binaryThreshold, UINT8_MAX, THRESH_BINARY);
    qProcessedImg = ConvertMatToQImage(mOutputImg);
    ui->iwProcessedImg->setImage(qProcessedImg);
}

void BinaryTest::on_qsbBinaryThreshold_valueChanged(int arg1)
{
    binaryThreshold = double(arg1);
    procImg();
}

