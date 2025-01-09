#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QTimer>
#include <QThread>
#include "kxtablemodel.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class KturnTable;
class KxHttpRequestManager;
class KxResponseHandler;
class MediaPlayer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();
    void initTableView();
    void initOther();
    void onSearchMusic();
    void initLyric(QByteArray &lyric);
    void onLyricScroll();
    void displayVslider();
    void updateMessage(const QModelIndex &index);
    void updateMessageInt(const int index);
    void updateCurrentTime(int currentTime);
    void changePlayIcon();
    void changeSliderIcon();
    void playOrStop();
    void playOrStopOpen();
    void controlLyric(int currentTime);
signals:
    void searchMusic(QString& musicname);
   
private:
    Ui::MainWindow* ui;
    KturnTable* m_turnTable;
    KxHttpRequestManager* m_requestManager;
    KxResponseHandler* m_responseHandler;
    KxTableMode* m_musicModel;
    MediaPlayer* m_kplay;
	int m_nowWord = 0;
    QTimer* m_lyricTimer;
    QMap<int, QString> lrcMap;
	int m_next = 0;
    bool m_work = false;
};
#endif // MAINWINDOW_H
