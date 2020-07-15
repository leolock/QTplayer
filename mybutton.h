#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>
#include <QWidget>

class MyButton : public QPushButton
{
    Q_OBJECT

public:
    MyButton(QWidget *);
protected:
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);

};

class PlayButton : public QPushButton
{
    Q_OBJECT

public:
    PlayButton(QWidget *);
protected:
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);

};

class PauseButton : public QPushButton
{
    Q_OBJECT

public:
    PauseButton(QWidget *);
protected:
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);

};

class StopButton : public QPushButton
{
    Q_OBJECT

public:
    StopButton(QWidget *);
protected:
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);

};

class ForwardButton : public QPushButton
{
    Q_OBJECT

public:
    ForwardButton(QWidget *);
protected:
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);

};

class BackButton : public QPushButton
{
    Q_OBJECT

public:
    BackButton(QWidget *);
protected:
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);

};

class NextButton : public QPushButton
{
    Q_OBJECT

public:
    NextButton(QWidget *);
protected:
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);

};

class LastButton : public QPushButton
{
    Q_OBJECT

public:
    LastButton(QWidget *);
protected:
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);

};

class SoundButton : public QPushButton
{
    Q_OBJECT

public:
    SoundButton(QWidget *);

};
class MuteButton : public QPushButton
{
    Q_OBJECT

public:
    MuteButton(QWidget *);

};
#endif // MYBUTTON_H
