#pragma once
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QWheelEvent>
#include <QMenu>
#include <QCursor>
#include <QFileDialog>
#include <QSplitter>
#include <QMessageBox>
#include <QDebug>
#include "selectrect.h"

enum MouseDown{Left,Mid,Right,No};

class ImageWidget :
	public QWidget
{
	Q_OBJECT
public:
    ImageWidget(QWidget *parent);
	~ImageWidget();

    void set_image_with_data(QImage img, bool fit_widget = true, bool always_initialization = false);
    void set_image_with_pointer(QImage* img , bool fit_widget = true, bool always_initialization = false);
    void clear();
    void only_show_image(bool flag = false);

signals:
    void parent_widget_size_changed(int width, int height);

public slots:

private slots:
    void reset_image();
    void save();
    void select();
    void is_select_mode_exit();

private:
    void set_default_parameters();
    void zoom_out();
    void zoom_in();
    void translate(int x,int y);

    void create_contextmenu();
    void wheelEvent(QWheelEvent *e);
    void mouseMoveEvent(QMouseEvent * e);
    void mousePressEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
    void contextMenuEvent(QContextMenuEvent *e);
    void resizeEvent(QResizeEvent *event);

    QImage *mp_img;
    double scalex;
    double scaley;
	int xtranslate;
	int ytranslate;
    int last_x_pos = 0;
    int last_y_pos = 0;

	int mousePosX;
	int mousePosY;
    bool is_image_load = false;
    bool is_select_mode = false;
    bool is_only_show_image = false;
    bool is_image_cloned = false;
    bool is_fit_widget_size = true;

	MouseDown mouse;
	
	QAction* mActionResetPos;
	QAction* mActionSave;
    QAction* mActionSelect;

	QMenu* mMenu;

};
