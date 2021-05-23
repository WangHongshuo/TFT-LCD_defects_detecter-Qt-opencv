#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "detecter.h"
#include <QImage>
#include <QMainWindow>
#include <QTime>
#include <QElapsedTimer>
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
    void update_defects_info(Mat& defectsInfo);
    void show_images();
    void get_parameters();

private:
    Ui::MainWindow* ui;
    Mat mInputImg;
    QImage qInputImg;
    QImage qDFTImg;
    QImage qProcessedDFTImg;
    QImage qProcessedImg;
    QImage qInvBinaryImg;
    Detecter defect;

    int avgFilterSize;
    int R;
    int r1;
    int r2;
    int binaryThreshold;

    QElapsedTimer timer;
    bool isImgLoad;
};

#endif // MAINWINDOW_H
