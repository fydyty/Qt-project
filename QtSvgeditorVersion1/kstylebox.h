#ifndef __K_STYLE_BOX_H_
#define __K_STYLE_BOX_H_

#include <QComboBox>

#include "kparambox.h"

class KStyleBox : public KParamBox
{
	Q_OBJECT

	friend class KMainWindow;
public:
	KStyleBox(const QString &title,QWidget *parent);
	~KStyleBox();
private:
	QComboBox *m_pStylePen;
};

#endif
