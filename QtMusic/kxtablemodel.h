#ifndef __KX_TABLE_MODEL_H__
#define __KX_TABLE_MODEL_H__

#include <QAbstractTableModel>
#include <QVector>

class KxHttpRequestManager;
class KxResponseHandler;
class KxMusic;
class KxTableMode :public QAbstractTableModel
{
	Q_OBJECT
public:
	KxTableMode(QObject* parent = nullptr);
	~KxTableMode();

	void searchMusicJson(QString& musicName);
	void searchMusic(QString &pathMusic);
	KxMusic parseJsonData(QByteArray &data);
	// 返回表头表头数据
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override; // 返回模型的行数
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override; // 返回模型的列数
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	void updateData(QByteArray &data);
	void takeMusicIndex(const QModelIndex& index);
	void takeMusicInt(const int index);
	QVector<KxMusic> getVector();
	KxResponseHandler* getRespon();
signals:
	void mp3Ready(QUrl &qurl);

private:
	KxHttpRequestManager* m_pHttpRequest;
	KxResponseHandler* m_pHttpRespHandle;
	QVector<KxMusic> m_musicVector;
	int m_musicNum;
	QString url;
};

#endif
