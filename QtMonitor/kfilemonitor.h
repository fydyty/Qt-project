#ifndef KFILEMONITOR_H
#define KFILEMONITOR_H

#include <QObject>
#include <QString>
#include <QThread>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <Windows.h>
#include <QSet>
#include <QSettings>

class KFileMonitor : public QObject
{
	Q_OBJECT
	friend class MainWindow;
public:
	explicit KFileMonitor(QObject* parent = nullptr);
	~KFileMonitor();

	void setPath(const QString& directoryPath);
	void startMonitoring();
	void stopMonitoring();
	void setFilterInSuffixes(const QSet<QString>& suffixes);
	void setFilterExSuffixes(const QSet<QString>& suffixes);
	void saveConfig(const QString& configFilePath);
	void loadConfig(const QString& configFilePath);

signals:
	void fileEvent(const QString& eventInfo);
	void fileInsert(const QString& time, const QString& action, const QString& file);
public slots:
	void isSubFolders();
	void isCreate();
	void isDelete();
	void isEdit();
	void isRename();
signals:
	void loadOk();
private:
	void monitorDirectory();
	QString formatEventInfo(const QString& eventDescription, const QString& fileName);
	bool shouldMonitorFile(const QString& fileName);

	QString m_directoryPath;
	QString m_saveLogFile;
	HANDLE m_directoryHandle;
	QThread m_thread;
	bool m_stopRequested;

	bool m_isSubFolders;
	bool m_isCreate;
	bool m_isDelete;
	bool m_isEdit;
	bool m_isRename;

	QSet<QString> m_filterInSuffixes;
	QSet<QString> m_filterExSuffixes;
};

#endif // KFILEMONITOR_H
