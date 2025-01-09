#ifndef KDATAWORKER_H
#define KDATAWORKER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>

class KDataWorker : public QObject
{
	Q_OBJECT

public:
	explicit KDataWorker(QString name,QObject* parent = nullptr);

public slots:
	void initializeDatabase();  // 在工作线程中初始化数据库
	void insertLogData(const QString& time, const QString& action, const QString& file);
	void clearTable();
signals:
	void clearFinished();
private:
	QString m_name;
	QSqlDatabase m_db;  // 数据库连接对象
	QSqlTableModel* m_model;
};

#endif // KDATAWORKER_H
