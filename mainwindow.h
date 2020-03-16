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
    void on_open_file_clicked();
    void on_update_date_clicked();
    void on_auto_update_clicked(bool checked);
    void update();
    void show_images();
    void get_parameters();

private:
    Ui::MainWindow* ui;
    Mat M_input_img;
    QImage Q_input_img;
    QImage Q_DFT_img;
    QImage Q_p_DFT_img;
    QImage Q_output_img;
    QImage Q_bw_img;
    Detecter defect;

    int avg_filter_window_size;
    int R_value;
    int r1_value;
    int r2_value;
    int bw_t_value;

    QElapsedTimer timer;
    bool is_img_load;
};

#endif // MAINWINDOW_H
