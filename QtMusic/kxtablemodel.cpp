#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include "kxtablemodel.h"
#include "kxhttprequestmanager.h"
#include "kxresponsehandler.h"
#include "kxmusic.h"

KxTableMode::KxTableMode(QObject* parent)
	: QAbstractTableModel(parent)
	,m_pHttpRequest(new KxHttpRequestManager(this))
	,m_pHttpRespHandle(new KxResponseHandler(this))
	,m_musicNum(0)
{
	url = "https://gitee.com/MarkYangUp/music/raw/master";
	(void)QObject::connect(m_pHttpRequest, &KxHttpRequestManager::requestFinished,m_pHttpRespHandle,&KxResponseHandler::onHandleResponse);
	(void)QObject::connect(m_pHttpRespHandle, &KxResponseHandler::jsonReady,this,&KxTableMode::updateData);
	
}

KxTableMode::~KxTableMode()
{
	delete m_pHttpRespHandle;
	delete m_pHttpRequest;
}

void KxTableMode::searchMusicJson(QString &musicName)
{

	QString temp = url + "/" + QUrl::toPercentEncoding(musicName) + "/music.json";
	QUrl qurl(temp);
	m_pHttpRequest->sendGetRequest(qurl);
}

void KxTableMode::searchMusic(QString &pathMusic)
{
	QString temp = url + pathMusic;
	QUrl qurl(temp);
	m_pHttpRequest->sendGetRequest(qurl);
}



KxMusic KxTableMode::parseJsonData(QByteArray& data)
{
	QJsonParseError parseError;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &parseError);
	if (parseError.error != QJsonParseError::NoError)
	{
		qWarning("Failed to parse JSON: %s", qPrintable(parseError.errorString()));
		return KxMusic(); 
	} 
	QJsonObject temp = jsonDoc.object();
	QJsonArray jsonArray= temp["list"].toArray();
	QJsonObject object = jsonArray.at(0).toObject();

	QString musicName = object["musicName"].toString();
	QString albumName = object["albumName"].toString();
	int duration = object["duration"].toInt();
	QString path = object["path"].toString();
	QString mp3 = object["mp3"].toString();
	QString lyric = object["lyric"].toString();
	QString img = object["img"].toString();

	m_musicNum += 1;
	KxMusic music(m_musicNum,musicName,albumName,duration,path,mp3,lyric,img);

	return music;
}

QVariant KxTableMode::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Horizontal)
	{
		switch (section)
		{
		case 0:
			return QString("NO.");
		case 1:
			return QString("MusicName");
		case 2:
			return QString("AlbumName");
		case 3:
			return QString("Duration");

		}
	}
	return QVariant();
}

int KxTableMode::rowCount(const QModelIndex& parent) const
{
	return  parent.isValid() ? 0 : m_musicVector.size();
}

int KxTableMode::columnCount(const QModelIndex& parent) const
{
	return parent.isValid() ? 0 : 4;
}

QVariant KxTableMode::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (index.row() >= m_musicVector.size() || index.row() < 0)
		return QVariant();

	if (role == Qt::DisplayRole)
	{
		const auto& music = m_musicVector.at(index.row());// 从容器中获取数据对象
		switch (index.column())
		{
		case 0:
			return music.getId();// 返回第0列数据
		case 1:
			return music.getMusicName();// 返回第1列数据
		case 2:
			return music.getAlbumName();// 返回第2列数据
		case 3:
			return music.getDuration();
		default:
			break;
		}

	}

	return QVariant();
}

void KxTableMode::updateData(QByteArray& data)
{ 
	beginResetModel();
	QByteArray jsondata = data;
	KxMusic music = parseJsonData(jsondata);

	QString path = url + QUrl::toPercentEncoding(music.getPath()) + "/" + music.getMp3();
	qDebug() << "update:" << path;
	emit mp3Ready(QUrl(path));
	m_musicVector.push_back(music);
	endResetModel();
}

void KxTableMode::takeMusicIndex(const QModelIndex& index)
{
	int row = index.row();
	KxMusic currentMusic = m_musicVector.at(row);
	QString path = QUrl::toPercentEncoding(currentMusic.getPath());
	QString lyric = path + "/" + currentMusic.getLyric();
	QString img = path + "/" + currentMusic.getImg();
	QString mp3 = path + "/" + currentMusic.getMp3();
	searchMusic(lyric);
	searchMusic(img);
}

void KxTableMode::takeMusicInt(const int index)
{
	int row = index;
	qDebug() << "row:" << row;
	KxMusic currentMusic = m_musicVector.at(row);
	QString path = currentMusic.getPath();
	QString lyric = path + "/" + currentMusic.getLyric();
	QString img = path + "/" + currentMusic.getImg();
	QString mp3 = path + "/" + currentMusic.getMp3();
	searchMusic(lyric);
	searchMusic(img);
}

QVector<KxMusic> KxTableMode::getVector()
{
	return m_musicVector;
}

KxResponseHandler* KxTableMode::getRespon()
{
	return m_pHttpRespHandle;
}

