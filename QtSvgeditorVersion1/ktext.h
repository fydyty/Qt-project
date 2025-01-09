#ifndef __K_TEXT_H_
#define __K_TEXT_H_

#include "kshape.h"

class KText : public KShape
{
	Q_OBJECT

public:
	KText(QObject *parent = Q_NULLPTR);
	~KText();
	virtual void drawShape(QPaintDevice* parent = Q_NULLPTR, QPainter* painter = nullptr) override;
	virtual KShapeType getShapeType();
	virtual void setText(QString &text) override;
private:
	QString m_text;
};

#endif
