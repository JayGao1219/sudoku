#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QObject>

class MyLabel:public QLabel
{
    Q_OBJECT
public:
    MyLabel(QWidget *parent=0);
private:
protected:
    virtual void mouseReleaseEvent(QMouseEvent *ev);
signals:
    void clicked();
};

#endif // MYLABEL_H
