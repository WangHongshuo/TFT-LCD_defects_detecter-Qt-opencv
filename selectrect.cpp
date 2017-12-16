#include "selectrect.h"
#include <iostream>
#include <QDebug>

SelectRect::SelectRect(QWidget *parent) : QWidget(parent)
{
    image = new QImage;
    is_image_load = false;

    // 初始化右键菜单
    subMenu = new QMenu();
    subActionReset = subMenu->addAction(tr("重选"));
    subActionSave = subMenu->addAction(tr("另存为"));
    subActionExit = subMenu->addAction(tr("退出"));
    connect(subActionExit,SIGNAL(triggered()),this,SLOT(select_exit()));
    connect(subActionSave,SIGNAL(triggered()),this,SLOT(cut_img()));
    connect(subActionReset,SIGNAL(triggered()),this,SLOT(select_reset()));
    // 关闭后释放资源
    this->setAttribute(Qt::WA_DeleteOnClose);
}

SelectRect::~SelectRect()
{
//    disconnect(this->parent(),SIGNAL(parent_widget_size_changed(int,int)),this,SLOT(receive_parent_size_changed_value(int,int)));
//    disconnect(this,SIGNAL(select_mode_exit()),this->parent(),SLOT(is_select_mode_exit()));
    delete image;
    delete subMenu;
}

void SelectRect::paintEvent(QPaintEvent *event)
{
    // 背景
    QPainterPath mask;
    // 选中的范围
    QPainterPath select_area;
    // 椭圆
//    QRect boundingRectangle(rect.x,rect.y,rect.w,rect.h);
//    select_area.addEllipse(boundingRectangle);
    select_area.addRect(rect.x,rect.y,rect.w,rect.h);
    mask.addRect(this->geometry());
    QPainterPath drawMask =mask.subtracted(select_area);

    QPainter paint(this);
    paint.setOpacity(0.5);
    paint.fillPath(drawMask,QBrush(Qt::black));

}

void SelectRect::mousePressEvent(QMouseEvent *event)
{
        switch(event->button())
        {
        case Qt::LeftButton:
            mouse = Left;
            first_mouse_pos_x = event->x();
            first_mouse_pos_y = event->y();
//            qDebug() << first_mousePosX << first_mousePosX;
            break;
        case Qt::RightButton:
            mouse = Right;
            break;
        case Qt::MiddleButton:
            mouse = Mid;
            break;
        default:
            mouse = No;
    }
}

void SelectRect::mouseMoveEvent(QMouseEvent *event)
{
    if (mouse == Left )
    {
        // 限定在mask内
        rect.x = first_mouse_pos_x;
        rect.y = first_mouse_pos_y;
        int x = event->x();
        int y = event->y();
        if(x < 0)
            x = 0;
        else if(x > this->width())
            x = this->width();
        if(y < 0)
            y = 0;
        else if (y > this->height())
            y = this->height();
        rect.w = x - rect.x;
        rect.h = y - rect.y;

    }
    update();
}

void SelectRect::contextMenuEvent(QContextMenuEvent *event)
{
    subMenu->exec(QCursor::pos());
}

void SelectRect::select_exit()
{
    emit select_mode_exit();
    this->close();
}

void SelectRect::select_reset()
{
    rect.w = 0;
    rect.h = 0;
    update();
}

void SelectRect::cut_img()
{
    // 接受到截取框信息
    if(is_image_load)
    {
        rect_info data;
        data = rect;
        // 修正矩形坐标
        if(data.w < 0)
        {
            data.x += data.w;
            data.w = -data.w;
        }
        if(data.h < 0)
        {
            data.y += data.h;
            data.h = -data.h;
        }
//        qDebug() << data.x << data.y << data.w << data.h;
        if(data.w == 0 && data.h == 0)
        {
            QMessageBox msgBox(QMessageBox::Critical,tr("错误"),tr("未选取区域！"));
            msgBox.exec();
        }
        else
        {
            QImage* temp = new QImage;
            QImage* save_img = new QImage;
            *temp = image->copy();
            //        qDebug()<< temp << &mp_img;
            //        qDebug() << temp->size();
            //        qDebug() << this->width() * scalex << this->height() * scalex;
            *temp = temp->scaled(this->width() * scalex,this->height() * scalex,Qt::KeepAspectRatio);
            //        qDebug() << temp->size();
            // 计算相对于图像内的坐标
            int x,y,w,h;
            x = data.x - xtranslate;
            y = data.y - ytranslate;
            w = data.w;
            h = data.h;
            // 限定截取范围在图像内
            // 修正顶点
            if(x < 0)
            {
                w = data.w + x;
                x = 0;
            }
            if (y < 0)
            {
                h = data.h + y;
                y = 0;
            }
            if(x + data.w > temp->width())
                w = temp->width() - x;
            if(y + data.h > temp->height())
                h = temp->height() - y;
//            qDebug() << xtranslate << ytranslate << temp->width() << temp->height();
//            qDebug() << x << y << w << h;
            if(w > 0 && h > 0)
            {
                *save_img = temp->copy(x,y,w,h);
                QString filename = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                                "C:/",
                                                                tr("Images (*.png *.xpm *.jpg *.tiff *.bmp)"));
                save_img->save(filename);
            }
            else
            {
                QMessageBox msgBox(QMessageBox::Critical,tr("错误"),tr("未选中图像！"));
                msgBox.exec();
            }
            delete temp;
            delete save_img;
        }
    }
    else
    {
        QMessageBox msgBox(QMessageBox::Critical,tr("错误"),tr("未选中图像！"));
        msgBox.exec();
    }
}

void SelectRect::receive_parent_size_changed_value(int width, int height)
{
    this->setGeometry(0,0,width,height);
}
