#include "kdataworker.h"
#include <QDebug>
#include <QThread>
KDataWorker::KDataWorker(QString name, QObject* parent)
	: m_name(name)
	,QObject(parent)
{
	initializeDatabase();
}

void KDataWorker::initializeDatabase()
{
	m_db = QSqlDatabase::addDatabase("QSQLITE", m_name + "worker");  // 为每个线程指定唯一的连接名
	m_db.setDatabaseName(QString::fromStdString("%1.db").arg(m_name));
	if (!m_db.open())
	{
		qDebug() << "Database Error: " << m_db.lastError().text();
		return;
	}
	// 创建表格
	QSqlQuery query(m_db);
	query.exec("PRAGMA journal_mode=WAL;"); // 启用 WAL 模式
	query.exec(QString::fromStdString("CREATE TABLE IF NOT EXISTS %1 ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"time TEXT, "
		"action TEXT, "
		"file TEXT)").arg(m_name));
}


void KDataWorker::insertLogData(const QString& time, const QString& action, const QString& file)
{
	if (!m_db.isOpen()) {
		qDebug() << "Database is not open";
		return;
	}

	QSqlQuery query(m_db);
	query.prepare(QString::fromStdString("INSERT INTO %1 (time, action, file) VALUES (:time, :action, :file)").arg(m_name));
	query.bindValue(":time", time);
	query.bindValue(":action", action);
	query.bindValue(":file", file);

	if (!query.exec()) {
		qDebug() << "Failed to insert log data:" << query.lastError().text();
		return;
	}
}

void KDataWorker::clearTable()
{
	if (!m_db.isOpen()) {
		qDebug() << "Database is not open";
		return;
	}

	QSqlQuery query(m_db);
	query.prepare(QString("DELETE FROM %1").arg(m_name));

	if (!query.exec()) {
		qDebug() << "Failed to clear table:" << query.lastError().text();
	}
	else {
		qDebug() << "Table cleared successfully";
		emit clearFinished();
	}
}