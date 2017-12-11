#include "ImageWidget.h"
#include <iostream>
#include <QDebug>
#include "selectrect.h"

ImageWidget::ImageWidget(QWidget *parent):QWidget(parent),scalex(1),scaley(1),xtranslate(0),ytranslate(0),mouse(No)
{
    is_image_load = false;
    is_select_mode = false;
    iniActions();
}

ImageWidget::~ImageWidget(void)
{
    if(is_image_load)
        mp_img = NULL;
}


void ImageWidget::wheelEvent(QWheelEvent *e)
{
    if(is_image_load && !is_select_mode)
    {
        int numDegrees = e->delta();
        if(numDegrees > 0)
        {
            zoomout();
        }
        if(numDegrees < 0)
        {
            zoomin();
        }
        update();
    }
}
void ImageWidget::mousePressEvent(QMouseEvent *e)
{
    if(is_image_load)
    {
        switch(e->button())
        {
        case Qt::LeftButton:
            mouse = Left;
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
//        qDebug() << mouse;
        //初次按键事件鼠标坐标
        mousePosX = e->x();
        mousePosY = e->y();
    }
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if(mouse == Left)
    {
        last_x_pos = xtranslate;
        last_y_pos = ytranslate;
    }
}


void ImageWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(mouse == Left )
    {
        //xtranslate = e->x() - mousePosX;
        //ytranslate = e->y() - mousePosY;

//        qDebug() << "e.mouseposX: " << mousePosX << "e.mouseposY: " << mousePosY;
//        qDebug() << "e.x: " << e->x() << "e.y: " << e->y();
        //相对移动坐标
        translate(e->x() - mousePosX,e->y() - mousePosY);
    }
}

void ImageWidget::translate(int x,int y)
{
    xtranslate = last_x_pos + x;
    ytranslate = last_y_pos + y;
    update();
}

void ImageWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(200,200,200)));
    painter.drawRect(0,0,this->width(),this->height());
    if(!is_image_load)
        return;
    painter.drawImage(QPoint(xtranslate,ytranslate),mp_img->scaled(this->width()*scalex,this->height()*scaley,Qt::KeepAspectRatio));
//    qDebug() << scalex;
}

void ImageWidget::contextMenuEvent(QContextMenuEvent *e)
{
    mMenu->exec(QCursor::pos());
    //mousePosX = QCursor::pos().x();
    //mousePosY = QCursor::pos().y();
}

void ImageWidget::resetPos()
{
    xtranslate = 0;
    ytranslate = 0;
    scalex = 1;
    scaley = 1;
    last_x_pos = 0;
    last_y_pos = 0;
    update();
}

void ImageWidget::zoomout()
{
    if(scalex<=12&&scaley<=12)
    {
        scalex *= 1.1;
        scaley *= 1.1;
    }
    update();
}

void ImageWidget::zoomin()
{
    if(scalex>=0.05&&scaley>=0.05)
    {
        scalex *= 1.0/1.1;
        scaley *= 1.0/1.1;
    }
    update();
}

void ImageWidget::select()
{
    if(is_image_load)
    {
        is_select_mode = true;
        SelectRect* m = new SelectRect(this);
        m->setGeometry(0,0,this->geometry().width(),this->geometry().height());
        connect(m,SIGNAL(select_mode_exit()),this,SLOT(is_select_mode_exit()));
        m->set_image(mp_img);
        m->scalex = scalex;
        m->scaley = scaley;
        m->xtranslate = xtranslate;
        m->ytranslate = ytranslate;
        m->show();
    }
}


void ImageWidget::iniActions()
{
    mMenu = new QMenu();
    mActionResetPos = mMenu->addAction(tr("重置"));
    mActionSave		= mMenu->addAction(tr("另存为"));
    mActionSelect   = mMenu->addAction(tr("截取"));

    connect(mActionResetPos,SIGNAL(triggered()),this,SLOT(resetPos()));
    connect(mActionSave,SIGNAL(triggered()),this,SLOT(save()));
    connect(mActionSelect,SIGNAL(triggered()),this,SLOT(select()));
}

void ImageWidget::save()
{
    if(is_image_load)
    {
        QString filename = QFileDialog::getSaveFileName(this, tr("Open File"),
                                                        "C:/",
                                                        tr("Images (*.png *.xpm *.jpg *.tiff *.bmp)"));
        mp_img->save(filename);
    }
}

void ImageWidget::is_select_mode_exit()
{
    is_select_mode = false;
//    qDebug() << is_select_mode;
}
