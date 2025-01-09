#include "kstylebox.h"
#include <QRegExpValidator>

KStyleBox::KStyleBox(const QString &title,QWidget *parent)
	: KParamBox(title,parent)
	, m_pStylePen(Q_NULLPTR)
{
	m_pStylePen = new QComboBox(this);
	m_pStylePen->addItem(QString::fromLocal8Bit("  —"));
	m_pStylePen->addItem("  - -");
	m_pStylePen->addItem("  . .");
	m_pStylePen->setStyleSheet("background-color:#3f3f3c; font-size:20px; color:#4f80ff; font-family: Microsoft YaHei; font-weight: bold; border-width:0; border-style:outset");
	m_pStylePen->setCurrentIndex(0);

	m_pVLayout->addWidget(m_pStylePen);// 加入到布局中 m_pVLayout 设置在父类中
}

KStyleBox::~KStyleBox()
{
}
