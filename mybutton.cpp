#include "mybutton.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QBitmap>

MyButton::MyButton(QWidget *parent):
    QPushButton(parent)
{
    QPixmap pixmap(":/images/play_1.png");

    resize(pixmap.size());
//    resize(100,100);
    /* 设置按钮的有效区域 */
    setMask(QBitmap(pixmap.mask()));
    setStyleSheet("QPushButton{border-image: url(:/images/play_1.png);}");
}

void MyButton::enterEvent(QEvent *)
{
    setStyleSheet("QPushButton{border-image: url(:/images/play_2.png);}");
}

void MyButton::leaveEvent(QEvent *)
{
    setStyleSheet("QPushButton{border-image: url(:/images/play_1.png);}");
}
//开始播放按钮
PlayButton::PlayButton(QWidget *parent):
    QPushButton(parent)
{
    QPixmap pixmap(":/images/play_1.png");

    resize(pixmap.size());
//    resize(100,100);
    /* 设置按钮的有效区域 */
    setMask(QBitmap(pixmap.mask()));
    setStyleSheet("QPushButton{border-image: url(:/images/play_1.png);}");
}

void PlayButton::enterEvent(QEvent *)
{
    setStyleSheet("QPushButton{border-image: url(:/images/play_2.png);}");
}

void PlayButton::leaveEvent(QEvent *)
{
    setStyleSheet("QPushButton{border-image: url(:/images/play_1.png);}");
}
//暂停播放按钮
PauseButton::PauseButton(QWidget *parent):
    QPushButton(parent)
{
    QPixmap pixmap(":/images/pause_1.png");

    resize(pixmap.size());
//    resize(100,100);
    /* 设置按钮的有效区域 */
    setMask(QBitmap(pixmap.mask()));
    setStyleSheet("QPushButton{border-image: url(:/images/pause_1.png);}");
}

void PauseButton::enterEvent(QEvent *)
{
    setStyleSheet("QPushButton{border-image: url(:/images/pause_2.png);}");
}

void PauseButton::leaveEvent(QEvent *)
{
    setStyleSheet("QPushButton{border-image: url(:/images/pause_1.png);}");
}
//快进按钮
ForwardButton::ForwardButton(QWidget *parent):
    QPushButton(parent)
{
    QPixmap pixmap(":/images/play_1.png");

    resize(pixmap.size());
//    resize(100,100);
    /* 设置按钮的有效区域 */
    setMask(QBitmap(pixmap.mask()));
    setStyleSheet("QPushButton{border-image: url(:/images/play_1.png);}");
}

void ForwardButton::enterEvent(QEvent *)
{
    setStyleSheet("QPushButton{border-image: url(:/images/play_2.png);}");
}

void ForwardButton::leaveEvent(QEvent *)
{
    setStyleSheet("QPushButton{border-image: url(:/images/play_1.png);}");
}
//快退按钮
BackButton::BackButton(QWidget *parent):
    QPushButton(parent)
{
    QPixmap pixmap(":/images/play_1.png");

    resize(pixmap.size());
//    resize(100,100);
    /* 设置按钮的有效区域 */
    setMask(QBitmap(pixmap.mask()));
    setStyleSheet("QPushButton{border-image: url(:/images/play_1.png);}");
}

void BackButton::enterEvent(QEvent *)
{
    setStyleSheet("QPushButton{border-image: url(:/images/play_2.png);}");
}

void BackButton::leaveEvent(QEvent *)
{
    setStyleSheet("QPushButton{border-image: url(:/images/play_1.png);}");
}
//下一首播放按钮
NextButton::NextButton(QWidget *parent):
    QPushButton(parent)
{
    QPixmap pixmap(":/images/next_1.png");

    resize(pixmap.size());
//    resize(100,100);
    /* 设置按钮的有效区域 */
    setMask(QBitmap(pixmap.mask()));
    setStyleSheet("QPushButton{border-image: url(:/images/next_1.png);}");
}

void NextButton::enterEvent(QEvent *)
{
    setStyleSheet("QPushButton{border-image: url(:/images/next_2.png);}");
}

void NextButton::leaveEvent(QEvent *)
{
    setStyleSheet("QPushButton{border-image: url(:/images/next_1.png);}");
}
//上一首播放按钮
LastButton::LastButton(QWidget *parent):
    QPushButton(parent)
{
    QPixmap pixmap(":/images/last_1.png");

    resize(pixmap.size());
//    resize(100,100);
    /* 设置按钮的有效区域 */
    setMask(QBitmap(pixmap.mask()));
    setStyleSheet("QPushButton{border-image: url(:/images/last_1.png);}");
}

void LastButton::enterEvent(QEvent *)
{
    setStyleSheet("QPushButton{border-image: url(:/images/last_2.png);}");
}

void LastButton::leaveEvent(QEvent *)
{
    setStyleSheet("QPushButton{border-image: url(:/images/last_1.png);}");
}
