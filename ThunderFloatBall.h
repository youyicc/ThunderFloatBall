#ifndef THUNDERFLOATBALL_H
#define THUNDERFLOATBALL_H

#include <QRect>
#include <QLabel>
#include <QtMath>
#include <QDialog>
#include <QScreen>
#include <QPainter>
#include <QPaintEvent>
#include <QTimerEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QGuiApplication>

class ThunderFloatBall : public QDialog
{
    Q_OBJECT


private:
    QPoint last;
    int m_value;
    double m_offset;
    QRect geometry;

private:
    QLabel* lab_Value;

public:
    ThunderFloatBall(QWidget *parent = nullptr);
    ~ThunderFloatBall();

private:
    void CalcWayLinesPath(int&,int&,QPainterPath&,QPainterPath&);

protected:
    void paintEvent(QPaintEvent*);
    void timerEvent(QTimerEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);


};
#endif
