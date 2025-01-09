#ifndef __KX_TODO_H__
#define __KX_TODO_H__

#include <QString>

class KxMusic
{
public:
	KxMusic() = default;
	KxMusic(int id,
		QString musicName,
		QString albumName,
		int duration,
		QString path,
		QString mp3,
		QString lyric,
		QString img);
	~KxMusic() = default;
	int getId() const;
	QString getMusicName() const;
	QString getAlbumName() const;
	int getDuration() const;
	QString getPath() const;
	QString getMp3() const;
	QString getLyric() const;
	QString getImg() const;
	void setId(int id);
	void setMusicName(QString &musicName);
	void setAlbumName(QString &albumName);
	void setDuration(int duration);
	
private:
	int m_id;
	QString m_musicName;
	QString m_albumName;
	int m_duration;
	QString m_path;
	QString m_mp3;
	QString m_lyric;
	QString m_img;
};

#endif
