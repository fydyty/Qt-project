#include "kxresponsehandler.h"

#include <QJsonDocument>
#include <QDebug>

KxResponseHandler::KxResponseHandler(QObject* parent)
	: QObject(parent)
{
}

KxResponseHandler::~KxResponseHandler()
{
}

void KxResponseHandler::onHandleResponse(QNetworkReply* reply)
{
	if (reply->error() == QNetworkReply::NoError)
	{
		QByteArray responseData = reply->readAll();// ��ȡӦ������
		if (responseData.size() >= 2 && responseData[0] == (char)0xFF && responseData[1] == (char)0xD8)
		{
			// JPG �ļ�
			emit jpgReady(responseData);
		}
		else if (responseData.size() >= 3 && responseData[0] == 'I' && responseData[1] == 'D' && responseData[2] == '3')
		{
			// MP3 �ļ�
			emit mp3Ready(reply->url());
		}
		else if (responseData.size() >= 4 && responseData[0] == '{' && responseData[responseData.size() - 1] == '}')
		{
			// JSON �ļ�
			emit jsonReady(responseData);
		}
		else
		{
			// Ĭ��Ϊ TXT �ļ�
			emit lyricReady(responseData);
		}
	}
	else
	{
		qDebug() << "Error : " << reply->errorString();
	}

}