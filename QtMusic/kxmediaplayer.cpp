
#include "kxmediaplayer.h"
MediaPlayer::MediaPlayer(QWidget *parent)
    : QWidget(parent)
    
{
    mPlayer = new QMediaPlayer(this); ///播放器
    mPlayList = new QMediaPlaylist(this); ///播放器列表
    mPlayList->setPlaybackMode(QMediaPlaylist::Loop); ///设置为循环播放模式
    mPlayer->setPlaylist(mPlayList);
    mPlayer->setVolume(50);

    (void)connect(mPlayer, &QMediaPlayer::positionChanged, this, &MediaPlayer::onPositionChanged);
    (void)connect(mPlayer, &QMediaPlayer::durationChanged, this, &MediaPlayer::onDurationChanged);
}
MediaPlayer::~MediaPlayer()
{
	delete mPlayer;
	delete mPlayList;
}


void MediaPlayer::onDurationChanged(qint64 duration)
{
    ///文件时长变化，更新进度显示
    int secs=duration; //秒
    int mins=secs/60; //分钟
    secs=secs%60; //余数秒

    mDurationTime = QString("%1:%2").arg(QString::number(mins), 2, '0').arg(QString::number(secs), 2,  '0');
    qDebug() << mDurationTime;
    emit transDuration(duration,mDurationTime);
}

void MediaPlayer::onPositionChanged(qint64 position)
{
    ///当前文件播放位置变化，更新进度显示.
    //if (ui->sliderPosition->isSliderDown()) 
        //return;
    int secs=position/1000; //秒
    int mins=secs/60; //分钟
    secs=secs%60;//余数秒

    mPositionTime = QString("%1:%2").arg(QString::number(mins),2,'0').arg(QString::number(secs), 2, '0');
    emit transPosition(position);
   
}

void MediaPlayer::on_pushButton_AddSong_notplay(QUrl& qurl)
{
	QMediaContent content(qurl);
	mPlayList->addMedia(content);
}


void MediaPlayer::on_listWidget_doubleClicked(const QModelIndex &index)
{
    ///双击时切换播放文件
    int rowNo=index.row() ;
    mPlayList->setCurrentIndex (rowNo) ;
    mPlayer->play() ;
}

void MediaPlayer::on_slider_Volume_valueChanged(int value)
{
    ///调整音量
    mPlayer->setVolume(value) ;
}

void MediaPlayer::on_previous_music()
{
    int index = mPlayList->currentIndex();
    if (index != 0)
    {
        index--;
    }
    else
    {
        index = mPlayList->mediaCount() - 1;
    }
    mPlayer->pause();
    mPlayList->previous();
    mPlayer->play();
    emit lastOrNextMusic(index);
}

void MediaPlayer::on_next_music()
{
	int index = mPlayList->currentIndex();
	if (index != mPlayList->mediaCount() - 1)
	{
		index++;
	}
    else
    {
        index = 0;
    }
	mPlayer->pause();
	mPlayList->next();
    mPlayer->play();
	emit lastOrNextMusic(index);
}

void MediaPlayer::playMusic()
{
    if (mPlayer->state() == QMediaPlayer::PausedState)
    {
        mPlayer->play();
    }
    else
    {
        mPlayer->pause();
    }
	
}
