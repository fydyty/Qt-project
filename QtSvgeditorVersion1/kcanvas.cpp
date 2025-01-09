#include "kcanvas.h"
#include "kshapefactory.h"

#include <QInputDialog>
#include <QMouseEvent>
#include <QDebug>
#include <QFontMetrics>

KCanvas::KCanvas(QWidget* parent)
	: QWidget(parent)
	, m_pCurrentShape(Q_NULLPTR)
	, m_TransType(KTransType::None)
	, m_isDrawing(false)
	, m_isLPress(false)
	, m_isSelected(false)
	, m_isImage(false)
	, m_onSelectAll(false)
	, m_image(nullptr)
	, m_pasteShape(Q_NULLPTR)
	, m_label(this)
{
	setAttribute(Qt::WA_StyledBackground, true);

	// 设置背景颜色为白色
	setStyleSheet("background-color:#FFFFFF");

	KGlobalData::getGlobalDataIntance()->setCanvasColor("FFFFFFFF");
	resize(KGlobalData::getGlobalDataIntance()->getCanvasWidth(),
		KGlobalData::getGlobalDataIntance()->getCanvasHeight());

	setMouseTracking(true);//设置鼠标追踪
	setFocusPolicy(Qt::ClickFocus);// 单击获取焦点

}

KCanvas::~KCanvas()
{
	delete m_pCurrentShape;
	m_pCurrentShape = nullptr;
	delete m_pasteShape;
	m_pasteShape = nullptr;
	for (auto &shape : m_pShapeList)
	{
		if (shape != nullptr)
		{
			delete shape;
			shape = nullptr;
		}
	}
		
	m_pShapeList.clear();
	
}

// 绘图事件处理
void KCanvas::paintEvent(QPaintEvent* event)
{
	if (!m_pShapeList.isEmpty())
	{
		auto it = m_pShapeList.rbegin();
		while (it != m_pShapeList.rend())
		{ 
			(*it)->drawShape(this);
			it++;
		}
	}

	if (m_pCurrentShape != Q_NULLPTR)
	{
		if (m_isDrawing) // 图形绘制标志为 true，则表示正在绘制该图形，此时需要不断重新绘制
		{
			m_pCurrentShape->initPen();
			m_pCurrentShape->drawShape(this);
		}
			

		if (m_isSelected) // 图形选中，则绘制选中框
			m_pCurrentShape->drawOutLine(this);
	}

	if (m_onSelectAll)
	{
		auto it = m_pShapeList.rbegin();
		while (it != m_pShapeList.rend())
		{
			(*it)->drawOutLine(this);
			it++;
		}
	}

	if (m_isImage)
	{
		m_label.show();
		m_label.setGeometry(0, 0, KGlobalData::getGlobalDataIntance()->getCanvasWidth()
			*KGlobalData::getGlobalDataIntance()->getCanvasScale()
			, KGlobalData::getGlobalDataIntance()->getCanvasHeight()
			* KGlobalData::getGlobalDataIntance()->getCanvasScale());
		QPixmap pixmap = QPixmap::fromImage(m_image);
		m_label.setPixmap(pixmap);
	}
}

void KCanvas::mousePressEvent(QMouseEvent* event)
{
	//获取当前需要绘制的图形类型
	KGlobalData::KDrawFlag flag = KGlobalData::getGlobalDataIntance()->getDrawFlag();
	
	if (flag == KGlobalData::KDrawFlag::NoneDrawFlag)
		return;
	
	if (event->buttons() == Qt::LeftButton)
	{
		m_isLPress = true;// 记录鼠标左键已经点击
		if (flag == KGlobalData::KDrawFlag::MouseDrawFlag)
		{
			m_pCurrentShape = getCurrentShape(event->pos());
			if (m_pCurrentShape != Q_NULLPTR)
			{
				m_lastPos = event->pos();// 当前位置移动的最后位置(会更新)
				m_isSelected = true;// 设置选中标志
				m_TransType = getTransType(event->pos());// 获取移动类型
			}
			else
			{
				m_isSelected = false;
				m_onSelectAll = false;
			}
		}
		else
		{ 
			m_isSelected = false;
			m_pCurrentShape = KShapeFactory::createShape(KGlobalData::getGlobalDataIntance()->getDrawFlag());

			if (m_pCurrentShape != Q_NULLPTR)
			{
				m_pCurrentShape->setStartPoint(event->pos() / KGlobalData::getGlobalDataIntance()->getCanvasScale());
				if (flag == KGlobalData::KDrawFlag::PenDrawFlag)
				{
					QVector<QPoint> points;
					points.push_back(event->pos());
					m_points.push_back(points);
					m_pCurrentShape->setPoints(points);
				}
				else if (flag == KGlobalData::KDrawFlag::TextDrawFlag)
				{
					QString text = QInputDialog::getText(this, tr("Please Input"), tr("Please Input Text:"), QLineEdit::Normal, "");

					QFont font("Arial", 10);
					QFontMetrics fm(font);  
					int width = fm.horizontalAdvance(text);
					int height = fm.height();

					m_pCurrentShape->setText(text);
					m_pCurrentShape->setStartPoint(QPoint(event->pos().x()/KGlobalData::getGlobalDataIntance()->getCanvasScale(), (event->pos().y() + height)/KGlobalData::getGlobalDataIntance()->getCanvasScale()));
					m_pCurrentShape->setEndPoint(QPoint((event->pos().x()+width)/KGlobalData::getGlobalDataIntance()->getCanvasScale(),event->pos().y() / KGlobalData::getGlobalDataIntance()->getCanvasScale()));
					m_pShapeList.prepend(m_pCurrentShape);
					m_pCurrentShape = Q_NULLPTR;

					m_isLPress = false;
					m_isDrawing = false;
					m_TransType = KTransType::None;
				}
			}
				
		}
	}
	update();
}

void KCanvas::mouseMoveEvent(QMouseEvent* event)
{
	KGlobalData::KDrawFlag flag = KGlobalData::getGlobalDataIntance()->getDrawFlag();
	if (flag == KGlobalData::KDrawFlag::NoneDrawFlag)
		return;


	KTransType transType = getTransType(event->pos());
	updateCusorStyle(flag, transType);//选中时设置鼠标选中样式

	if (event->buttons() != Qt::LeftButton)//bug : 在不点击鼠标左键并在在选中状态，移动鼠标会移动图形
			return;


	if (flag != KGlobalData::KDrawFlag::MouseDrawFlag)
	{
		if (m_isLPress && !m_isDrawing)
			m_isDrawing = true;
		if (m_pCurrentShape != Q_NULLPTR)
			m_pCurrentShape->setEndPoint(event->pos() / KGlobalData::getGlobalDataIntance()->getCanvasScale());
		if (flag == KGlobalData::KDrawFlag::PenDrawFlag)
		{
			auto& points = m_points[m_points.size() - 1];
			points.push_back(event->pos());
			m_pCurrentShape->setPoints(points);
		}
	}
	else
	{
		if (m_pCurrentShape == Q_NULLPTR)
			return;
		dragMoveShape(transType, event->pos() / KGlobalData::getGlobalDataIntance()->getCanvasScale());//移动选中图形
	}

	update();
}

void KCanvas::mouseReleaseEvent(QMouseEvent* event)
{
	KGlobalData::KDrawFlag flag = KGlobalData::getGlobalDataIntance()->getDrawFlag();

	if (m_isLPress) // 鼠标左键按下后释放，则表示移动或者绘图完毕
	{
		if (flag != KGlobalData::KDrawFlag::MouseDrawFlag)// 如果是绘图
		{
			if (m_pCurrentShape != Q_NULLPTR)
			{
				if (m_pCurrentShape->isValid())// 判断图形矩形是否有效
					m_pCurrentShape->setEndPoint(event->pos() / KGlobalData::getGlobalDataIntance()->getCanvasScale());

				if (flag == KGlobalData::KDrawFlag::PenDrawFlag)
				{
					auto& points = m_points[m_points.size() - 1];
					points.push_back(event->pos());
					m_pCurrentShape->setPoints(points);
				}
		
				m_pShapeList.prepend(m_pCurrentShape);
				m_pCurrentShape = Q_NULLPTR;
			}
			else
			{
				delete m_pCurrentShape;
				m_pCurrentShape = Q_NULLPTR;
			}
		}

		m_isLPress = false;
		m_isDrawing = false;
		m_TransType = KTransType::None;
	}

	update();
}

KShape* KCanvas::getCurrentShape(const QPoint &pos)
{
	long long minArea = 1e18;
	KShape * res = nullptr;
	for (QList<KShape*>::iterator it = m_pShapeList.begin();
		it != m_pShapeList.end(); ++it)
	{
		if ((*it)->getShapeRect().contains(pos))
		{
			long long area = abs((long long)(*it)->getStartPoint().rx() - (long long)(*it)->getEndPoint().rx())
				* abs((long long)(*it)->getStartPoint().ry() - (long long)(*it)->getEndPoint().ry());
			if (area < minArea)
			{
				minArea = area;
				res = *it;
			}
		}
	}
	return res;
}

KTransType KCanvas::getTransType(const QPoint& pos)
{
	if (!m_pCurrentShape)
		return KTransType::None;

	// 获取图形矩形
	QRect rect = m_pCurrentShape->getShapeRect();

	if (!rect.contains(pos))
		return KTransType::None;
	
	//qDebug() << "rect = " << rect.topLeft().x();
	//qDebug() << "rect = " << rect.topLeft().y();

	// 判断是整体移动，局部拖拽移动
	if (qAbs(pos.x() - rect.topLeft().x()) < 5 
			&& qAbs(pos.y() - rect.topLeft().y()) < 5)
		return KTransType::TopLeft;

	if (qAbs(rect.topRight().x() - pos.x()) < 5 
			&& qAbs(pos.y() - rect.topRight().y()) < 5)
		return KTransType::TopRight;

	if (qAbs(rect.bottomRight().x() - pos.x()) < 5 
			&& qAbs(rect.bottomRight().y() - pos.y()) < 5)
		return KTransType::BottomRight;

	if (qAbs(pos.x() - rect.bottomLeft().x()) < 5 
			&& qAbs(rect.bottomLeft().y() - pos.y()) < 5)
		return KTransType::BottomLeft;

	if (qAbs(pos.y() - rect.top()) < 5 
			&& pos.x() > rect.topLeft().x() + 5
			&& pos.x() < rect.topRight().x() - 5)
		return KTransType::Top;

	if (qAbs(rect.right() - pos.x()) < 5
			&& pos.y() > rect.topRight().y() + 5
			&& pos.y() < rect.bottomRight().y() - 5)
		return KTransType::Right;

	if (qAbs(rect.bottom() - pos.y()) < 5 
			&& pos.x() > rect.bottomLeft().x() + 5 
			&& pos.x() < rect.bottomRight().x() - 5)
		return KTransType::Bottom;

	if (qAbs(pos.x() - rect.left()) < 5 
			&& pos.y() > rect.topLeft().y() + 5 
			&& pos.y() < rect.bottomLeft().y() - 5)
		return KTransType::Left;

	return KTransType::Contains;
}

void KCanvas::updateCusorStyle(KGlobalData::KDrawFlag flag, KTransType transType)
{
	// 如果已经选中
	if (m_isSelected
		&& flag == KGlobalData::KDrawFlag::MouseDrawFlag
		&& transType != KTransType::None)
	{
		//TODO：变换图标,设置不同的鼠标样式,根据点击位置，设置不同的鼠标样式
		if (transType == KTransType::TopLeft || transType == KTransType::BottomRight)
			setCursor(Qt::SizeFDiagCursor);
		else if (transType == KTransType::TopRight || transType == KTransType::BottomLeft)
			setCursor(Qt::SizeBDiagCursor);
		else if (transType == KTransType::Top || transType == KTransType::Bottom)
			setCursor(Qt::SizeVerCursor);
		else if (transType == KTransType::Left || transType == KTransType::Right)
			setCursor(Qt::SizeHorCursor);

		else if (transType == KTransType::Contains)
			setCursor(Qt::SizeAllCursor);
	}
	else
		unsetCursor();
}

void KCanvas::dragMoveShape(KTransType transType,const QPoint &pos)
{
	if (m_pCurrentShape == Q_NULLPTR)
		return;
	switch (m_TransType)
	{
	case KTransType::None:
		return;
	case KTransType::TopLeft:
		// 如果是左上角，则当前位置为新的矩形起始位置
		m_pCurrentShape->moveTopLeft(pos);
		break;
	case KTransType::Top:
		// 如果是向上移动，则修改起始位置的 y 坐标
		m_pCurrentShape->moveTop(pos);
		break;
	case KTransType::TopRight:
		m_pCurrentShape->moveTopRight(pos);
		break;
	case KTransType::Left:
		m_pCurrentShape->moveLeft(pos);
		break;
	case KTransType::Contains: // 整体移动
	{
		// m_lastPos 为选中时光标位置，在移动过程中不断赋值为前一次的位置
		QPoint newpos = pos * KGlobalData::getGlobalDataIntance()->getCanvasScale() -  m_lastPos; // 计算要移动的偏移
		m_pCurrentShape->move(newpos / KGlobalData::getGlobalDataIntance()->getCanvasScale());
		m_lastPos = pos * KGlobalData::getGlobalDataIntance()->getCanvasScale();
	}
	break;
	case KTransType::Right:
		m_pCurrentShape->moveRight(pos);
		break;
	case KTransType::BottomLeft:
		m_pCurrentShape->moveBottomLeft(pos);
		break;
	case KTransType::Bottom:
		m_pCurrentShape->moveBottom(pos);
		break;
	case KTransType::BottomRight:
		m_pCurrentShape->moveBottomRight(pos);
		break;
	default:
		break;
	}
}

KShape* KCanvas::getShape()
{
	return m_pCurrentShape;
}

QList<KShape*> KCanvas::getShapeList()
{
	return m_pShapeList;
}

bool KCanvas::getSelected()
{
	return m_isSelected;
}

void KCanvas::setSvgImage(QImage& image)
{
	m_image = image;
	m_isImage = true;
	update();
}

void KCanvas::clear()
{
	m_pCurrentShape = Q_NULLPTR;
	m_TransType = KTransType::None;
	m_isDrawing = false;
	m_isLPress = false;
	m_isSelected = false;
	m_isImage = false;
	m_label.hide();
	m_pShapeList.clear();
	setStyleSheet("background-color:#FFFFFF");
	KGlobalData::getGlobalDataIntance()->setCanvasColor("FFFFFFFF");
	update();
}

void KCanvas::onSelectAll()
{
	m_onSelectAll = !m_onSelectAll;
	update();
}

void KCanvas::onCut()
{
	if (m_pCurrentShape != Q_NULLPTR)
	{
		QList<KShape*>::iterator it = m_pShapeList.begin();
		for (; it != m_pShapeList.end(); ++it) {
			if ((*it)->getStartPoint() == m_pCurrentShape->getStartPoint()
				&& (*it)->getEndPoint() == m_pCurrentShape->getEndPoint()
				&& (*it)->getShapeType() == m_pCurrentShape->getShapeType()) {
				m_pShapeList.erase(it);
				m_isSelected = false;
				break;
			}
		}
		update();
		m_pasteShape = m_pCurrentShape;
		m_pCurrentShape = Q_NULLPTR;
	}
}

void KCanvas::onCopy()
{
	if (m_pCurrentShape != Q_NULLPTR)
	{
		m_pasteShape = KShapeFactory::createShape(m_pCurrentShape->getShapeType());
		m_pasteShape->copy(m_pCurrentShape);
	}
}

void KCanvas::onPaste()
{
	if (m_pasteShape != Q_NULLPTR)
	{
		m_pShapeList.prepend(m_pasteShape);
		m_pasteShape = Q_NULLPTR;
		update();
	}
}

void KCanvas::onDuplicate()
{
	onCopy();
	onPaste();
}

void KCanvas::onDelete()
{
	if (m_pCurrentShape != Q_NULLPTR)
	{
		QList<KShape*>::iterator it = m_pShapeList.begin();
		for (; it != m_pShapeList.end(); ++it) {
			if ((*it)->getStartPoint() == m_pCurrentShape->getStartPoint()
				&& (*it)->getEndPoint() == m_pCurrentShape->getEndPoint()
				&& (*it)->getShapeType() == m_pCurrentShape->getShapeType()) {
				KShape* temp = *it;
				it = m_pShapeList.erase(it); 
				delete temp;
				m_isSelected = false;
				m_pCurrentShape = Q_NULLPTR;
				break;
			}
		}
		update();
	}
}

void KCanvas::onUpLayer()
{
	if (m_pCurrentShape != Q_NULLPTR)
	{
		QList<KShape*>::iterator it = m_pShapeList.begin();
		for (; it != m_pShapeList.end(); ++it) {
			if ((*it)->getStartPoint() == m_pCurrentShape->getStartPoint()
				&& (*it)->getEndPoint() == m_pCurrentShape->getEndPoint()
				&& (*it)->getShapeType() == m_pCurrentShape->getShapeType()) {
				if ( it != m_pShapeList.begin())
				{
					std::iter_swap(it, it - 1);
				}
				break;
			}
		}
		update();
	}
}

void KCanvas::onDownLayer()
{
	if (m_pCurrentShape != Q_NULLPTR)
	{
		QList<KShape*>::iterator it = m_pShapeList.begin();
		for (; it != m_pShapeList.end(); ++it) {
			if ((*it)->getStartPoint() == m_pCurrentShape->getStartPoint()
				&& (*it)->getEndPoint() == m_pCurrentShape->getEndPoint()
				&& (*it)->getShapeType() == m_pCurrentShape->getShapeType()) 
			{
				if ((it+1)!= m_pShapeList.end())
				{
					std::iter_swap(it, it + 1);
				}
				break;
			}
		}
		update();
	}
}

void KCanvas::onMostFront()
{
	if (m_pCurrentShape != Q_NULLPTR)
	{
		QList<KShape*>::iterator it = m_pShapeList.begin();
		for (; it != m_pShapeList.end(); ++it) {
			if ((*it)->getStartPoint() == m_pCurrentShape->getStartPoint()
				&& (*it)->getEndPoint() == m_pCurrentShape->getEndPoint()
				&& (*it)->getShapeType() == m_pCurrentShape->getShapeType()) {
				m_pShapeList.erase(it);
				m_pShapeList.prepend(m_pCurrentShape);
				break;
			}
		}
		update();
	}
}

void KCanvas::onMostBack()
{
	if (m_pCurrentShape != Q_NULLPTR)
	{
		QList<KShape*>::iterator it = m_pShapeList.begin();
		for (; it != m_pShapeList.end(); ++it) {
			if ((*it)->getStartPoint() == m_pCurrentShape->getStartPoint()
				&& (*it)->getEndPoint() == m_pCurrentShape->getEndPoint()
				&& (*it)->getShapeType() == m_pCurrentShape->getShapeType()) {
				m_pShapeList.erase(it);
				m_pShapeList.append(m_pCurrentShape);
				break;
			}
		}
		update();
	}
}


