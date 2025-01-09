#include "kpen.h"
#include <QPainter>

KPen::KPen(QObject *parent)
	: KShape(parent)
{
}

KPen::~KPen()
{
}

void KPen::drawShape(QPaintDevice* parent, QPainter* painter)
{
	bool ok = false;
	if (painter == nullptr)
	{
		painter = new QPainter(parent);
		ok = true;
	}
	painter->setPen(m_pen);
	painter->setBrush(m_brush);
	for (int i = 0; i < m_points.size() - 1; i++)
	{
		painter->drawLine(m_points.at(i), m_points.at(i + 1));
	}
	if (ok)
	{
		painter->end();
	}
}

KShapeType KPen::getShapeType()
{
	return KShapeType::LineShapeType;
}

void KPen::setPoints(QVector<QPoint> points)
{
	m_points = points;
}

void KPen::move(QPoint offset)
{
	setStartPoint(getStartPoint() + offset);
	setEndPoint(getEndPoint() + offset);
	for (auto &point:m_points)
	{
		point += offset;
	}
}

