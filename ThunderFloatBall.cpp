#include "ThunderFloatBall.h"

ThunderFloatBall::ThunderFloatBall(QWidget *parent)
    : QDialog(parent)
{
    m_value=80;
    m_offset=0;
    //����һ����ʱ��
    startTimer(150);
    setFixedSize(70,70);
    //ȥ�������� �����ö� ȥ��������ͼ��
    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool|Qt::WindowCloseButtonHint|Qt::WindowStaysOnTopHint);

    //��ȡ�����ߴ� ����λ��
    QScreen *primaryScreen=QGuiApplication::primaryScreen();
    if(primaryScreen==nullptr)
        return;
    geometry=primaryScreen->availableGeometry();
    this->move(geometry.width()-100,geometry.height()*0.2);

    //���ô��ڱ���͸��
    setAttribute(Qt::WA_TranslucentBackground, true);

    try {
        //��ʼ���ؼ�
        lab_Value=new QLabel();
        QLabel *lab_Unit=new QLabel();
        QLabel *lab_NetSpeed=new QLabel();
        QHBoxLayout* valueLayout=new QHBoxLayout();
        QVBoxLayout* mainLayout=new QVBoxLayout();
        //���ÿؼ���ʽ
        lab_Value->setFont(QFont("����",18));
        //���ÿؼ�����
        lab_Value->setText("10");
        lab_Unit->setText("MB/s");
        lab_NetSpeed->setText("+ 0B/s");
        //���ÿؼ����ֶ��뷽ʽ
        lab_Value->setAlignment(Qt::AlignHCenter|Qt::AlignBottom);
        lab_Unit->setAlignment(Qt::AlignHCenter|Qt::AlignBottom);
        lab_NetSpeed->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        lab_Value->setFixedHeight(40);
        lab_Unit->setFixedHeight(36);
        //���ò���
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
    painter.setRenderHint(QPainter::Antialiasing,true);  // �����;
    int height = this->height();
    int width = this->width();
    int roundWidth = qMin(width, height)-2;

    //������·������
    QPainterPath wavyPath1;
    QPainterPath wavyPath2;
    //���㲨�������ڵ�·������
    CalcWayLinesPath(width,height,wavyPath1,wavyPath2);

    //Բ·������
    QPainterPath roundPath;
    roundPath.addEllipse(1, 1, roundWidth, roundWidth);

    //����һ�����Բ
    painter.setPen(QColor(230,230,230,200));
    painter.setBrush(QColor(255,255,255));
    painter.drawEllipse(1, 1, roundWidth, roundWidth);

    //�ò����ߺ����Բ�Ľ������ƽ���
    QPainterPath intersectedPath;
    painter.setPen(Qt::NoPen);
    //��һ��������Բ�Ľ���·�����ϲ�����·��
    intersectedPath=roundPath.intersected(wavyPath1);
    painter.setBrush(QColor(185,187,255,100));
    painter.drawPath(intersectedPath);

    //�ڶ���������Բ�Ľ���·�����ϲ�����·��
    intersectedPath=roundPath.intersected(wavyPath2);
    painter.setBrush(QColor(107,150,255,150));
    painter.drawPath(intersectedPath);
}

void ThunderFloatBall::CalcWayLinesPath(int &width,int &height,QPainterPath& wavyPath1,QPainterPath& wavyPath2)
{
    //����ƫ��ֵ
    m_offset+=0.5;
    if (m_offset > (width)) {
        m_offset = 0;
    }

    //���㵱ǰֵ�ڽ������и߶�
    int currentHeight = (1-(m_value)/100.0)*height;

    //����Բ���ж��ٸ��������� ����Ϊ1.5������
    double roundCycle = 3 * M_PI /width;

    //�ƶ������½���ʼ��
    wavyPath1.moveTo(0, height);
    wavyPath2.moveTo(0, height);
    for(int i=0;i<=width;++i) {
        //��һ������Y��
        double wavyY1 = (double)(2*qSin(roundCycle*i+m_offset)) + currentHeight;
        //�ڶ�������Y��
        double wavyY2 = -(double)(2*qSin(roundCycle*i+m_offset)) + currentHeight;
        if (m_value == 0) {
            wavyY1 = height;
            wavyY2 = height;
        }
        if (m_value == 100) {
            wavyY1 = 0;
            wavyY2 = 0;
        }
        //��ӵ���·����
        wavyPath1.lineTo(i, wavyY1);
        wavyPath2.lineTo(i, wavyY2);
    }
    //�ƶ������½ǽ����� �γ�һ���պ�·�� �������ȡ����
    wavyPath1.lineTo(width, height);
    wavyPath2.lineTo(width, height);
}
