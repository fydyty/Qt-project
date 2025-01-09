#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
	, m_fileMonitor(nullptr)
{
    ui->setupUi(this);
	setGeometry(700, 100, 800, 800);
	m_centralWidget = new QWidget;

	//文件列表
	QStackedWidget* stackedWidget = new QStackedWidget(this);
	setCentralWidget(stackedWidget);

	m_model = new QFileSystemModel;
	m_model->setRootPath(QDir::rootPath());
	m_treeView = new QTreeView;
	m_treeView->setModel(m_model);
	m_treeView->setRootIndex(m_model->index("Desktop"));
	m_treeView->header()->resizeSection(0, 200);
	//是否监控子文件
	m_subFolders = new QCheckBox("watch sub-folders");
	//选择的文件
	m_selectPathLabel = new QLabel("Selected Folder:");
	//选择要触发的事件
	m_eventLayout = new QGridLayout;
	m_fileCreate = new QCheckBox("File create");
	m_fileDelete = new QCheckBox("File delete");
	m_fileChange = new QCheckBox("File change");
	m_fileRename = new QCheckBox("File rename");

	m_eventLayout->addWidget(m_fileCreate, 1, 0);
	m_eventLayout->addWidget(m_fileDelete, 1, 1);
	m_eventLayout->addWidget(m_fileChange, 2, 0);
	m_eventLayout->addWidget(m_fileRename, 2, 1);
	
	//启动，结束按钮
	m_buttonLayout = new QHBoxLayout;
	m_start = new QPushButton("Start");
	m_stop = new QPushButton("Stop");
	m_saveConfigButton = new QPushButton("Save Config");
	m_loadConfigButton = new QPushButton("Load Config");
	m_buttonLayout->addWidget(m_start);
	m_buttonLayout->addWidget(m_stop);
	m_buttonLayout->addWidget(m_saveConfigButton);
	m_buttonLayout->addWidget(m_loadConfigButton);
	//日志
	m_log = new QTextEdit;
	m_log->setReadOnly(true);
	//过滤器
	m_filter = new KFilter;
	//文件监控
	m_fileMonitor = new KFileMonitor(this);
	//最小化按钮
	m_minimize = new QPushButton("Tray");
	//日志保存路径
	m_saveLayout = new QHBoxLayout(this);
	QSpacerItem* spacer = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);
	m_saveLogPath = new QLabel("Save Log File:");
	m_saveLog = new QPushButton("Select save file", this);

	m_saveLayout->addWidget(m_saveLog);
	m_saveLayout->addItem(spacer);
	m_saveLayout->addWidget(m_minimize);

	
	// 整体布局
	m_layout = new QVBoxLayout;
	m_layout->addWidget(m_treeView);
	m_layout->addWidget(m_subFolders);
	m_layout->addWidget(m_selectPathLabel);
	m_layout->addLayout(m_eventLayout);
	m_layout->addWidget(m_filter);
	m_layout->addLayout(m_buttonLayout);
	m_layout->addWidget(m_log);
	m_layout->addLayout(m_saveLayout);
	m_layout->addWidget(m_saveLogPath);
	
	m_centralWidget->setLayout(m_layout);

	//面板2
	m_userBase = new KDataBase("Users", this);

	//面板3
	m_dataBase = new KDataBase("Logs",this);
	
	//三个面板整合
	stackedWidget->addWidget(m_centralWidget);
	stackedWidget->addWidget(m_userBase);
	stackedWidget->addWidget(m_dataBase);
	//菜单选项
	QMenuBar* menuBar = new QMenuBar(this);
	menuBar->setStyleSheet(
		"QMenuBar::item {"
		"   padding: 5px 10px;"  // 内边距
		"   background-color: #f0f0f0;"
		"   border: 1px solid #a0a0a0;"  // 每个菜单项的边框
		"}"
		"QMenuBar::item:selected {"
		"   background-color: #d0d0d0;"  // 悬停时背景色
		"}"
		"QMenuBar::item:pressed {"
		"   background-color: #b0b0b0;"  // 按下时背景色
		"}"
		
	);
	setMenuBar(menuBar);

	QAction* action1 = menuBar->addAction("Watch Settings");
	QAction* action2 = menuBar->addAction("User Logs");
	QAction* action3 = menuBar->addAction("Monitor Logs");
	//最小化托盘
	m_trayIcon = new QSystemTrayIcon(this);
	m_trayIcon->setIcon(QIcon(":/icons/monitor.png")); // 设置托盘图标
	m_trayIcon->setToolTip("FileMonitor By QiuDer"); // 设置鼠标悬停提示

	QMenu* trayMenu = new QMenu(this);
	QAction* startAction = new QAction("Start", this);
	QAction* stopAction = new QAction("Stop", this);
	QAction* restoreAction = new QAction("Restore", this);
	QAction* quitAction = new QAction("Exit", this);
	(void)QObject::connect(startAction, &QAction::triggered, m_fileMonitor, &KFileMonitor::startMonitoring);
	(void)QObject::connect(stopAction, &QAction::triggered, m_fileMonitor, &KFileMonitor::stopMonitoring);
	(void)QObject::connect(restoreAction, &QAction::triggered, this, &MainWindow::showNormal);
	(void)QObject::connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
	trayMenu->addAction(startAction);
	trayMenu->addAction(stopAction);
	trayMenu->addAction(restoreAction);
	trayMenu->addAction(quitAction);
	m_trayIcon->setContextMenu(trayMenu);

	// 关联菜单项到槽函数
	(void)QObject::connect(action1, &QAction::triggered, [stackedWidget]() { stackedWidget->setCurrentIndex(0); });
	(void)QObject::connect(action2, &QAction::triggered, [stackedWidget]() { stackedWidget->setCurrentIndex(1); });
	(void)QObject::connect(action3, &QAction::triggered, [stackedWidget]() { stackedWidget->setCurrentIndex(2); });

	
	// 连接信号和槽，当选中树形视图中的文件时，显示文件内容到文本编辑器
	(void)QObject::connect(m_treeView, &QTreeView::clicked, [=](const QModelIndex& index) {
		m_filePath = m_model->filePath(index);
		m_selectPathLabel->setText("Selected Folder:" + m_filePath);
		m_fileMonitor->setPath(m_filePath);
		QString timestamp = QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss");
		emit userHandle(timestamp, "selected Folder", "Selected Folder:" + m_filePath);
	});

	(void)QObject::connect(m_fileMonitor, &KFileMonitor::fileEvent, this,&MainWindow::addLog);

	(void)QObject::connect(m_start, &QPushButton::clicked,m_fileMonitor,&KFileMonitor::startMonitoring);

	(void)QObject::connect(m_stop, &QPushButton::clicked, m_fileMonitor,&KFileMonitor::stopMonitoring);
	(void)QObject::connect(m_start, &QPushButton::clicked, this, [=]() {
		m_start->setEnabled(false);
		m_stop->setEnabled(true);
		QString timestamp = QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss");
		emit userHandle(timestamp,"start clicked","start monitor");
		});

	(void)QObject::connect(m_stop, &QPushButton::clicked, this, [=]() {
		m_start->setEnabled(true);
		m_stop->setEnabled(false);
		QString timestamp = QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss");
		emit userHandle(timestamp, "stop clicked", "stop monitor");
		});

	(void)QObject::connect(m_subFolders, &QCheckBox::clicked,m_fileMonitor,&KFileMonitor::isSubFolders);
	(void)QObject::connect(m_fileCreate, &QCheckBox::clicked,m_fileMonitor,&KFileMonitor::isCreate);
	(void)QObject::connect(m_fileChange, &QCheckBox::clicked,m_fileMonitor,&KFileMonitor::isEdit);
	(void)QObject::connect(m_fileDelete, &QCheckBox::clicked,m_fileMonitor,&KFileMonitor::isDelete);
	(void)QObject::connect(m_fileRename, &QCheckBox::clicked, m_fileMonitor, & KFileMonitor::isRename);
	
	(void)QObject::connect(m_filter, &KFilter::inSuffIsOk, m_fileMonitor, &KFileMonitor::setFilterInSuffixes);
	(void)QObject::connect(m_filter, &KFilter::exSuffIsOk, m_fileMonitor, &KFileMonitor::setFilterExSuffixes);

	(void)QObject::connect(m_saveConfigButton, &QPushButton::clicked, this, &MainWindow::onSaveConfigButtonClicked);
	(void)QObject::connect(m_loadConfigButton, &QPushButton::clicked, this, &MainWindow::onLoadConfigButtonClicked);

	(void)QObject::connect(m_minimize, &QPushButton::clicked, this, &MainWindow::on_minimizeButton_clicked);
	(void)QObject::connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayIconActivated);
	(void)QObject::connect(m_fileMonitor, &KFileMonitor::fileEvent,this,&MainWindow::showMessage);
	(void)QObject::connect(m_saveLog, &QPushButton::clicked, this, &MainWindow::openFileDialog);

	(void)QObject::connect(m_fileMonitor, &KFileMonitor::fileInsert, m_dataBase, &KDataBase::insertLogData, Qt::QueuedConnection);
	(void)QObject::connect(this,&MainWindow::userHandle, m_userBase, &KDataBase::insertLogData, Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
	delete m_fileMonitor;
	delete m_model;
	delete m_treeView;
	delete m_subFolders;
	delete m_selectPathLabel;
	delete m_eventLayout;
	delete m_fileCreate;
	delete m_fileDelete;
	delete m_fileChange;
	delete m_fileRename;
	delete m_buttonLayout;
	delete m_start;
	delete m_stop;
	delete m_saveConfigButton;
	delete m_loadConfigButton;
	delete m_log;
	delete m_filter;
	delete m_minimize;
	delete m_saveLayout;
	delete m_saveLogPath;
	delete m_saveLog;
	delete m_layout;
	delete m_userBase;
	delete m_dataBase;
	delete m_trayIcon;
	delete ui;
}

void MainWindow::addLog(const QString& eventInfo)
{
	m_log->append(eventInfo);
}

void MainWindow::onSaveConfigButtonClicked()
{
	QString configFilePath = QFileDialog::getSaveFileName(this, tr("Save Config"), "", tr("Config Files (*.ini)"));
	if (!configFilePath.isEmpty())
	{
		m_fileMonitor->saveConfig(configFilePath);
		QString timestamp = QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss");
		emit userHandle(timestamp, "save config", "save file:" + configFilePath);
		QMessageBox::information(this, tr("Save Config"), tr("Configuration saved to: %1").arg(configFilePath));
	}
}

void MainWindow::onLoadConfigButtonClicked()
{
	QString configFilePath = QFileDialog::getOpenFileName(this, tr("Load Config"), "", tr("Config Files (*.ini)"));
	if (!configFilePath.isEmpty() && QFile::exists(configFilePath))
	{
		m_fileMonitor->loadConfig(configFilePath);
		updateUi();
		QString timestamp = QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss");
		emit userHandle(timestamp, "load config", "loaded file:" + configFilePath);
		QMessageBox::information(this, tr("Load Config"), tr("Configuration loaded from: %1").arg(configFilePath));
	}
	else
	{
		QMessageBox::warning(this, tr("Load Config"), tr("File not found or invalid."));
	}
}


void MainWindow::updateUi()
{
	m_selectPathLabel->setText("Selected Folder:" + m_fileMonitor->m_directoryPath);
	m_subFolders->setChecked(m_fileMonitor->m_isSubFolders);
	m_fileCreate->setChecked(m_fileMonitor->m_isCreate);
	m_fileDelete->setChecked(m_fileMonitor->m_isDelete);
	m_fileRename->setChecked(m_fileMonitor->m_isRename);
	m_fileChange->setChecked(m_fileMonitor->m_isEdit);

	QString suff = "";
	for (auto x : m_fileMonitor->m_filterInSuffixes)
	{
		suff.append(x + ",");
	}
	m_filter->m_include->setText(suff);
	suff = "";
	for (auto x : m_fileMonitor->m_filterExSuffixes)
	{
		suff.append(x + ",");
	}
	m_filter->m_exclude->setText(suff);
}

void MainWindow::on_minimizeButton_clicked()
{
	this->hide();
	m_trayIcon->show();
	QString timestamp = QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss");
	emit userHandle(timestamp, "Tray", "Minimize pallets");
}

void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::Trigger) {
		if (this->isHidden())
		{
			this->showNormal();
			QString timestamp = QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss");
			emit userHandle(timestamp, "Display", "display interface");
		}
		else
		{
			this->hide();
			QString timestamp = QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss");
			emit userHandle(timestamp, "hide", "hide interface");
		}
	}
	else if (reason == QSystemTrayIcon::Context) {
		m_trayIcon->contextMenu()->exec(QCursor::pos());
	}
}

void MainWindow::showMessage(const QString& eventInfo)
{
	if (m_trayIcon && m_trayIcon->isVisible())
	{
		m_trayIcon->showMessage("File Event Notification", eventInfo, QSystemTrayIcon::Information, 3000);
	}
}


void MainWindow::openFileDialog()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt)"));
	if (!fileName.isEmpty())
	{
		m_saveLogPath->setText("Save File:" + fileName);
		m_fileMonitor->m_saveLogFile = fileName;
		QString timestamp = QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss");
		emit userHandle(timestamp, "save log", "save log to:" + fileName);
	}
}