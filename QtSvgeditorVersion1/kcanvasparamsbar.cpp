#include "kcanvasparamsbar.h"

KCanvasParamsBar::KCanvasParamsBar(const QString &title,QWidget *parent)
	: KParamsBar(title,parent)
	, m_pWidthBox(Q_NULLPTR)
	, m_pHeightBox(Q_NULLPTR)
	, m_pColorBox(Q_NULLPTR)
{
	m_pWidthBox = new KValueBox(QStringLiteral("���"), KGlobalData::getGlobalDataIntance()->getCanvasWidth(), this);
	m_pHeightBox = new KValueBox(QStringLiteral("�߶�"), KGlobalData::getGlobalDataIntance()->getCanvasHeight(), this);
	m_pScaleBox = new KValueBox(QStringLiteral("����"), KGlobalData::getGlobalDataIntance()->getCanvasScale(), this);
	m_pPenWidthBox = new KValueBox(QStringLiteral("���"), KGlobalData::getGlobalDataIntance()->getShapePenWidth(), this);
	m_pColorBox = new KColorBox(QStringLiteral("��ɫ"), KGlobalData::getGlobalDataIntance()->getCanvasColor(), this);
	m_pColorPenBox = new KColorBox(QStringLiteral("�߿���ɫ"), KGlobalData::getGlobalDataIntance()->getPenColor(), this);
	m_pColorBrushBox = new KColorBox(QStringLiteral("�����ɫ"), KGlobalData::getGlobalDataIntance()->getBrushColor(), this);
	m_pStyleBox = new KStyleBox(QStringLiteral("��ʽ"), this);

	// ��ӵ����񲼾�
	m_pGridLayout->addWidget(m_pWidthBox, 0, 0);// �� 0 �� �� 0 ��
	m_pGridLayout->addWidget(m_pHeightBox, 0, 1);// �� 0 �� �� 1 ��
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
