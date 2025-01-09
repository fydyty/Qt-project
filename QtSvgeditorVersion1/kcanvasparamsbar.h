#ifndef __K_CANVAS_PARAMS_BAR_H_
#define __K_CANVAS_PARAMS_BAR_H_

#include "kparamsbar.h"
#include "kcolorbox.h"
#include "kvaluebox.h"
#include "kstylebox.h"
class KCanvasParamsBar : public KParamsBar
{
	Q_OBJECT
	friend class KMainWindow;
public:
	KCanvasParamsBar(const QString &title,QWidget *parent);
	~KCanvasParamsBar();
private:
	KValueBox *m_pWidthBox;
	KValueBox *m_pHeightBox;
	KValueBox *m_pScaleBox;
	KValueBox *m_pPenWidthBox;
	KColorBox *m_pColorBox;
	KColorBox *m_pColorPenBox;
	KColorBox *m_pColorBrushBox;
	KStyleBox* m_pStyleBox;
};

#endif
