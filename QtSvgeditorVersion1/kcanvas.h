#ifndef __K_CANVAS_H_
#define __K_CANVAS_H_

#include <QWidget>
#include <QMenu>
#include <QLabel>
#include "kshape.h"
#include "kglobaldata.h"

enum class KTransType
{
	None = 0,
	TopLeft, 
	Top,
	TopRight,
	Left,
	Contains,// 全部 
	Right,
	BottomLeft,
	Bottom,
	BottomRight
};


class KCanvas : public QWidget
{
	Q_OBJECT

public:
	KCanvas(QWidget *parent = Q_NULLPTR);
	~KCanvas();

	virtual void paintEvent(QPaintEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	

	KShape* getCurrentShape(const QPoint &pos);;// 获取当前位置图形
	KTransType getTransType(const QPoint &pos); // 获取移动类型
	void updateCusorStyle(KGlobalData::KDrawFlag flag, KTransType transType);
	void dragMoveShape(KTransType transType,const QPoint &pos);
	KShape* getShape();
	QList <KShape*> getShapeList();
	bool getSelected();
	void setSvgImage(QImage &image);
	void clear();

	void onSelectAll();
	void onCut();
	void onCopy();
	void onPaste();
	void onDuplicate();
	void onDelete();
	void onUpLayer();
	void onDownLayer();
	void onMostFront();
	void onMostBack();
private:
	KShape* m_pCurrentShape;
	KShape* m_pasteShape;
	QList <KShape*> m_pShapeList;// 存储当前画布中的图形
	QVector <QVector<QPoint>> m_points;
	QPoint m_lastPos;// 记录前一次的位置
	QImage m_image;
	QLabel m_label;
	KTransType m_TransType;// 记录移动类型
	
	bool m_isLPress;// 标记鼠标左键
	bool m_isDrawing;// 是否绘图
	bool m_isSelected;// 是否为选中
	bool m_onSelectAll;
	bool m_isImage;
};

#endif
