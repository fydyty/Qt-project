#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QIcon>
#include <QModelIndex>
#include <QUrl>
#include <QBuffer>
#include <QListWidgetItem>
#include "mainwindow.h"
class MediaPlayer : public QWidget
{
    Q_OBJECT

public:
    MediaPlayer(QWidget *parent = nullptr);
    ~MediaPlayer();

    ///自定义槽函数
    void onDurationChanged (qint64 duration) ;
    void onPositionChanged(qint64 position) ;
    void on_pushButton_AddSong_notplay(QUrl& qurl);
    void on_previous_music();
    void on_next_music();
    void playMusic();
    void on_listWidget_doubleClicked(const QModelIndex &index);
    void on_slider_Volume_valueChanged(int value);
signals:
    void transDuration(qint64 duration, QString& durationStr);
    void transPosition(int currentTime);
    void lastOrNextMusic(int index);
private:
    QMediaPlayer *mPlayer = nullptr;  //播放器
    QMediaPlaylist *mPlayList = nullptr; //播放器列表

    QString mDurationTime="00:00"; //总长度
    QString mPositionTime = "00:00";//当前播放到的位置
};
