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
#endif // MYBUTTON_H
