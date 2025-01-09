#ifndef __KXRESPONSEHANDLER_H__
#define __KXRESPONSEHANDLER_H__

#include <QObject>
#include <QNetworkReply>

class QNetworkReply;

class KxResponseHandler : public QObject
{
	Q_OBJECT

public:
	KxResponseHandler(QObject* parent = nullptr);
	~KxResponseHandler();
	void onHandleResponse(QNetworkReply* reply);// 处理 http 应答数据
signals:
	void jsonReady(QByteArray& data);
	void jpgReady(QByteArray& data);
	void mp3Ready(QUrl& qurl);
	void lyricReady(QByteArray& data);
};

#endif