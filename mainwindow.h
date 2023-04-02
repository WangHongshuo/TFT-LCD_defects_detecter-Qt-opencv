#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "detecter.h"
#include "binarytest.h"
#include <QImage>
#include <QMainWindow>
#include <QTime>
#include <QElapsedTimer>
#include <QTableWidgetItem>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private slots:
    void on_qpbOpenFile_clicked();
    void on_qpbUpdate_clicked();
    void on_qcbAutoUpdate_clicked(bool checked);
    void update();
    void updateDefectsInfo(Mat& defectsInfo);
    void showImages();
    void getParameters();
    void on_qtwDefectsInfo_cellClicked(int row, int column);
    void on_qpbBinaryTest_clicked();
    void recvBinaryTestWndClose();


private:
    Ui::MainWindow* ui;
    BinaryTest* binaryTestWnd;
    Mat mInputImg;
    QImage qInputImg;
    QImage qDFTImg;
    QImage qProcessedDFTImg;
    QImage qProcessedImg;
    QImage qInvBinaryImg;
    Detecter defect;

    Mat defectsInfo;
    std::vector<int> defectIndex;

    int avgFilterSize;
    int R;
    int r1;
    int r2;
    int binaryThreshold;

    QElapsedTimer timer;
    bool isImgLoad;
};

#endif // MAINWINDOW_H
