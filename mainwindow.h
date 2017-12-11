#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include "ImageWidget.h"
#include "mat_qimage_convert.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "detecter.h"

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_open_file_clicked();

private:
    Ui::MainWindow *ui;

    Mat M_input_img;
    QImage Q_input_img;
    QImage Q_DFT_img;
    QImage Q_p_DFT_img;
    QImage Q_output_img;
    QImage Q_bw_img;
    Detecter defect;

};

#endif // MAINWINDOW_H
