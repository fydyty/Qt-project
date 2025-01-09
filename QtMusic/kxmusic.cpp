#include "kxmusic.h"

KxMusic::KxMusic(int id,QString musicName,QString albumName,int duration,QString path,QString mp3,QString lyric,QString img)
	:m_id(id)
	,m_musicName(musicName)
	,m_albumName(albumName)
	,m_duration(duration)
	,m_path(path)
	,m_mp3(mp3)
	,m_lyric(lyric)
	,m_img(img)
{
}

int KxMusic::getId() const
{
	return m_id;
}

QString KxMusic::getMusicName() const 
{
	return m_musicName;
}

QString KxMusic::getAlbumName() const
{
	return m_albumName;
}

int KxMusic::getDuration() const
{
	return m_duration;
}

QString KxMusic::getPath() const
{
	return m_path;
}

QString KxMusic::getMp3() const
{
	return m_mp3;
}

QString KxMusic::getLyric() const
{
	return m_lyric;
}

QString KxMusic::getImg() const
{
	return m_img;
}

void KxMusic::setId(int id)
{
	m_id = id;
}

void KxMusic::setMusicName(QString& musicName)
{
	m_musicName = musicName;
}

void KxMusic::setAlbumName(QString& albumName)
{
	m_albumName = albumName;
}

void KxMusic::setDuration(int duration)
{
	m_duration = duration;
}


