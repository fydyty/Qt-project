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
		QByteArray responseData = reply->readAll();// 读取应答数据
		if (responseData.size() >= 2 && responseData[0] == (char)0xFF && responseData[1] == (char)0xD8)
		{
			// JPG 文件
			emit jpgReady(responseData);
		}
		else if (responseData.size() >= 3 && responseData[0] == 'I' && responseData[1] == 'D' && responseData[2] == '3')
		{
			// MP3 文件
			emit mp3Ready(reply->url());
		}
		else if (responseData.size() >= 4 && responseData[0] == '{' && responseData[responseData.size() - 1] == '}')
		{
			// JSON 文件
			emit jsonReady(responseData);
		}
		else
		{
			// 默认为 TXT 文件
			emit lyricReady(responseData);
		}
	}
	else
	{
		qDebug() << "Error : " << reply->errorString();
	}

}