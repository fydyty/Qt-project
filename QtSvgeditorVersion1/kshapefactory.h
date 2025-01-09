#ifndef __K_SHAPEFACTORY_H_
#define __K_SHAPEFACTORY_H_

#include <QObject>

#include "kglobaldata.h"
#include "krect.h"
#include "kline.h"
#include "kcircle.h"
#include "kpen.h"
#include "ktext.h"
#include "kpent.h"
#include "khexa.h"
#include "kstar.h"

class KShapeFactory : public QObject
{
	Q_OBJECT
	friend class KShape;
public:
	KShapeFactory(QObject *parent);
	~KShapeFactory();
	static KShape* createShape(KGlobalData::KDrawFlag drawFlag);
	static KShape* createShape(KShapeType shapeType);
};

#endif
