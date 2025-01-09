#include "kdatabase.h"

KDataBase::KDataBase(QString name, QWidget* parent)
	:QWidget(parent)
	, m_name(name)
	, m_thread(nullptr)
	, m_worker(nullptr)
{
	initializeDatabase();
	m_thread = new QThread(this);
	m_thread->start();
	initializeWorker();
	initializeUI();
}

KDataBase::~KDataBase()
{
	delete m_model;
	m_db.close(); // 关闭数据库连接
	delete m_tableView;
	delete m_fromDateTimeEdit;
	delete m_toDateTimeEdit;
	delete m_searchEdit;

	m_thread->quit();
	m_thread->wait();
	delete m_worker;
	delete m_thread;
}

void KDataBase::initializeDatabase()
{
	// 创建并打开数据库连接
	m_db = QSqlDatabase::addDatabase("QSQLITE",m_name);  // 使用唯一的连接名称
	m_db.setDatabaseName(QString::fromStdString("%1.db").arg(m_name));
	if (!m_db.open()) {
		qDebug() << "Database Error in KDataBase: " << m_db.lastError().text();
		return;
	}
}

void KDataBase::initializeUI()
{
	QVBoxLayout* mainLayout = new QVBoxLayout(this);

	// 时间段查询部分
	QHBoxLayout* timeFilterLayout = new QHBoxLayout;
	QLabel* fromLabel = new QLabel("From:");
	QLabel* toLabel = new QLabel("To:");
	m_fromDateTimeEdit = new QDateTimeEdit;
	m_toDateTimeEdit = new QDateTimeEdit;
	m_fromDateTimeEdit->setCalendarPopup(true);
	m_toDateTimeEdit->setCalendarPopup(true);
	m_fromDateTimeEdit->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
	m_toDateTimeEdit->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
	m_fromDateTimeEdit->setDateTime(QDateTime::currentDateTime());
	m_toDateTimeEdit->setDateTime(QDateTime::currentDateTime());
	timeFilterLayout->addWidget(fromLabel);
	timeFilterLayout->addWidget(m_fromDateTimeEdit);
	timeFilterLayout->addWidget(toLabel);
	timeFilterLayout->addWidget(m_toDateTimeEdit);

	// 关键字搜索部分
	QHBoxLayout* searchLayout = new QHBoxLayout;
	m_searchEdit = new QLineEdit;
	m_searchEdit->setPlaceholderText("Search...");
	QPushButton* searchButton = new QPushButton("Search");
	QPushButton* insertButton = new QPushButton("Insert from DataBase");
	QPushButton* clearButton = new QPushButton("clear DataBase");
	searchLayout->addWidget(m_searchEdit);
	searchLayout->addWidget(searchButton);
	searchLayout->addWidget(insertButton);
	searchLayout->addWidget(clearButton);

	// 表格显示部分
	m_tableView = new QTableView;
	m_model = new QSqlTableModel(this, m_db);  // 将数据库连接传递给模型
	m_model->setTable(m_name);
	m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
	m_model->select();

	m_tableView->setModel(m_model);
	m_tableView->setSortingEnabled(true);
	// 设置前三列为固定宽度
	m_tableView->setColumnWidth(0, 80);
	m_tableView->setColumnWidth(1, 200);
	m_tableView->setColumnWidth(2, 200);

	// 设置前三列为固定模式
	m_tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	m_tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
	m_tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);

	// 设置第四列为 Stretch 模式
	m_tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
	m_tableView->update();

	// 将控件添加到布局中
	mainLayout->addLayout(timeFilterLayout);
	mainLayout->addLayout(searchLayout);
	mainLayout->addWidget(m_tableView);

	// 信号与槽连接
	(void)QObject::connect(searchButton, &QPushButton::clicked, this, &KDataBase::onSearchClicked);
	(void)QObject::connect(insertButton, &QPushButton::clicked, this, &KDataBase::onInsertClicked);
	(void)QObject::connect(clearButton, &QPushButton::clicked, m_worker, &KDataWorker::clearTable);
	(void)QObject::connect(m_worker, &KDataWorker::clearFinished, this, &KDataBase::onInsertClicked);

	// 设置布局
	setLayout(mainLayout);
}

void KDataBase::initializeWorker()
{
	m_worker = new KDataWorker(m_name);  // 在新线程中创建 KDataWorker
	m_worker->moveToThread(m_thread);
	(void)QObject::connect(this, &KDataBase::dataInsert, m_worker, &KDataWorker::insertLogData, Qt::QueuedConnection);
}

void KDataBase::insertLogData(const QString& time, const QString& action, const QString& file)
{
	emit dataInsert(time, action, file);
}

void KDataBase::onSearchClicked()
{
	QString fromTime = m_fromDateTimeEdit->dateTime().toString("yyyy/MM/dd HH:mm:ss");
	QString toTime = m_toDateTimeEdit->dateTime().toString("yyyy/MM/dd HH:mm:ss");
	QString keyword = m_searchEdit->text();
	QString filter = QString("time BETWEEN '%1' AND '%2' AND ( action LIKE '%%3%' OR file LIKE '%%3%')").arg(fromTime).arg(toTime).arg(keyword);
	m_model->setFilter(filter);
	m_model->select();
}

void KDataBase::onInsertClicked()
{
	while (m_model->canFetchMore()) {
		m_model->fetchMore();
	}
	m_model->select(); // 重新加载数据
	m_tableView->reset(); // 重置视图

}

