#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFileDialog>
#include <QTextCodec>
#include <QMessageBox>
#include <QDebug>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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
    ui->bw_t->setMinimum(0);

    ui->update_date->setEnabled(false);
    ui->auto_update->setEnabled(false);
    ui->auto_update->setChecked(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_open_file_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Image"),
                                                    "F:/",
                                                    tr("Image File(*.bmp *.jpg *.jpeg *.png)"));
    QTextCodec *code = QTextCodec::codecForName("gb18030");
    std::string name = code->fromUnicode(filename).data();
    M_input_img = imread(name,IMREAD_GRAYSCALE);
    //    qDebug() << M_input_img.channels();
    if(!M_input_img.data)
    {
        ui->update_date->setEnabled(false);
        ui->auto_update->setEnabled(false);
        ui->auto_update->setChecked(false);
        is_img_load = false;
        QMessageBox msgBox;
        msgBox.setText(tr("Image data is null!"));
        msgBox.exec();
    }
    else
    {
        int temp;
        if(M_input_img.cols < M_input_img.rows)
            temp = M_input_img.cols;
        else
            temp = M_input_img.rows;
        if(temp % 2 == 0)
            temp -= 1;
        ui->avg_filter_window->setMaximum(temp);
        ui->update_date->setEnabled(true);
        ui->auto_update->setEnabled(true);
        is_img_load = true;
        update();
    }
}

void MainWindow::update()
{
    if(is_img_load)
    {
        a.start();

        avg_filter_window_size = ui->avg_filter_window->value();
        if(avg_filter_window_size % 2 == 0)
        {
            avg_filter_window_size -= 1;
            ui->avg_filter_window->setValue(avg_filter_window_size);
        }
        R_value = ui->R->value();
        r1_value = ui->r1->value();
        r2_value = ui->r2->value();
        bw_t_value = ui->bw_t->value();

        defect.set_parameters(M_input_img,avg_filter_window_size,R_value,r1_value,r2_value,bw_t_value);
        Q_input_img = Mat2QImage_with_pointer(M_input_img);
        ui->show_input_img->set_image_with_pointer(&Q_input_img,true,true);
        Q_DFT_img = Mat2QImage_with_pointer((*defect.DFT_img));
        ui->show_DFT_img->set_image_with_pointer(&Q_DFT_img,true,true);
        Q_p_DFT_img = Mat2QImage_with_pointer((*defect.p_DFT_img));
        ui->show_p_DFT_img->set_image_with_pointer(&Q_p_DFT_img,true,true);
        Q_output_img = Mat2QImage_with_pointer((*defect.p_img));
        ui->show_p_img->set_image_with_pointer(&Q_output_img,true,true);
        Q_bw_img = Mat2QImage_with_pointer((*defect.bw_img));
        ui->show_bw_img->set_image_with_pointer(&Q_bw_img,true,true);
        ui->p_time->setText(QString::number(a.elapsed())+" Ms");
    }
}

void MainWindow::on_update_date_clicked()
{
    update();
}

void MainWindow::on_auto_update_clicked(bool checked)
{
    if(checked)
    {
        connect(ui->avg_filter_window,SIGNAL(valueChanged(int)),this,SLOT(update()));
        connect(ui->R,SIGNAL(valueChanged(int)),this,SLOT(update()));
        connect(ui->r1,SIGNAL(valueChanged(int)),this,SLOT(update()));
        connect(ui->r2,SIGNAL(valueChanged(int)),this,SLOT(update()));
        connect(ui->bw_t,SIGNAL(valueChanged(int)),this,SLOT(update()));
    }
    else
    {
        disconnect(ui->avg_filter_window,SIGNAL(valueChanged(int)),this,SLOT(update()));
        disconnect(ui->R,SIGNAL(valueChanged(int)),this,SLOT(update()));
        disconnect(ui->r1,SIGNAL(valueChanged(int)),this,SLOT(update()));
        disconnect(ui->r2,SIGNAL(valueChanged(int)),this,SLOT(update()));
        disconnect(ui->bw_t,SIGNAL(valueChanged(int)),this,SLOT(update()));
    }
}
