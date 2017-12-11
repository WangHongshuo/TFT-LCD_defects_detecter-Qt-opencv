#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFileDialog>
#include <QTextCodec>
#include <QMessageBox>
#include <QDebug>
#include <QTime>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setFixedSize(820,602);
    ui->setupUi(this);
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
    M_input_img = cv::imread(name,IMREAD_GRAYSCALE);
//    qDebug() << M_input_img.channels();
    if(!M_input_img.data)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Image data is null!"));
        msgBox.exec();
    }
    else
    {
        QTime a;
        a.start();
        defect.set_img(M_input_img);
        Q_input_img = Mat2QImage(M_input_img);
        ui->show_input_img->setImage(&Q_input_img);
        Q_DFT_img = Mat2QImage((*defect.DFT_img));
        ui->show_DFT_img->setImage(&Q_DFT_img);
        Q_p_DFT_img = Mat2QImage((*defect.p_DFT_img));
        ui->show_p_DFT_img->setImage(&Q_p_DFT_img);
        Q_output_img = Mat2QImage((*defect.p_img));
        ui->show_p_img->setImage(&Q_output_img);
        Q_bw_img = Mat2QImage((*defect.bw_img));
        ui->show_bw_img->setImage(&Q_bw_img);
        ui->p_time->setText(QString::number(a.elapsed())+" Ms");
    }
}
