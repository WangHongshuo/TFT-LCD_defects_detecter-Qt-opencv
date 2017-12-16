#ifndef SELECTRECT_H
#define SELECTRECT_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QWheelEvent>
#include <QMenu>
#include <QCursor>
#include <QFileDialog>
#include <QSplitter>
#include "ImageWidget.h"


typedef struct rect_info
{
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
}rect_info;
Q_DECLARE_METATYPE(rect_info)

class SelectRect : public QWidget
{
    Q_OBJECT
public:

    SelectRect(QWidget *parent);
    ~SelectRect();
    void set_image(QImage* img)
    {
        *image = img->copy();
        is_image_load = true;
    }


    double scalex;
    double scaley;
    int xtranslate;
    int ytranslate;

protected:


signals:
    void select_mode_exit();
public slots:


private slots:
    void receive_parent_size_changed_value(int width, int height);
    void select_exit();
    void select_reset();
    void cut_img();


private:

    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);

    QMenu* subMenu;
    QAction* subActionReset;
    QAction* subActionSave;
    QAction* subActionExit;
    rect_info rect;
    int first_mouse_pos_x;
    int first_mouse_pos_y;
    int mouse;
    QImage* image;
    bool is_image_load;

};

#endif // SELECTRECT_H
