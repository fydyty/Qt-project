#include "kmainwindow.h"
#include <QLineEdit>

KMainWindow::KMainWindow(QWidget *parent)
    : QWidget(parent)
    ,m_pVLayout(Q_NULLPTR)
    ,m_pHLayout(Q_NULLPTR)
    ,m_pSvgMainWin(Q_NULLPTR)
    ,m_pToolBar(Q_NULLPTR)
{
    ui.setupUi(this);
    initWindows();
    initConnection();
}

void KMainWindow::initWindows()
{
    m_pVLayout = new QVBoxLayout(this);
    m_pVLayout->setSpacing(0);
    m_pVLayout->setMargin(0);

    m_pHLayout = new QHBoxLayout(this);
    m_pHLayout->setSpacing(0);

    m_pToolBar = new KToolBar(this);
    m_pSvgMainWin = new KSvgMainWindow(this);
    m_pCanvasParamsBar = new KCanvasParamsBar(QStringLiteral("画布"),this);

	m_pMenuBar = new QMenuBar(this);
    m_pMenuBar->setStyleSheet("QMenuBar { background-color: black; color: white; }");

	m_pMenu = new QMenu(QString::fromLocal8Bit("文件"), m_pMenuBar);
	m_pMenuBar->addMenu(m_pMenu);
    m_pMenu->setStyleSheet("QMenu { background-color: white; color: black; }"
			"QMenu::item:selected { background-color: black; color: white; }");

	QAction* newFileAction = new QAction(QString::fromLocal8Bit("新建文件"), this);
	newFileAction->setShortcut(QKeySequence("Ctrl+N"));
	m_pMenu->addAction(newFileAction);

	QAction* openSVGAction = new QAction(QString::fromLocal8Bit("打开SVG"), this);
	openSVGAction->setShortcut(QKeySequence("Ctrl+O"));
	m_pMenu->addAction(openSVGAction);

	QAction* saveAction = new QAction(QString::fromLocal8Bit("保存"), this);
	saveAction->setShortcut(QKeySequence("Ctrl+S"));
	m_pMenu->addAction(saveAction);

	QAction* exportPNGAction = new QAction(QString::fromLocal8Bit("导出PNG"), this);
	exportPNGAction->setShortcut(QKeySequence("Ctrl+E"));
	m_pMenu->addAction(exportPNGAction);

    m_pHLayout->addWidget(m_pToolBar);
    m_pHLayout->addWidget(m_pSvgMainWin);
    m_pHLayout->addWidget(m_pCanvasParamsBar);

    m_pVLayout->addWidget(m_pMenuBar);
    m_pVLayout->addLayout(m_pHLayout);
    

    setLayout(m_pVLayout);
 }

void KMainWindow::initConnection()
{
    (void)connect(m_pCanvasParamsBar->m_pWidthBox->m_pParamEdit, &QLineEdit::editingFinished, this, &KMainWindow::validateCanvasParams);
    (void)connect(m_pCanvasParamsBar->m_pHeightBox->m_pParamEdit, &QLineEdit::editingFinished, this, &KMainWindow::validateCanvasParams);
    (void)connect(m_pCanvasParamsBar->m_pScaleBox->m_pParamEdit, &QLineEdit::editingFinished, this, &KMainWindow::validateCanvasParams);
    (void)connect(m_pCanvasParamsBar->m_pPenWidthBox->m_pParamEdit, &QLineEdit::editingFinished, this, &KMainWindow::changePenWidth);
	(void)connect(m_pCanvasParamsBar->m_pStyleBox->m_pStylePen, SIGNAL(currentIndexChanged(int)), this, SLOT(changePenStyle(int)));
    (void)connect(m_pCanvasParamsBar->m_pColorBox, &KColorBox::pickedColor, this, &KMainWindow::validateCanvasParams);
    (void)connect(m_pCanvasParamsBar->m_pColorPenBox, &KColorBox::pickedColor, this, &KMainWindow::changePenColor);
    (void)connect(m_pCanvasParamsBar->m_pColorBrushBox, &KColorBox::pickedColor, this, &KMainWindow::changeBrushColor);
    (void)connect(m_pSvgMainWin, &KSvgMainWindow::changeRate, this, &KMainWindow::changeParams);

    (void)connect(m_pMenu->actions().at(0), &QAction::triggered, this, &KMainWindow::onNewFile);
    (void)connect(m_pMenu->actions().at(1), &QAction::triggered, this, &KMainWindow::openSvgFile);
    (void)connect(m_pMenu->actions().at(2), &QAction::triggered, this, &KMainWindow::saveSvgFile);
    (void)connect(m_pMenu->actions().at(3), &QAction::triggered, this, &KMainWindow::saveAsPng);
}

void KMainWindow::changeParams()
{
    m_pCanvasParamsBar->m_pScaleBox->m_pParamEdit->setText(QString::number(KGlobalData::getGlobalDataIntance()->getCanvasScale()));
}


void KMainWindow::validateCanvasParams()
{
    qint32 width = m_pCanvasParamsBar->m_pWidthBox->m_pParamEdit->text().toInt();
    qint32 height = m_pCanvasParamsBar->m_pHeightBox->m_pParamEdit->text().toInt();
    qreal scale = m_pCanvasParamsBar->m_pScaleBox->m_pParamEdit->text().toFloat();
    QString str = m_pCanvasParamsBar->m_pColorBox->getStr();

    KGlobalData::getGlobalDataIntance()->setCanvasWidth(width);
    KGlobalData::getGlobalDataIntance()->setCanvasHeight(height);
    KGlobalData::getGlobalDataIntance()->setCanvaScale(scale);
    KGlobalData::getGlobalDataIntance()->setCanvasColor(str);
    m_pSvgMainWin->m_pCanvas->resize(width * scale, height * scale);
    
    m_pSvgMainWin->m_pCanvas->setStyleSheet(QString("background-color:#%1").arg(KGlobalData::getGlobalDataIntance()->getCanvasColor()));
    
}

void KMainWindow::changePenWidth()
{
    qint32 penWidth = m_pCanvasParamsBar->m_pPenWidthBox->m_pParamEdit->text().toInt();
    KGlobalData::getGlobalDataIntance()->setShapePenWidth(penWidth);
    if (m_pSvgMainWin->m_pCanvas->getSelected())
    {
        KShape* currentShape = m_pSvgMainWin->m_pCanvas->getShape();
        currentShape->setPenWidth(penWidth);
        m_pSvgMainWin->m_pCanvas->update();
    }
}

void KMainWindow::changePenStyle(int pos)
{
    Qt::PenStyle style;
	switch (pos)
	{
	case 0:
        style = Qt::SolidLine;
		break;
	case 1:
        style = Qt::DashLine;
		break;
	case 2:
        style = Qt::DotLine;
		break;
	default:
        style = Qt::SolidLine;
		break;
	}
	KGlobalData::getGlobalDataIntance()->setShapePenStyle(style);
	if (m_pSvgMainWin->m_pCanvas->getSelected())
	{
		KShape* currentShape = m_pSvgMainWin->m_pCanvas->getShape();
		currentShape->setPenStyle(style);
		m_pSvgMainWin->m_pCanvas->update();
	}
}

void KMainWindow::changePenColor()
{
    QString color = m_pCanvasParamsBar->m_pColorPenBox->getStr();
	KGlobalData::getGlobalDataIntance()->setPenColor(color);
	if (m_pSvgMainWin->m_pCanvas->getSelected())
	{
		KShape* currentShape = m_pSvgMainWin->m_pCanvas->getShape();
		currentShape->setPenColor(color);
		m_pSvgMainWin->m_pCanvas->update();
	}
}

void KMainWindow::changeBrushColor()
{
    QString color = m_pCanvasParamsBar->m_pColorBrushBox->getStr();
	KGlobalData::getGlobalDataIntance()->setBrushColor(color);
	if (m_pSvgMainWin->m_pCanvas->getSelected())
	{
		KShape* currentShape = m_pSvgMainWin->m_pCanvas->getShape();
		currentShape->setBrushColor(color);
		m_pSvgMainWin->m_pCanvas->update();
	}
}

void KMainWindow::onNewFile()
{
    m_pSvgMainWin->m_pCanvas->clear();
}

void KMainWindow::openSvgFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("打开SVG文件"), "", "SVG Files (*.svg)");
	if (!fileName.isEmpty())
	{
		QSvgRenderer renderer(fileName);

		QImage image(m_pSvgMainWin->m_pCanvas->size(), QImage::Format_ARGB32);
		//image.fill(Qt::transparent);

		QPainter imagePainter(&image);
		renderer.render(&imagePainter);
		imagePainter.end();

		m_pSvgMainWin->m_pCanvas->setSvgImage(image);
		m_pSvgMainWin->m_pCanvas->update();
	}
}

void KMainWindow::saveSvgFile()
{
	QString fileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("保存SVG文件"), "", "SVG Files (*.svg)");
	if (!fileName.isEmpty())
	{
		// 创建一个QSvgGenerator对象来生成SVG文件
		QSvgGenerator generator;
		generator.setFileName(fileName);
		generator.setSize(QSize(KGlobalData::getGlobalDataIntance()->getCanvasWidth(), KGlobalData::getGlobalDataIntance()->getCanvasHeight())); // 设置SVG文件的大小
		generator.setViewBox(QRect(0, 0, KGlobalData::getGlobalDataIntance()->getCanvasWidth()
			* KGlobalData::getGlobalDataIntance()->getCanvasScale(), 
			KGlobalData::getGlobalDataIntance()->getCanvasHeight()
			* KGlobalData::getGlobalDataIntance()->getCanvasScale())); // 设置SVG文件的视图框
		
		QPainter painter(&generator);
		painter.setBrush(KShape::toRgb(KGlobalData::getGlobalDataIntance()->getCanvasColor()));
		painter.drawRect(0, 0, KGlobalData::getGlobalDataIntance()->getCanvasWidth()
			* KGlobalData::getGlobalDataIntance()->getCanvasScale(),
			KGlobalData::getGlobalDataIntance()->getCanvasHeight()
			* KGlobalData::getGlobalDataIntance()->getCanvasScale());
		QList <KShape*> shapeList = m_pSvgMainWin->m_pCanvas->getShapeList();
		if (!shapeList.isEmpty())
		{
			auto it = shapeList.rbegin();
			while (it != shapeList.rend())
			{
				(*it)->drawShape(&generator,&painter);
				it++;
			}
		}
		
		painter.end();
	}
}

void KMainWindow::saveAsPng() {
	QString fileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("保存PNG文件"), "", "PNG Files (*.png)");
	if (!fileName.isEmpty())
	{
		// 创建一个QImage对象，使用设备的大小和颜色格式
		QImage image(m_pSvgMainWin->m_pCanvas->size(), m_pSvgMainWin->m_pCanvas->colorCount() == 32 ? QImage::Format_ARGB32 : QImage::Format_RGB32);

		// 创建一个QPainter对象，用于在QImage上绘制
		QPainter painter(&image);
		m_pSvgMainWin->m_pCanvas->render(&painter); // 将设备的内容渲染到QPainter上

		// 结束绘制并保存为PNG文件
		painter.end();
		image.save(fileName);
	}
}