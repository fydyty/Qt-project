#ifndef KDATABASE_H
#define KDATABASE_H

#include "kdataworker.h"
#include <QWidget>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <QDateTimeEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QSqlQuery>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QTableView>
#include <QHeaderView>
#include <QDebug>
#include <QThread>

class KDataBase:public QWidget
{
	Q_OBJECT
public:
	KDataBase(QString name,QWidget* parent = nullptr);
	~KDataBase();
	void onSearchClicked();
	void onInsertClicked();

	void insertLogData(const QString& time, const QString& action, const QString& file);
	void initializeWorker();
	void initializeDatabase();
	void initializeUI();
signals:
	void dataInsert(const QString& time, const QString& action, const QString& file);
private:
	QString m_name;
	QSqlTableModel* m_model;
	QSqlDatabase m_db;
	QTableView* m_tableView;
	QDateTimeEdit* m_fromDateTimeEdit;
	QDateTimeEdit* m_toDateTimeEdit;
	QLineEdit* m_searchEdit;

	QThread* m_thread;
	KDataWorker* m_worker;
};

#endif // !KDATABASE_H
