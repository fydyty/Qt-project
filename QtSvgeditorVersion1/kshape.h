#ifndef __K_SHAPE_H_
#define __K_SHAPE_H_

#include <QObject>
#include <QPoint>
#include <QPaintDevice>
#include <QPainter>
#include <QtMath>
#include <kglobaldata.h>
#include <QDebug>
enum class KShapeType
{
	None = 0,
	PenShapeType,
	LineShapeType,
	CircleShapeType,
	RectShapeType,
	PentShapeType,
	HexaShapeType,
	TextShapeType,
	StarShapeType
};

class KShape : public QObject
{
	Q_OBJECT
	friend class KMainWindow;
public:
	KShape(QObject *parent = Q_NULLPTR);
	~KShape();
	
	// 在子类中重写，实现不同类型的图形的绘制
	virtual void drawShape(QPaintDevice *parent = Q_NULLPTR ,QPainter *painter = nullptr) = 0;

	virtual void move(QPoint offset);
	virtual void moveTop(QPoint pos);
	virtual void moveBottom(QPoint pos);
	virtual void moveLeft(QPoint pos);
	virtual void moveRight(QPoint pos);
	virtual void moveTopLeft(QPoint pos);
	virtual void moveTopRight(QPoint pos);
	virtual void moveBottomLeft(QPoint pos);
	virtual void moveBottomRight(QPoint pos);
	virtual void setPoints(QVector<QPoint> points);
	virtual void setText(QString &text);
	virtual KShapeType getShapeType();
	

	QPoint getStartPoint();
	QPoint getEndPoint();
	QPoint getStartPointRate();
	QPoint getEndPointRate();

	void setStartPoint(const QPoint &point);
	void setEndPoint(const QPoint &point);
	void setPenWidth(int value);
	void setPenStyle(Qt::PenStyle style);
	void setPenColor(QString &color);
	void setBrushColor(QString& color);
	static QColor toRgb(QString& color);
	void initPen();
	QPen getPen();
	QBrush getBrush();

	void drawOutLine(QPaintDevice* parent = Q_NULLPTR);
	QRect getShapeRect();
	bool isValid();
	void copy(KShape* shape);
protected:
	QPoint m_startPoint;// 起始坐标
	QPoint m_endPoint; // 结束坐标
	QPen m_pen;
	QBrush m_brush;
};

#endif
