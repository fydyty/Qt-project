#include "kshape.h"

KShape::KShape(QObject *parent)
	: QObject(parent)
{
	m_pen.setColor(Qt::black);
	m_pen.setWidth(1);
	m_brush.setStyle(Qt::SolidPattern);
}

KShape::~KShape()
{
}

void KShape::move(QPoint offset)
{
	m_startPoint += offset;
	m_endPoint += offset;
}

void KShape::moveTop(QPoint pos)
{
	m_startPoint.ry() = pos.y();
}

void KShape::moveBottom(QPoint pos)
{
	m_endPoint.ry() = pos.y();
}

void KShape::moveLeft(QPoint pos)
{
	m_startPoint.rx() = pos.x();
}

void KShape::moveRight(QPoint pos)
{
	m_endPoint.rx() = pos.x();
}

void KShape::moveTopLeft(QPoint pos)
{
	m_startPoint = pos;
}

void KShape::moveTopRight(QPoint pos)
{
	m_startPoint.ry() = pos.y();
	m_endPoint.rx() = pos.x();
}

void KShape::moveBottomLeft(QPoint pos)
{
	m_startPoint.rx() = pos.x();
	m_endPoint.ry() = pos.y();
}

void KShape::moveBottomRight(QPoint pos)
{
	m_endPoint = pos;
}

void KShape::setPoints(QVector<QPoint> points)
{
}

void KShape::setText(QString& text)
{
}

// 绘制选中矩形框
void KShape::drawOutLine(QPaintDevice* parent)
{
	QPainter painter(parent);
	QPen pen;

	pen.setColor(QColor(79, 128, 255));
	pen.setWidth(3);
	painter.setPen(pen);

	QPoint start = getStartPoint();
	QPoint end = getEndPoint();

	painter.drawRect(QRect(getStartPointRate(), getEndPointRate()));
}

QPoint KShape::getStartPoint()
{
	return m_startPoint;
}

QPoint KShape::getEndPoint()
{
	return m_endPoint;
}

QPoint KShape::getStartPointRate()
{
	return m_startPoint * KGlobalData::getGlobalDataIntance()->getCanvasScale();
}

QPoint KShape::getEndPointRate()
{
	return m_endPoint * KGlobalData::getGlobalDataIntance()->getCanvasScale();
}

void KShape::setStartPoint(const QPoint & point)
{
	m_startPoint = point;
}

void KShape::setEndPoint(const QPoint& point)
{
	m_endPoint = point;
}

void KShape::setPenWidth(int value)
{
	m_pen.setWidth(value);
}

void KShape::setPenStyle(Qt::PenStyle style)
{
	m_pen.setStyle(style);
}

void KShape::setPenColor(QString& color)
{
	m_pen.setColor(toRgb(color));
}

void KShape::setBrushColor(QString& color)
{
	m_brush.setColor(toRgb(color));
}

QColor KShape::toRgb(QString& color)
{
	std::string hex = color.QString::toStdString();
	int r = std::stoi(hex.substr(2, 2), nullptr, 16);
	int g = std::stoi(hex.substr(4, 2), nullptr, 16);
	int b = std::stoi(hex.substr(6, 2), nullptr, 16);
	return QColor(r,g,b);
}

void KShape::initPen()
{
	m_pen.setWidth(KGlobalData::getGlobalDataIntance()->getShapePenWidth());
	m_pen.setStyle(KGlobalData::getGlobalDataIntance()->getShapePenStyle());
	m_pen.setColor(toRgb(KGlobalData::getGlobalDataIntance()->getPenColor()));
	m_brush.setColor(toRgb(KGlobalData::getGlobalDataIntance()->getBrushColor()));
}

QPen KShape::getPen()
{
	return m_pen;
}

QBrush KShape::getBrush()
{
	return m_brush;
}

KShapeType KShape::getShapeType()
{
	return KShapeType();
}

QRect KShape::getShapeRect()
{

	return QRect(getStartPointRate() , getEndPointRate());
}


bool KShape::isValid()
{
	return (m_startPoint.x() != m_endPoint.x() || m_startPoint.y() != m_endPoint.y());
}

void KShape::copy(KShape* shape)
{
	m_pen = shape->getPen();
	m_brush = shape->getBrush();
	m_startPoint = shape->getStartPoint() + QPoint(20, 20);
	m_endPoint = shape->getEndPoint() + QPoint(20, 20);
}
