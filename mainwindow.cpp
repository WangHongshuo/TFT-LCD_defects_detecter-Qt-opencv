#include "mainwindow.h"
#include "ImageWidget-Qt/ImageWidget.h"
#include "QImageMatConvert/mat_qimage_convert.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->qsbAvgFilterSize->setValue(11);
    ui->qsbAvgFilterSize->setSingleStep(2);
    ui->qsbAvgFilterSize->setMinimum(1);

    ui->qsbR->setValue(2);
    ui->qsbR->setMinimum(0);

    ui->qsbr1->setValue(1);
    ui->qsbr1->setMinimum(-1);

    ui->qsbr2->setValue(5);
    ui->qsbr2->setMinimum(-1);

    ui->qsbBinaryThreshold->setValue(10);
    ui->qsbBinaryThreshold->setMinimum(1);

    ui->qpbUpdate->setEnabled(false);
    ui->qcbAutoUpdate->setEnabled(false);
    ui->qcbAutoUpdate->setChecked(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_qpbOpenFile_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Image"),
                                                    "",
                                                    tr("Image File(*.bmp *.jpg *.jpeg *.png)"));
    mInputImg = imread(filename.toStdString(), IMREAD_GRAYSCALE);
    if (!mInputImg.data) {
        ui->qpbUpdate->setEnabled(false);
        ui->qpbUpdate->setEnabled(false);
        isImgLoad = false;
        ui->iwInputImg->clear();
        ui->iwDFTImg->clear();
        ui->iwProccessedDFTImg->clear();
        ui->iwProcessedImg->clear();
        ui->iwInvBinaryImg->clear();
        QMessageBox msgBox;
        msgBox.setText(tr("Image data is null!"));
        msgBox.exec();
    } else {
        int temp;
        if (mInputImg.cols < mInputImg.rows)
            temp = mInputImg.cols;
        else
            temp = mInputImg.rows;
        if (temp % 2 == 0)
            temp -= 1;
        ui->qsbAvgFilterSize->setMaximum(temp);
        ui->qpbUpdate->setEnabled(true);
        ui->qcbAutoUpdate->setEnabled(true);
        isImgLoad = true;
        timer.start();
        get_parameters();
        Mat defectsInfo = defect.setParametersAndProc(mInputImg, avgFilterSize, R, r1, r2, binaryThreshold);
        update_defects_info(defectsInfo);
        show_images();
        ui->qlProcTime->setText(QString::number(timer.elapsed()) + " Ms");
    }
}

void MainWindow::update()
{
    if (isImgLoad) {
        timer.start();
        get_parameters();
        Mat defectsInfo = defect.setParametersAndProc(avgFilterSize, R, r1, r2, binaryThreshold);
        update_defects_info(defectsInfo);
        show_images();
        ui->qlProcTime->setText(QString::number(timer.elapsed()) + " Ms");
    }
}

void MainWindow::update_defects_info(Mat &defectsInfo)
{
    for (int i=1;i<defectsInfo.rows;i++){
        if (defectsInfo.ptr<int>(i)[4] == 0){
            continue;
        }
    }
}

void MainWindow::show_images()
{
    qInputImg = ConvertMatToQImage(mInputImg);
    ui->iwInputImg->setImage(qInputImg);
    qDFTImg = ConvertMatToQImage(defect.getFreqDomainImg());
    ui->iwDFTImg->setImage(qDFTImg);
    qProcessedDFTImg = ConvertMatToQImage(defect.getLowPowerImg());
    ui->iwProccessedDFTImg->setImage(qProcessedDFTImg);
    qProcessedImg = ConvertMatToQImage(defect.getBinaryImg());
    ui->iwProcessedImg->setImage(qProcessedImg);
    qInvBinaryImg = ConvertMatToQImage(defect.getInvBinaryImg());
    ui->iwInvBinaryImg->setImage(qInvBinaryImg);
}

void MainWindow::get_parameters()
{
    // 获取均值滤波器窗口大小，R，r1，r2，二值化阈值参数
    avgFilterSize = ui->qsbAvgFilterSize->value();
    if (avgFilterSize % 2 == 0) {
        avgFilterSize -= 1;
        ui->qsbAvgFilterSize->setValue(avgFilterSize);
    }
    R = ui->qsbR->value();
    r1 = ui->qsbr1->value();
    r2 = ui->qsbr2->value();
    binaryThreshold = ui->qsbBinaryThreshold->value();
}

void MainWindow::on_qpbUpdate_clicked()
{
    update();
}

void MainWindow::on_qcbAutoUpdate_clicked(bool checked)
{
    if (checked) {
        connect(ui->qsbAvgFilterSize, SIGNAL(valueChanged(int)), this, SLOT(update()));
        connect(ui->qsbR, SIGNAL(valueChanged(int)), this, SLOT(update()));
        connect(ui->qsbr1, SIGNAL(valueChanged(int)), this, SLOT(update()));
        connect(ui->qsbr2, SIGNAL(valueChanged(int)), this, SLOT(update()));
        connect(ui->qsbBinaryThreshold, SIGNAL(valueChanged(int)), this, SLOT(update()));
        update();
    } else {
        disconnect(ui->qsbAvgFilterSize, SIGNAL(valueChanged(int)), this, SLOT(update()));
        disconnect(ui->qsbR, SIGNAL(valueChanged(int)), this, SLOT(update()));
        disconnect(ui->qsbr1, SIGNAL(valueChanged(int)), this, SLOT(update()));
        disconnect(ui->qsbr2, SIGNAL(valueChanged(int)), this, SLOT(update()));
        disconnect(ui->qsbBinaryThreshold, SIGNAL(valueChanged(int)), this, SLOT(update()));
    }
}
