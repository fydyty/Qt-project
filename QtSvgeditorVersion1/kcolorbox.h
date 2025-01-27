#ifndef __K_PARAMBOX_H_
#define __K_PARAMBOX_H_

#include <QPushButton>
#include <QColorDialog>

#include "kparambox.h"

class KColorBox : public KParamBox
{
	Q_OBJECT

public:
	KColorBox(const QString &title,QString &rgbColorStr,
		QWidget *parent);
	~KColorBox();

	// 鼠标释放事件,鼠标释放时弹出颜色选择对话框
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	QString getStr();
signals:
	void pickedColor();
private slots:
	void showPickColorDlg();
private:
	QPushButton* m_pColorBtn;
	QString m_str;
};

#endif
