#include "kcolorbox.h"


KColorBox::KColorBox(const QString &title,QString &rgbColorStr,QWidget *parent)
	: KParamBox(title,parent)
	, m_pColorBtn(Q_NULLPTR)
{
	m_pColorBtn = new QPushButton(this);
	m_pColorBtn->setMaximumSize(70, 50);
	m_pColorBtn->setStyleSheet(QString("border-width: 0px; background-color:#%1; border-style:outset;margin:5px").arg(rgbColorStr));
	m_pVLayout->addWidget(m_pColorBtn); // 添加到布局器
	m_str = rgbColorStr;
	(void)connect(m_pColorBtn, &QPushButton::clicked, this, &KColorBox::showPickColorDlg);
}

KColorBox::~KColorBox()
{
}

void KColorBox::mouseReleaseEvent(QMouseEvent* event)
{
	m_pColorBtn->click();
}

QString KColorBox::getStr()
{
	return m_str;
}

void KColorBox::showPickColorDlg()
{
	// QColorDialog : 系统实现颜色选择框
	QColor color = QColorDialog::getColor(QColor(m_str.toUInt(NULL, 16)), nullptr, QStringLiteral("选择颜色"), QColorDialog::ShowAlphaChannel);

	if (!color.isValid())
		return;

	m_str = QString::number(color.rgb(), 16); // 将 16进制的颜色值转换成 str 字符串

	// 将当前颜色值保存到全局数据对象中
	//KGlobalData::getGlobalDataIntance()->setCanvasColor(str);

	// 设置按钮背景颜色为当前选择的颜色
	m_pColorBtn->setStyleSheet(QString("border-width: 0px; background-color:#%1; border-style:outset;margin:5px").arg(m_str));

	emit pickedColor(); // 发出信号	
}

