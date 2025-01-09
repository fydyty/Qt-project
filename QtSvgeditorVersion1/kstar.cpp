#include "kstar.h"
#include <QPainter>

KStar::KStar(QObject *parent)
	: KShape(parent)
{
}

KStar::~KStar()
{
}

void KStar::drawShape(QPaintDevice* parent, QPainter* painter)
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
	
	QPoint center = (getStartPointRate() + getEndPointRate()) / 2;
	float xHlaf = abs(getStartPointRate().x() - getEndPointRate().x());
	float yHlaf = abs(getStartPointRate().y() - getEndPointRate().y());
	float radius = std::min(abs(getStartPointRate().x() - getEndPointRate().x()), abs(getStartPointRate().y() - getEndPointRate().y())) / (1 + cos(M_PI / 5));
	float radiusIn = radius * sin(M_PI / 5) - radius * cos(M_PI / 5) * tan(M_PI / 10);

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

	QPointF points[10];
	double angleStep = M_PI/5; // 每个顶点的角度步长
	for (int i = 0; i < 10; ++i) {
		double angle = i * angleStep;
		double x;
		double y;
		if (i % 2 == 0)
		{
			x = radius * sin(angle);
			y = radius * cos(angle);
		}
		else
		{
			x = radiusIn * sin(angle);
			y = radiusIn * cos(angle);
		}
		
		points[i] = QPointF(centerX + x, centerY - y);
	}
	// 绘制星形
	painter->drawPolygon(points, 10);
	if (ok)
	{
		painter->end();
	}
}

KShapeType KStar::getShapeType()
{
	return KShapeType::StarShapeType;
}
