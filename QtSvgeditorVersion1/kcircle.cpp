#include "kcircle.h"
#include <QPainter>

KCircle::KCircle(QObject *parent)
	: KShape(parent)
{
}

KCircle::~KCircle()
{
}

void KCircle::drawShape(QPaintDevice* parent, QPainter* painter)
{
	bool ok = false;
	if (painter == nullptr)
	{
		painter = new QPainter(parent);
		ok = true;
	}
	painter->setPen(m_pen);
	painter->setBrush(m_brush);
	painter->drawEllipse(getStartPointRate().x(),getStartPointRate().y(),getEndPointRate().x()-getStartPointRate().x(),getEndPointRate().y()-getStartPointRate().y()); 
	if (ok)
	{
		painter->end();
	}
}

KShapeType KCircle::getShapeType()
{
	return KShapeType::CircleShapeType;
}
