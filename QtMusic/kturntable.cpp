#include "kturnTable.h"
#include <qpainter.h>
#include <qdebug.h>
#include <qpushbutton.h>


KturnTable::KturnTable(QWidget *parent)
    : QWidget(parent)
{
	this->resize(800,500);
    m_pTimer = new QTimer();
    (void)connect(m_pTimer, &QTimer::timeout, this, &KturnTable::upDateRote);
}

KturnTable::~KturnTable()
{
}

void KturnTable::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath painterPath;

    painterPath.addEllipse(50,50,300,300);
    painter.setClipPath(painterPath);
    painter.drawPath(painterPath);

    QPixmap pixMap;
    if (!m_img)
    {
		/* QString pathname = QDir::currentPath() + "/image/turntable.webp";
		 bool ok = pixMap.load(pathname);*/
		
        pixMap.load("D:\\QT\\Develop-Training\\QtMusic\\image\\turntable.webp");
    }
    else 
    {
		pixMap.loadFromData(m_imageData);
    }
    

    painter.translate(200, 200);//平移坐标原点  必须

    painter.save();
    painter.rotate(2*m_Rote);
      //将需要旋转的内容绘制在中间
    painter.drawPixmap(-150,-150,300,300,pixMap);  //旋转操作始终是围绕坐标原点进行旋转
    painter.restore();
}

void KturnTable::upDateRote()
{
    m_Rote=m_Rote+1;
    if(m_Rote==180)
    {
        m_Rote=0;
    }
    update();
}

void KturnTable::onStartorStopRote()
{
    m_bStart=!m_bStart;
    if(m_bStart)
    {
        m_pTimer->start(100);
    }
    else
    {
        m_pTimer->stop();
    }
}

void KturnTable::onStartorStopRoteOpen()
{
	m_bStart = true;
	if (m_bStart)
	{
		m_pTimer->start(100);
	}
	else
	{
		m_pTimer->stop();
	}
}


void KturnTable::setImg(QByteArray& data)
{
    m_img = true;
    m_imageData = data;
    update();
}

