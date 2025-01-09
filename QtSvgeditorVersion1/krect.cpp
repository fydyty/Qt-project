#include "krect.h"
#include <QPainter>

KRect::KRect(QObject *parent)
	: KShape(parent)
{
}

KRect::~KRect()
{
}

void KRect::drawShape(QPaintDevice* parent, QPainter* painter)
{
	bool ok = false;
	if (painter == nullptr)
	{
		painter = new QPainter(parent);
		ok = true;
	}
	painter->setPen(m_pen);
	painter->setBrush(m_brush);
	painter->drawRect(QRect(getStartPointRate(), getEndPointRate()));
	if (ok)
	{
		painter->end();
	}
}

KShapeType KRect::getShapeType()
{
	return KShapeType::RectShapeType;
}
