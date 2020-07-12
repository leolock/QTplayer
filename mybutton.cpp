#include "mybutton.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QBitmap>

MyButton::MyButton(QWidget *parent):
    QPushButton(parent)
{
    QPixmap pixmap(":/images/play--filled.png");

    resize(pixmap.size());
//    resize(100,100);
    /* 设置按钮的有效区域 */
    setMask(QBitmap(pixmap.mask()));
    setStyleSheet("QPushButton{border-image: url(:/images/play--filled.png);}");
}

void MyButton::enterEvent(QEvent *)
{
    setStyleSheet("QPushButton{border-image: url(:/images/start.png);}");
}

void MyButton::leaveEvent(QEvent *)
{
    setStyleSheet("QPushButton{border-image: url(:/images/play--filled.png);}");
}
