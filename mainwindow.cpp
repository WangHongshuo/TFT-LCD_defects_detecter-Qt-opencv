#include "mainwindow.h"
#include "ImageWidget-Qt/ImageWidget.h"
#include "QImageMatConvert/mat_qimage_convert.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QTextCodec>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->avg_filter_window->setValue(11);
    ui->avg_filter_window->setSingleStep(2);
    ui->avg_filter_window->setMinimum(1);

    ui->R->setValue(2);
    ui->R->setMinimum(0);

    ui->r1->setValue(1);
    ui->r1->setMinimum(-1);

    ui->r2->setValue(5);
    ui->r2->setMinimum(-1);

    ui->bw_t->setValue(10);
    ui->bw_t->setMinimum(1);

    ui->update_date->setEnabled(false);
    ui->auto_update->setEnabled(false);
    ui->auto_update->setChecked(false);

    ui->show_p_img->setEnableRecordLastParameters(true);
    ui->show_bw_img->setEnableRecordLastParameters(true);
    ui->show_DFT_img->setEnableRecordLastParameters(true);
    ui->show_input_img->setEnableRecordLastParameters(true);
    ui->show_p_DFT_img->setEnableRecordLastParameters(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_open_file_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Image"),
        "",
        tr("Image File(*.bmp *.jpg *.jpeg *.png)"));
    QTextCodec* code = QTextCodec::codecForName("gb18030");
    std::string name = code->fromUnicode(filename).data();
    M_input_img = imread(name, IMREAD_GRAYSCALE);
    if (!M_input_img.data) {
        ui->update_date->setEnabled(false);
        ui->auto_update->setEnabled(false);
        is_img_load = false;
        ui->show_input_img->clear();
        ui->show_DFT_img->clear();
        ui->show_p_DFT_img->clear();
        ui->show_p_img->clear();
        ui->show_bw_img->clear();
        QMessageBox msgBox;
        msgBox.setText(tr("Image data is null!"));
        msgBox.exec();
    } else {
        int temp;
        if (M_input_img.cols < M_input_img.rows)
            temp = M_input_img.cols;
        else
            temp = M_input_img.rows;
        if (temp % 2 == 0)
            temp -= 1;
        ui->avg_filter_window->setMaximum(temp);
        ui->update_date->setEnabled(true);
        ui->auto_update->setEnabled(true);
        is_img_load = true;
        timer.start();
        get_parameters();
        defect.set_parameters(M_input_img, avg_filter_window_size, R_value, r1_value, r2_value, bw_t_value);
        show_images();
        ui->p_time->setText(QString::number(timer.elapsed()) + " Ms");
    }
}

void MainWindow::update()
{
    if (is_img_load) {
        timer.start();
        get_parameters();
        defect.set_parameters(avg_filter_window_size, R_value, r1_value, r2_value, bw_t_value);
        show_images();
        ui->p_time->setText(QString::number(timer.elapsed()) + " Ms");
    }
}

void MainWindow::show_images()
{
    Q_input_img = Mat2QImage_with_pointer(M_input_img);
    ui->show_input_img->setImageWithPointer(&Q_input_img);
    Q_DFT_img = Mat2QImage_with_pointer((*defect.DFT_img));
    ui->show_DFT_img->setImageWithPointer(&Q_DFT_img);
    Q_p_DFT_img = Mat2QImage_with_pointer((*defect.p_DFT_img));
    ui->show_p_DFT_img->setImageWithPointer(&Q_p_DFT_img);
    Q_output_img = Mat2QImage_with_pointer((*defect.p_img));
    ui->show_p_img->setImageWithPointer(&Q_output_img);
    Q_bw_img = Mat2QImage_with_pointer((*defect.bw_img));
    ui->show_bw_img->setImageWithPointer(&Q_bw_img);
}

void MainWindow::get_parameters()
{
    // 获取均值滤波器窗口大小，R，r1，r2，二值化阈值参数
    avg_filter_window_size = ui->avg_filter_window->value();
    if (avg_filter_window_size % 2 == 0) {
        avg_filter_window_size -= 1;
        ui->avg_filter_window->setValue(avg_filter_window_size);
    }
    R_value = ui->R->value();
    r1_value = ui->r1->value();
    r2_value = ui->r2->value();
    bw_t_value = ui->bw_t->value();
}

void MainWindow::on_update_date_clicked()
{
    update();
}

void MainWindow::on_auto_update_clicked(bool checked)
{
    if (checked) {
        connect(ui->avg_filter_window, SIGNAL(valueChanged(int)), this, SLOT(update()));
        connect(ui->R, SIGNAL(valueChanged(int)), this, SLOT(update()));
        connect(ui->r1, SIGNAL(valueChanged(int)), this, SLOT(update()));
        connect(ui->r2, SIGNAL(valueChanged(int)), this, SLOT(update()));
        connect(ui->bw_t, SIGNAL(valueChanged(int)), this, SLOT(update()));
        update();
    } else {
        disconnect(ui->avg_filter_window, SIGNAL(valueChanged(int)), this, SLOT(update()));
        disconnect(ui->R, SIGNAL(valueChanged(int)), this, SLOT(update()));
        disconnect(ui->r1, SIGNAL(valueChanged(int)), this, SLOT(update()));
        disconnect(ui->r2, SIGNAL(valueChanged(int)), this, SLOT(update()));
        disconnect(ui->bw_t, SIGNAL(valueChanged(int)), this, SLOT(update()));
    }
}
