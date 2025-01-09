#include "ktext.h"
#include <QPainter>
#include <QDebug>
KText::KText(QObject *parent)
	: KShape(parent)
{
}

KText::~KText()
{
}

void KText::drawShape(QPaintDevice* parent, QPainter* painter)
{
	bool ok = false;
	if (painter == nullptr)
	{
		painter = new QPainter(parent);
		ok = true;
	}
	painter->setPen(m_pen);
	painter->setBrush(m_brush);
	QFont font;
	font.setPointSize(int(12 * KGlobalData::getGlobalDataIntance()->getCanvasScale())); // 设置字号大小为20
	painter->setFont(font);
	painter->drawText(getStartPointRate(), m_text);
	if (ok)
	{
		painter->end();
	}
}

KShapeType KText::getShapeType()
{
	return KShapeType::LineShapeType;
}

void KText::setText(QString& text)
{
	m_text = text;
}
