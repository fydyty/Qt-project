#include "kshapefactory.h"

KShapeFactory::KShapeFactory(QObject *parent)
	: QObject(parent)
{
}

KShapeFactory::~KShapeFactory()
{
}

KShape* KShapeFactory::createShape(KGlobalData::KDrawFlag drawFlag)
{
	switch(drawFlag)
	{
	case KGlobalData::KDrawFlag::RectDrawFlag:
		return new KRect;
	case KGlobalData::KDrawFlag::LineDrawFlag:
		return new KLine;
	case KGlobalData::KDrawFlag::CircleDrawFlag:
		return new KCircle;
	case KGlobalData::KDrawFlag::PenDrawFlag:
		return new KPen;
	case KGlobalData::KDrawFlag::TextDrawFlag:
		return new KText;
	case KGlobalData::KDrawFlag::PentDrawFlag:
		return new KPent;
	case KGlobalData::KDrawFlag::HexaDrawFlag:
		return new KHexa;
	case KGlobalData::KDrawFlag::StarDrawFlag:
		return new KStar;
	default:
		break;
	}
}

KShape* KShapeFactory::createShape(KShapeType shapeType)
{
	switch (shapeType)
	{
	case KShapeType::RectShapeType:
		return new KRect;
	case KShapeType::LineShapeType:
		return new KLine;
	case KShapeType::CircleShapeType:
		return new KCircle;
	case KShapeType::PenShapeType:
		return new KPen;
	case KShapeType::TextShapeType:
		return new KText;
	case KShapeType::PentShapeType:
		return new KPent;
	case KShapeType::HexaShapeType:
		return new KHexa;
	case KShapeType::StarShapeType:
		return new KStar;
	default:
		break;
	}
}


