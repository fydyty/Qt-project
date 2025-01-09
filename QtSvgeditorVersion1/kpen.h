#ifndef __K_PEN_H_
#define __K_PEN_H_

#include "kshape.h"

class KPen : public KShape
{
	Q_OBJECT

public:
	KPen(QObject *parent = Q_NULLPTR);
	~KPen();
	virtual void drawShape(QPaintDevice* parent = Q_NULLPTR, QPainter* painter = nullptr) override;
	virtual KShapeType getShapeType();
	virtual void setPoints(QVector<QPoint> points) override;
	virtual void move(QPoint offset) override;
private:
	QVector<QPoint> m_points;
};

#endif
