#include "ImageWidget.h"
#include <iostream>
#include <QDebug>
#include "selectrect.h"

ImageWidget::ImageWidget(QWidget *parent):QWidget(parent),scalex(1),scaley(1),xtranslate(0),ytranslate(0),mouse(No)
{
    is_image_load = false;
    is_select_mode = false;
    create_contextmenu();
}

ImageWidget::~ImageWidget()
{
    if(is_image_load)
        mp_img = NULL;
    if(is_image_cloned)
        delete mp_img;
    delete  mMenu;
}

void ImageWidget::set_image_with_data(QImage img,bool fit_widget, bool always_initialization)
{
    is_fit_widget_size = fit_widget;
    if(!is_image_cloned)
    {
        mp_img = new QImage;
        is_image_cloned = true;
    }
    *mp_img = img.copy(0,0,img.width(),img.height());
    //    qDebug() << mp_img->data_ptr() << img.data_ptr();
    is_image_load = true;
    if(always_initialization)
        set_default_parameters();
    mouse = No;
    update();
}

void ImageWidget::set_image_with_pointer(QImage *img, bool fit_widget, bool always_initialization)
{
    is_fit_widget_size = fit_widget;
    mp_img = img;
    is_image_load = true;
    if(always_initialization)
        set_default_parameters();
    mouse = No;
    update();
}

void ImageWidget::clear()
{
    if(is_image_load)
    {
        is_image_load = false;

        if(is_image_cloned)
            delete mp_img;
        else
            mp_img = NULL;
        update();
    }
}


void ImageWidget::only_show_image(bool flag)
{
    is_only_show_image = flag;
}


void ImageWidget::wheelEvent(QWheelEvent *e)
{
    if(is_image_load && !is_select_mode && !is_only_show_image)
    {
        int numDegrees = e->delta();
        if(numDegrees > 0)
        {
            zoom_out();
        }
        if(numDegrees < 0)
        {
            zoom_in();
        }
        update();
    }
}
void ImageWidget::mousePressEvent(QMouseEvent *e)
{
    if(is_image_load && !is_only_show_image)
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
    if(is_fit_widget_size)
        painter.drawImage(QPoint(xtranslate,ytranslate),mp_img->scaled(this->width()*scalex,this->height()*scaley,Qt::KeepAspectRatio));
    else
        painter.drawImage(QPoint(xtranslate,ytranslate),mp_img->scaled(mp_img->width()*scalex,mp_img->height()*scaley,Qt::KeepAspectRatio));
    //    qDebug() << scalex;
}

void ImageWidget::contextMenuEvent(QContextMenuEvent *e)
{
    if(!is_only_show_image)
        mMenu->exec(QCursor::pos());
    //mousePosX = QCursor::pos().x();
    //mousePosY = QCursor::pos().y();
}

void ImageWidget::resizeEvent(QResizeEvent *event)
{
    if(is_select_mode && is_image_load)
        emit parent_widget_size_changed(this->width(),this->height());
}

void ImageWidget::reset_image()
{
    xtranslate = 0;
    ytranslate = 0;
    scalex = 1;
    scaley = 1;
    last_x_pos = 0;
    last_y_pos = 0;
    update();
}

void ImageWidget::zoom_out()
{
    if(scalex<=12&&scaley<=12)
    {
        scalex *= 1.1;
        scaley *= 1.1;
    }
    update();
}

void ImageWidget::zoom_in()
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
        connect(this,SIGNAL(parent_widget_size_changed(int,int)),m,SLOT(receive_parent_size_changed_value(int,int)));
        m->set_image(mp_img);
        m->scalex = scalex;
        m->scaley = scaley;
        m->xtranslate = xtranslate;
        m->ytranslate = ytranslate;
        m->show();
    }
}


void ImageWidget::create_contextmenu()
{
    mMenu = new QMenu();
    mActionResetPos = mMenu->addAction(tr("重置"));
    mActionSave		= mMenu->addAction(tr("另存为"));
    mActionSelect   = mMenu->addAction(tr("截取"));

    connect(mActionResetPos,SIGNAL(triggered()),this,SLOT(reset_image()));
    connect(mActionSave,SIGNAL(triggered()),this,SLOT(save()));
    connect(mActionSelect,SIGNAL(triggered()),this,SLOT(select()));
}

void ImageWidget::save()
{
    if(is_image_load)
    {
        QImage temp = mp_img->copy();
        QString filename = QFileDialog::getSaveFileName(this, tr("Open File"),
                                                        "C:/",
                                                        tr("Images (*.png *.xpm *.jpg *.tiff *.bmp)"));
         temp.save(filename);
    }
}

void ImageWidget::is_select_mode_exit()
{
    is_select_mode = false;
    //    qDebug() << is_select_mode;
}

void ImageWidget::set_default_parameters()
{
    scalex = scaley = 1.0;
    xtranslate = ytranslate = 0;
    last_x_pos = last_y_pos = 0;
}
