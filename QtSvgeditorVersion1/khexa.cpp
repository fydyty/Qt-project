#include "khexa.h"
#include <QPainter>

KHexa::KHexa(QObject *parent)
	: KShape(parent)
{
}

KHexa::~KHexa()
{
}

void KHexa::drawShape(QPaintDevice* parent, QPainter* painter)
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
	
	float centerX = center.x();
	float centerY = center.y();
	float radius = std::min(abs(getStartPointRate().x() - getEndPointRate().x()), abs(getStartPointRate().y() - getEndPointRate().y())) / sqrt(3);

	
	QPolygonF polygon;
	for (int i = 1; i <= 6; ++i)
	{
		float angle = M_PI * i / 3;
		float x = radius * cos(angle) + centerX;
		float y = centerY - radius * sin(angle);
		polygon << QPointF(x, y);
	}

	
	painter->drawPolygon(polygon);
	if (ok)
	{
		painter->end();
	}
}

KShapeType KHexa::getShapeType()
{
	return KShapeType::HexaShapeType;
}
