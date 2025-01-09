#include "kline.h"
#include <QPainter>

KLine::KLine(QObject *parent)
	: KShape(parent)
{
}

KLine::~KLine()
{
}

void KLine::drawShape(QPaintDevice* parent, QPainter* painter)
{
	bool ok = false;
	if (painter == nullptr)
	{
		painter = new QPainter(parent);
		ok = true;
	}
	painter->setPen(m_pen);
	painter->setBrush(m_brush);
	painter->drawLine(QLine(getStartPointRate(),getEndPointRate())); 
	if (ok)
	{
		painter->end();
	}
}

KShapeType KLine::getShapeType()
{
	return KShapeType::LineShapeType;
}
