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

    void setImage(QImage* img, bool is_default = true)
	{
        mp_img = img;
        is_image_load = true;
        if(is_default)
        {
            scalex = scaley = 1.0;
            xtranslate = ytranslate = 0;
            last_x_pos = last_y_pos = 0;
        }
        mouse = No;
		update();
	}

	void clear()
	{
        if(is_image_load)
		{
            is_image_load = false;
			//delete mp_img;
            mp_img = NULL;
            update();
		}
	}
	
	void iniActions();	
	void wheelEvent(QWheelEvent *e);
	void mouseMoveEvent(QMouseEvent * e);
	void mousePressEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
    void contextMenuEvent(QContextMenuEvent *e);


public slots:
	void resetPos();
	void zoomout();
	void zoomin();
	void translate(int x,int y);
	void save();
    void select();
signals:


private slots:
    void is_select_mode_exit();

private:

    QImage *mp_img;
    double scalex;
    double scaley;
	int xtranslate;
	int ytranslate;
    int last_x_pos = 0;
    int last_y_pos = 0;

	int mousePosX;
	int mousePosY;
    bool is_image_load;
    bool is_select_mode;

	MouseDown mouse;
	
	QAction* mActionResetPos;
	QAction* mActionSave;
    QAction* mActionSelect;

	QAction* conditionDialation;
	QMenu* mMenu;

};
