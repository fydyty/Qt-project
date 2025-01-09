#include "kxhttprequestmanager.h"

#include <QNetworkReply>

KxHttpRequestManager::KxHttpRequestManager(QObject* parent)
	: QObject(parent)
{
	(void)connect(&m_manager, &QNetworkAccessManager::finished, this, &KxHttpRequestManager::requestFinished);
}

KxHttpRequestManager::~KxHttpRequestManager()
{
}
// ∑¢ÀÕ get «Î«Û
void KxHttpRequestManager::sendGetRequest(const QUrl& url)
{
	QNetworkRequest request(url);
	m_manager.get(request);
}
