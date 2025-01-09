#include "kcanvasparamsbar.h"

KCanvasParamsBar::KCanvasParamsBar(const QString &title,QWidget *parent)
	: KParamsBar(title,parent)
	, m_pWidthBox(Q_NULLPTR)
	, m_pHeightBox(Q_NULLPTR)
	, m_pColorBox(Q_NULLPTR)
{
	m_pWidthBox = new KValueBox(QStringLiteral("宽度"), KGlobalData::getGlobalDataIntance()->getCanvasWidth(), this);
	m_pHeightBox = new KValueBox(QStringLiteral("高度"), KGlobalData::getGlobalDataIntance()->getCanvasHeight(), this);
	m_pScaleBox = new KValueBox(QStringLiteral("比例"), KGlobalData::getGlobalDataIntance()->getCanvasScale(), this);
	m_pPenWidthBox = new KValueBox(QStringLiteral("宽度"), KGlobalData::getGlobalDataIntance()->getShapePenWidth(), this);
	m_pColorBox = new KColorBox(QStringLiteral("颜色"), KGlobalData::getGlobalDataIntance()->getCanvasColor(), this);
	m_pColorPenBox = new KColorBox(QStringLiteral("边框颜色"), KGlobalData::getGlobalDataIntance()->getPenColor(), this);
	m_pColorBrushBox = new KColorBox(QStringLiteral("填充颜色"), KGlobalData::getGlobalDataIntance()->getBrushColor(), this);
	m_pStyleBox = new KStyleBox(QStringLiteral("样式"), this);

	// 添加到网格布局
	m_pGridLayout->addWidget(m_pWidthBox, 0, 0);// 第 0 行 第 0 列
	m_pGridLayout->addWidget(m_pHeightBox, 0, 1);// 第 0 行 第 1 列
	m_pGridLayout->addWidget(m_pScaleBox, 1, 0);
	m_pGridLayout->addWidget(m_pColorBox, 1, 1);
	m_pGridLayout->addWidget(m_pPenWidthBox, 2, 0);
	m_pGridLayout->addWidget(m_pStyleBox, 2, 1);
	m_pGridLayout->addWidget(m_pColorPenBox, 3, 0);
	m_pGridLayout->addWidget(m_pColorBrushBox, 3, 1);
}

KCanvasParamsBar::~KCanvasParamsBar()
{
}
