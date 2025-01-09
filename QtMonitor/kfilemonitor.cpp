#include "kfilemonitor.h"
#include <QDebug>
#include <QDateTime>

KFileMonitor::KFileMonitor(QObject* parent)
	: QObject(parent)
	, m_directoryHandle(INVALID_HANDLE_VALUE)
	, m_stopRequested(false)
	, m_isSubFolders(false)
	, m_isCreate(false)
	, m_isDelete(false)
	, m_isEdit(false)
	, m_isRename(false)
	, m_saveLogFile("")
{
	moveToThread(&m_thread);
	m_thread.start();
}

KFileMonitor::~KFileMonitor()
{
	stopMonitoring();
	m_thread.quit();
	m_thread.wait();
}

void KFileMonitor::setPath(const QString& directoryPath)
{
	m_directoryPath = directoryPath;
}

void KFileMonitor::startMonitoring()
{

	if (m_directoryHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_directoryHandle);
	}

	m_stopRequested = false;
	m_directoryHandle = CreateFileW(
		m_directoryPath.toStdWString().c_str(),
		FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
		NULL);

	if (m_directoryHandle == INVALID_HANDLE_VALUE)
	{
		qWarning() << "Failed to create directory handle.";
		return;
	}

	QThread::create([this]() { monitorDirectory(); })->start();
}

void KFileMonitor::stopMonitoring()
{
	m_stopRequested = true;
	if (m_directoryHandle != INVALID_HANDLE_VALUE)
	{
		CancelIo(m_directoryHandle);
		CloseHandle(m_directoryHandle);
		m_directoryHandle = INVALID_HANDLE_VALUE;
	}
}


void KFileMonitor::monitorDirectory()
{
	char buffer[1024 * 16];
	DWORD bytesReturned;

	while (!m_stopRequested)
	{
		BOOL result = ReadDirectoryChangesW(
			m_directoryHandle,
			buffer,
			sizeof(buffer),
			m_isSubFolders,
			FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_SECURITY,
			&bytesReturned,
			NULL,
			NULL);

		if (!result)
		{
			qWarning() << "ReadDirectoryChangesW failed.";
			break;
		}

		FILE_NOTIFY_INFORMATION* info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer);
		do
		{
			if (m_stopRequested)
				break;
			QString eventDescription;
			bool ok = true;
			switch (info->Action)
			{
			case FILE_ACTION_ADDED:
				ok = m_isCreate;
				eventDescription = "File create event";
				break;
			case FILE_ACTION_REMOVED:
				ok = m_isDelete;
				eventDescription = "File delete event";
				break;
			case FILE_ACTION_MODIFIED:
				ok = m_isEdit;
				eventDescription = "File modify event";
				break;
			case FILE_ACTION_RENAMED_OLD_NAME:
				ok = m_isRename;
				eventDescription = "File rename old name";
				break;
			case FILE_ACTION_RENAMED_NEW_NAME:
				ok = m_isRename;
				eventDescription = "File rename new name";
				break;
			default:
				eventDescription = "Unknown event";
				break;
			}
			if (ok)
			{
				QString fileName = QString::fromWCharArray(info->FileName, info->FileNameLength / sizeof(WCHAR));
				if (shouldMonitorFile(fileName))
				{
					QString eventInfo = formatEventInfo(eventDescription, fileName);
					QFile logFile(m_saveLogFile);
					if (logFile.exists())
					{
						if (logFile.open(QIODevice::Append | QIODevice::Text))
						{
							QTextStream out(&logFile);
							out << eventInfo << "\n";
							logFile.close();
						}
					}
					emit fileEvent(eventInfo);
				}
			}
			
			if (info->NextEntryOffset == 0)
				break;

			info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(reinterpret_cast<BYTE*>(info) + info->NextEntryOffset);
		} while (true);
	}
}

QString KFileMonitor::formatEventInfo(const QString& eventDescription, const QString& fileName)
{
	QString timestamp = QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss");
	emit fileInsert(timestamp,eventDescription, m_directoryPath + fileName);
	return QString("%1: %2, file name: %3").arg(timestamp, eventDescription, m_directoryPath + fileName);
}

void KFileMonitor::setFilterInSuffixes(const QSet<QString>& suffixes)
{
	m_filterInSuffixes = suffixes;
}

void KFileMonitor::setFilterExSuffixes(const QSet<QString>& suffixes)
{
	m_filterExSuffixes = suffixes;
}

bool KFileMonitor::shouldMonitorFile(const QString& fileName)
{
	QFileInfo fileInfo(fileName);
	if (m_filterInSuffixes.isEmpty())
	{
		if (m_filterExSuffixes.isEmpty())
		{
			return true;
		}
		return  !m_filterExSuffixes.contains(fileInfo.suffix().toLower());
	}
	return m_filterInSuffixes.contains(fileInfo.suffix().toLower());
}


void KFileMonitor::isSubFolders()
{
	m_isSubFolders = !m_isSubFolders;
}


void KFileMonitor::isCreate()
{
	m_isCreate = !m_isCreate;
}

void KFileMonitor::isDelete()
{
	m_isDelete = !m_isDelete;
}

void KFileMonitor::isEdit()
{
	m_isEdit = !m_isEdit;
}

void KFileMonitor::isRename()
{
	m_isRename = !m_isRename;
}

void KFileMonitor::saveConfig(const QString& configFilePath)
{
	QSettings settings(configFilePath, QSettings::IniFormat);
	settings.beginGroup("General");
	settings.setValue("directoryPath", m_directoryPath);
	settings.endGroup();

	settings.beginGroup("Settings");
	settings.setValue("isSubFolders", m_isSubFolders);
	settings.setValue("isCreate", m_isCreate);
	settings.setValue("isDelete", m_isDelete);
	settings.setValue("isEdit", m_isEdit);
	settings.setValue("isRename", m_isRename);
	settings.endGroup();

	settings.beginGroup("Filters");
	int index = 1;
	for (const QString& suffix : m_filterInSuffixes)
	{
		settings.setValue(QString("InSuffix%1").arg(index++), suffix);
	}
	index = 1;
	for (const QString& suffix : m_filterExSuffixes)
	{
		settings.setValue(QString("ExSuffix%1").arg(index++), suffix);
	}
	settings.endGroup();
}

void KFileMonitor::loadConfig(const QString& configFilePath)
{
	QSettings settings(configFilePath, QSettings::IniFormat);

	settings.beginGroup("General");
	m_directoryPath = settings.value("directoryPath").toString();
	settings.endGroup();

	settings.beginGroup("Settings");
	m_isSubFolders = settings.value("isSubFolders").toBool();
	m_isCreate = settings.value("isCreate").toBool();
	m_isDelete = settings.value("isDelete").toBool();
	m_isEdit = settings.value("isEdit").toBool();
	m_isRename = settings.value("isRename").toBool();
	settings.endGroup();

	settings.beginGroup("Filters");
	m_filterInSuffixes.clear();
	m_filterExSuffixes.clear();
	QStringList keys = settings.childKeys();
	for (const QString& key : keys)
	{
		if (key.startsWith("InSuffix"))
		{
			m_filterInSuffixes.insert(settings.value(key).toString());
		}
		else if (key.startsWith("ExSuffix"))
		{
			m_filterExSuffixes.insert(settings.value(key).toString());
		}
	}
	settings.endGroup();
}