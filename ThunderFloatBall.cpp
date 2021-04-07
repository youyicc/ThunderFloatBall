#include "ThunderFloatBall.h"

ThunderFloatBall::ThunderFloatBall(QWidget *parent)
    : QDialog(parent)
{
    m_value=80;
    m_offset=0;
    //设置一个定时器
    startTimer(150);
    setFixedSize(70,70);
    //去掉标题栏 设置置顶 去掉任务栏图标
    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool|Qt::WindowCloseButtonHint|Qt::WindowStaysOnTopHint);

    //获取主屏尺寸 设置位置
    QScreen *primaryScreen=QGuiApplication::primaryScreen();
    if(primaryScreen==nullptr)
        return;
    geometry=primaryScreen->availableGeometry();
    this->move(geometry.width()-100,geometry.height()*0.2);

    //设置窗口背景透明
    setAttribute(Qt::WA_TranslucentBackground, true);

    try {
        //初始化控件
        lab_Value=new QLabel();
        QLabel *lab_Unit=new QLabel();
        QLabel *lab_NetSpeed=new QLabel();
        QHBoxLayout* valueLayout=new QHBoxLayout();
        QVBoxLayout* mainLayout=new QVBoxLayout();
        //设置控件样式
        lab_Value->setFont(QFont("宋体",18));
        //设置控件文字
        lab_Value->setText("10");
        lab_Unit->setText("MB/s");
        lab_NetSpeed->setText("+ 0B/s");
        //设置控件文字对齐方式
        lab_Value->setAlignment(Qt::AlignHCenter|Qt::AlignBottom);
        lab_Unit->setAlignment(Qt::AlignHCenter|Qt::AlignBottom);
        lab_NetSpeed->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        lab_Value->setFixedHeight(40);
        lab_Unit->setFixedHeight(36);
        //设置布局
        valueLayout->setMargin(0);
        valueLayout->setSpacing(2);
        valueLayout->addStretch(1);
        valueLayout->addWidget(lab_Value);
        valueLayout->addWidget(lab_Unit);
        valueLayout->addStretch(1);
        mainLayout->setMargin(2);
        mainLayout->setSpacing(0);
        mainLayout->addLayout(valueLayout,3);
        mainLayout->addWidget(lab_NetSpeed,2);
        setLayout(mainLayout);
    } catch (...) {}
}

ThunderFloatBall::~ThunderFloatBall(){}

 void ThunderFloatBall::timerEvent(QTimerEvent*)
 {
    update();
 }

void ThunderFloatBall::mousePressEvent(QMouseEvent *e)
{
    last = e->globalPos();
}

void ThunderFloatBall::mouseMoveEvent(QMouseEvent *e)
{
    if((last.x()==0)&&(last.y()==0))
        return;
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    this->move(x()+dx, y()+dy);
}

void ThunderFloatBall::mouseReleaseEvent(QMouseEvent *e)
{
    if((last.x()==0)&&(last.y()==0))
        return;
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    this->move(x() + dx, y() + dy);
}

void ThunderFloatBall::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);  // 反锯齿;
    int height = this->height();
    int width = this->width();
    int roundWidth = qMin(width, height)-2;

    //波浪线路径集合
    QPainterPath wavyPath1;
    QPainterPath wavyPath2;
    //计算波浪线所在的路径集合
    CalcWayLinesPath(width,height,wavyPath1,wavyPath2);

    //圆路径集合
    QPainterPath roundPath;
    roundPath.addEllipse(1, 1, roundWidth, roundWidth);

    //绘制一个填充圆
    painter.setPen(QColor(230,230,230,200));
    painter.setBrush(QColor(255,255,255));
    painter.drawEllipse(1, 1, roundWidth, roundWidth);

    //用波浪线和填充圆的交集绘制进度
    QPainterPath intersectedPath;
    painter.setPen(Qt::NoPen);
    //第一条波浪与圆的交集路径集合并绘制路径
    intersectedPath=roundPath.intersected(wavyPath1);
    painter.setBrush(QColor(185,187,255,100));
    painter.drawPath(intersectedPath);

    //第二条波浪与圆的交集路径集合并绘制路径
    intersectedPath=roundPath.intersected(wavyPath2);
    painter.setBrush(QColor(107,150,255,150));
    painter.drawPath(intersectedPath);
}

void ThunderFloatBall::CalcWayLinesPath(int &width,int &height,QPainterPath& wavyPath1,QPainterPath& wavyPath2)
{
    //计算偏移值
    m_offset+=0.5;
    if (m_offset > (width)) {
        m_offset = 0;
    }

    //计算当前值在进度条中高度
    int currentHeight = (1-(m_value)/100.0)*height;

    //计算圆中有多少个完整周期 这里为1.5个周期
    double roundCycle = 3 * M_PI /width;

    //移动到左下角起始点
    wavyPath1.moveTo(0, height);
    wavyPath2.moveTo(0, height);
    for(int i=0;i<=width;++i) {
        //第一条波浪Y轴
        double wavyY1 = (double)(2*qSin(roundCycle*i+m_offset)) + currentHeight;
        //第二条波浪Y轴
        double wavyY2 = -(double)(2*qSin(roundCycle*i+m_offset)) + currentHeight;
        if (m_value == 0) {
            wavyY1 = height;
            wavyY2 = height;
        }
        if (m_value == 100) {
            wavyY1 = 0;
            wavyY2 = 0;
        }
        //添加点在路径中
        wavyPath1.lineTo(i, wavyY1);
        wavyPath2.lineTo(i, wavyY2);
    }
    //移动到右下角结束点 形成一个闭合路径 后面才能取交集
    wavyPath1.lineTo(width, height);
    wavyPath2.lineTo(width, height);
}
