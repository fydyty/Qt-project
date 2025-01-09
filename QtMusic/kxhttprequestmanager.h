#ifndef __KX_HTTP_REQUEST_MANAGER_H_
#define __KX_HTTP_REQUEST_MANAGER_H_

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>


class KxHttpRequestManager : public QObject
{
	Q_OBJECT

public:
	KxHttpRequestManager(QObject* parent = nullptr);
	~KxHttpRequestManager();
	void sendGetRequest(const QUrl& url);// ���� get ����

signals:
	void requestFinished(QNetworkReply* reply);

private:
	QNetworkAccessManager m_manager;
};

#endif