#ifndef BINARYTEST_H
#define BINARYTEST_H

#include <QWidget>
#include <QImage>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

namespace Ui {
class BinaryTest;
}

class BinaryTest : public QWidget
{
    Q_OBJECT

public:
    explicit BinaryTest(QWidget *parent = nullptr);
    ~BinaryTest();
    void setImg(Mat& img);

signals:
    void sendWindowsCloseEvent();

private slots:
    void on_qsbBinaryThreshold_valueChanged(int arg1);

private:
    Ui::BinaryTest *ui;
    QImage qProcessedImg;
    Mat mInputImg;
    Mat mOutputImg;
    double binaryThreshold = 127.0;


    void closeEvent(QCloseEvent* event);
    void procImg();
};

#endif // BINARYTEST_H
