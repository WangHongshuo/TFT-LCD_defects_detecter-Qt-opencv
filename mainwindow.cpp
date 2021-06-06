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
        ui->qtwDefectsInfo->setRowCount(0);
        QMessageBox msgBox;
        msgBox.setText(tr("Image data is null!"));
        msgBox.exec();
    } else {
        int temp = std::min(mInputImg.cols, mInputImg.rows);
        if (temp % 2 == 0)
            temp -= 1;
        ui->qsbAvgFilterSize->setMaximum(temp);
        ui->qpbUpdate->setEnabled(true);
        ui->qcbAutoUpdate->setEnabled(true);
        isImgLoad = true;
        timer.start();
        getParameters();
        defectsInfo = defect.setParametersAndProc(mInputImg, avgFilterSize, R, r1, r2, binaryThreshold);
        updateDefectsInfo(defectsInfo);
        showImages();
        ui->qlProcTime->setText(QString::number(timer.elapsed()) + " Ms");
    }
}

void MainWindow::update()
{
    if (!isImgLoad) {
        return;
    }
    getParameters();
    timer.start();
    defectsInfo = defect.setParametersAndProc(avgFilterSize, R, r1, r2, binaryThreshold);
    ui->qlProcTime->setText(QString::number(timer.elapsed()) + " Ms");
    showImages();
    updateDefectsInfo(defectsInfo);
}

void MainWindow::updateDefectsInfo(Mat &defectsInfo)
{
    defectIndex.clear();
    ui->qtwDefectsInfo->setRowCount(0);
    int row = 0;
    for (int i=1;i<defectsInfo.rows;i++){
        if (defectsInfo.ptr<int>(i)[4] == 0){
            continue;
        }
        ui->qtwDefectsInfo->insertRow(ui->qtwDefectsInfo->rowCount());
        ui->qtwDefectsInfo->setItem(row, 0, new QTableWidgetItem("unkown type"));
        ui->qtwDefectsInfo->setItem(row, 1, new QTableWidgetItem(QString("%1").arg(defectsInfo.ptr<int>(i)[4])));
        ui->qtwDefectsInfo->setItem(row, 2,
            new QTableWidgetItem(QString("x:%1, y:%2, w:%3, h:%4").arg(defectsInfo.ptr<int>(i)[0]).arg(defectsInfo.ptr<int>(i)[1])
                                     .arg(defectsInfo.ptr<int>(i)[2]).arg(defectsInfo.ptr<int>(i)[3])));
        defectIndex.push_back(i);
        row++;
    }
}

void MainWindow::showImages()
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

void MainWindow::getParameters()
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

void MainWindow::on_qtwDefectsInfo_cellClicked(int row, int column)
{
    ui->iwInvBinaryImg->removeAllROI();
    int i = defectIndex[row];
    ui->iwInvBinaryImg->addROI(QRect(defectsInfo.ptr<int>(i)[0], defectsInfo.ptr<int>(i)[1],defectsInfo.ptr<int>(i)[2],defectsInfo.ptr<int>(i)[3]), i);
}

