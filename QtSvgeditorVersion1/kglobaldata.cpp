#include "kglobaldata.h"
#include <Windows.h>


KGlobalData::KGlobalData(QObject* parent)
    : QObject(parent)
    , m_drawFlag(KDrawFlag::NoneDrawFlag)
    , m_canvasColor("FFFFFF")
    , m_penColor("FF000000")
    , m_brushColor("FFFFFFFF")
    , m_canvasWidth(600)
    , m_canvasHeight(400)
    , m_scale(1.0)
    , m_shapePenWidth(1)
    , m_penStyle(Qt::SolidLine)
{
}



KGlobalData *KGlobalData::getGlobalDataIntance()
{
    static KGlobalData s_instance;
    static KGlobalData* pinstance = nullptr;
    if (pinstance == nullptr)
        pinstance = &s_instance;
    return pinstance;
}

void KGlobalData::setDrawFlag(KGlobalData::KDrawFlag drawflag)
{
    this->m_drawFlag = drawflag;
}

KGlobalData::KDrawFlag KGlobalData::getDrawFlag()
{
    return m_drawFlag;
}

void KGlobalData::setCanvasWidth(const int width)
{
    this->m_canvasWidth = width;
}

void KGlobalData::setCanvasHeight(const int height)
{
    this->m_canvasHeight = height;
}

int KGlobalData::getCanvasWidth() const
{
    return this->m_canvasWidth;
}

int KGlobalData::getCanvasHeight() const
{
    return m_canvasHeight;
}

void KGlobalData::setShapePenWidth(const int width)
{
    m_shapePenWidth = width;
}

int KGlobalData::getShapePenWidth() const
{
    return m_shapePenWidth;
}

void KGlobalData::setShapePenStyle(const Qt::PenStyle value)
{
    m_penStyle = value;
}

Qt::PenStyle KGlobalData::getShapePenStyle() const
{
    return m_penStyle;
}

void KGlobalData::setCanvasColor(const QString& colorStr)
{
    this->m_canvasColor = colorStr;
}

QString KGlobalData::getCanvasColor()
{
    return m_canvasColor;
}

void KGlobalData::setPenColor(const QString& colorStr)
{
    m_penColor = colorStr;
}

QString KGlobalData::getPenColor()
{
    return m_penColor;
}

void KGlobalData::setBrushColor(const QString& colorStr)
{
    m_brushColor = colorStr;
}

QString KGlobalData::getBrushColor()
{
    return m_brushColor;
}

void KGlobalData::setCanvaScale(qreal scale)
{
    m_scale = scale;
}

qreal KGlobalData::getCanvasScale()
{
    return m_scale;
}

KGlobalData::~KGlobalData()
{
}
