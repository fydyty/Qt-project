#include "kpent.h"
#include <QPainter>
#include <QDebug>

KPent::KPent(QObject *parent)
	: KShape(parent)
{
}

KPent::~KPent()
{
}

void KPent::drawShape(QPaintDevice* parent, QPainter* painter)
{
	bool ok = false;
	if (painter == nullptr)
	{
		painter = new QPainter(parent);
		ok = true;
	}
	painter->setPen(m_pen);
	painter->setBrush(m_brush);
	painter->setRenderHint(QPainter::Antialiasing, true);
	// 设置五边形外矩形的起始坐标和终止坐标
	QPoint center = (getStartPointRate() + getEndPointRate()) / 2;
	float xHlaf = abs(getStartPointRate().x() - getEndPointRate().x());
	float yHlaf = abs(getStartPointRate().y() - getEndPointRate().y());
	float radius = std::min(abs(getStartPointRate().x() - getEndPointRate().x()), abs(getStartPointRate().y() - getEndPointRate().y())) / (1 + cos(M_PI / 5));

	float centerX = center.x();
	float centerY = center.y();

	if (xHlaf >= yHlaf)
	{
		centerY = getStartPointRate().y() + radius;
	}
	else 
	{
		centerX = getStartPointRate().x() + radius;
	}

	// 设置五边形的顶点坐标
	QPolygonF polygon;
	for (int i = 0; i < 5; ++i)
	{
		float angle = 2 * M_PI * i / 5;
		float x = radius * sin(angle) + centerX;
		float y = centerY - radius * cos(angle);
		polygon << QPointF(x, y);
	}

	// 绘制五边形
	painter->drawPolygon(polygon);
	if (ok)
	{
		painter->end();
	}
}

KShapeType KPent::getShapeType()
{
	return KShapeType::PentShapeType;
}
