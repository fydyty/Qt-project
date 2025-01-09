#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "kturntable.h"
#include "kxhttprequestmanager.h"
#include "kxresponsehandler.h"
#include "kxmediaplayer.h"
#include "kxmusic.h"
#include <QScrollBar>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_turnTable(nullptr)
    , m_requestManager(nullptr)
    , m_responseHandler(nullptr)
    , m_musicModel(nullptr)
	, m_kplay(nullptr)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
	delete m_kplay;
	delete m_lyricTimer;
	delete m_musicModel;
	delete m_requestManager;
	delete m_turnTable;
	delete m_responseHandler;
}

void MainWindow::init()
{
    initTableView();
	initOther();
	m_lyricTimer = new QTimer();
    m_turnTable = new KturnTable(ui->m_displayWidget);
    m_requestManager = new KxHttpRequestManager(this);
    m_responseHandler = new KxResponseHandler(this);
	m_kplay = new MediaPlayer(this);
    (void)connect(ui->m_play, &QPushButton::clicked, this, &MainWindow::playOrStop);
    (void)connect(ui->m_play, &QPushButton::clicked, this, &MainWindow::changePlayIcon);
    (void)connect(ui->m_play, &QPushButton::clicked, m_kplay, &MediaPlayer::playMusic);

	(void)connect(ui->m_lastMusic, &QPushButton::clicked, m_kplay, &MediaPlayer::on_previous_music);
	(void)connect(ui->m_nextMusic, &QPushButton::clicked, m_kplay, &MediaPlayer::on_next_music);
	(void)connect(m_kplay, &MediaPlayer::lastOrNextMusic, this, &MainWindow::updateMessageInt);
	(void)connect(m_kplay, &MediaPlayer::lastOrNextMusic, this, &MainWindow::playOrStopOpen);
	(void)connect(m_kplay, &MediaPlayer::lastOrNextMusic, m_musicModel, &KxTableMode::takeMusicInt);

    (void)connect(ui->m_serachButton, &QPushButton::clicked, this, &MainWindow::onSearchMusic);
    (void)connect(this, &MainWindow::searchMusic, m_musicModel, &KxTableMode::searchMusicJson);

    (void)connect(ui->m_musicTable, &QTableView::doubleClicked, m_musicModel, &KxTableMode::takeMusicIndex);
    (void)connect(ui->m_musicTable, &QTableView::doubleClicked, this, &MainWindow::playOrStopOpen);
	(void)connect(ui->m_musicTable, &QTableView::doubleClicked, this, &MainWindow::updateMessage);
	(void)connect(ui->m_musicTable, &QTableView::doubleClicked, m_kplay, &MediaPlayer::on_listWidget_doubleClicked);
	(void)connect(m_kplay, &MediaPlayer::transPosition, this, &MainWindow::updateCurrentTime);
	(void)connect(m_kplay, &MediaPlayer::transPosition, this, &MainWindow::controlLyric);

	(void)connect(m_musicModel->getRespon(), &KxResponseHandler::jpgReady, m_turnTable, &KturnTable::setImg);
	(void)connect(m_musicModel->getRespon(), &KxResponseHandler::lyricReady, this, &MainWindow::initLyric);
	(void)connect(m_musicModel, &KxTableMode::mp3Ready, m_kplay, &MediaPlayer::on_pushButton_AddSong_notplay);

	(void)connect(ui->m_adjustVoice, &QPushButton::clicked, this, &MainWindow::displayVslider);
	(void)connect(ui->m_verticalSlider, &QSlider::valueChanged, m_kplay, &MediaPlayer::on_slider_Volume_valueChanged);
	(void)connect(ui->m_verticalSlider, &QSlider::valueChanged, this, &MainWindow::changeSliderIcon);
}


void MainWindow::initTableView()
{
    m_musicModel = new KxTableMode(this);
	ui->m_musicTable->setModel(m_musicModel);
	ui->m_musicTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->m_musicTable->verticalHeader()->hide();
	ui->m_musicTable->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void MainWindow::initOther()
{
	ui->m_verticalSlider->hide();
	ui->m_verticalSlider->setMinimum(0);
	ui->m_verticalSlider->setMaximum(100);
	ui->m_verticalSlider->setValue(50);
}

void MainWindow::onSearchMusic()
{
    QString name = ui->m_searchMusic->text();
    emit searchMusic(name);
}

void MainWindow::initLyric(QByteArray& lyric)
{
	ui->m_lyricList->clear();
	lrcMap.clear();
	m_next = 0;
	m_nowWord = 0;
	QString lyricMusic = QString::fromUtf8(lyric);
	QStringList lyricList = lyricMusic.split("\\r\\n");
	for (int i = 0; i < lyricList.size() - 1; i++)
	{
		QString ss1 = lyricList.at(i);
		QRegExp ipRegExp = QRegExp("\\[\\d\\d\\S\\d\\d\\S\\d\\d\\]");
		bool match = ipRegExp.indexIn(ss1);
		if (match == false)
		{
			int s_1 = ss1.mid(1, 2).toInt();      //分
			int s_2 = ss1.mid(4, 2).toInt();      //秒
			int s_3 = ss1.mid(7, 2).toInt();      //厘秒
			int s_count = (s_1 * 60 + s_2) * 1000 + s_3*10;   
			int lrctime = s_count;
			QString lrcstr = ss1.mid(10);
			lrcMap.insert(lrctime, lrcstr);
		}
	}
	
	for (int i=0; i < lrcMap.size(); i++)
	{
		QMap<int, QString>::iterator it = lrcMap.begin() + i;
		QString word = it.value();
		ui->m_lyricList->addItem(word);
	}
	for (int i = 0; i < ui->m_lyricList->count(); ++i) {
		QListWidgetItem* item = ui->m_lyricList->item(i);
		item->setTextAlignment(Qt::AlignCenter);
		if (i == m_nowWord - 1) {
			item->setFont(QFont("Arial", 8, QFont::Normal)); // 设置为正常字体
		}
	}
	
}

void MainWindow::onLyricScroll()
{
	ui->m_lyricList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	if (m_nowWord >= ui->m_lyricList->count()) {
		m_nowWord = 0;
	}
	ui->m_lyricList->setCurrentRow(m_nowWord++);
	// 计算居中显示的位置
	int scrollToValue = ui->m_lyricList->verticalScrollBar()->value();
	int itemHeight = ui->m_lyricList->visualItemRect(ui->m_lyricList->item(0)).height();
	int visibleItems = ui->m_lyricList->viewport()->height() / itemHeight;
	int centerIndex = m_nowWord - visibleItems / 2;
	if (centerIndex < 0) {
		centerIndex = 0;
	}
	else if (centerIndex + visibleItems > ui->m_lyricList->count()) {
		centerIndex = ui->m_lyricList->count() - visibleItems;
	}

	ui->m_lyricList->verticalScrollBar()->setValue(centerIndex);

	for (int i = 0; i < ui->m_lyricList->count(); ++i) {
		QListWidgetItem* item = ui->m_lyricList->item(i);
		item->setTextAlignment(Qt::AlignCenter);
		if (i == m_nowWord - 1) {
			item->setFont(QFont("Arial", 10, QFont::Bold)); // 设置为黑体
		}
		else {
			item->setFont(QFont("Arial", 8, QFont::Normal)); // 设置为正常字体
		}
	}
}

void MainWindow::controlLyric(int currentTime)
{
	for (; m_nowWord < ui->m_lyricList->count(); )
	{
		QMap<int, QString>::iterator it = lrcMap.begin() + m_nowWord;
		m_next = it.key();
		if (m_next <= currentTime)
		{
			onLyricScroll();
		}
		else
		{
			m_nowWord--;
			onLyricScroll();
			break;
		}
	}
}

void MainWindow::displayVslider()
{
	if (ui->m_verticalSlider->isHidden())
		ui->m_verticalSlider->show();
	else
		ui->m_verticalSlider->hide();
}

void MainWindow::updateMessage(const QModelIndex& index)
{
	QVector<KxMusic> musicVector = m_musicModel->getVector();
	const KxMusic music = musicVector.at(index.row());
	QString name = music.getMusicName();
	int duration = music.getDuration();

	int secs = duration; //秒
	int mins = secs / 60; //分钟
	secs = secs % 60; //余数秒
	QString mDurationTime = QString("%1:%2").arg(QString::number(mins), 2, '0').arg(QString::number(secs), 2, '0');
	qDebug() << mDurationTime;
	ui->m_totalTime->setText(mDurationTime);
	ui->m_musicName->setText(name);
	ui->m_horizontalSlider->setMaximum(duration*1000);
}

void MainWindow::updateMessageInt(const int index)
{
	QVector<KxMusic> musicVector = m_musicModel->getVector();
	const KxMusic music = musicVector.at(index);
	QString name = music.getMusicName();
	int duration = music.getDuration();

	int secs = duration; //秒
	int mins = secs / 60; //分钟
	secs = secs % 60; //余数秒
	QString mDurationTime = QString("%1:%2").arg(QString::number(mins), 2, '0').arg(QString::number(secs), 2, '0');
	qDebug() << mDurationTime;
	ui->m_totalTime->setText(mDurationTime);
	ui->m_musicName->setText(name);
	ui->m_horizontalSlider->setMaximum(duration * 1000);
}

void MainWindow::updateCurrentTime(int currentTime)
{
	int secs = currentTime / 1000; //秒
	int mins = secs / 60; //分钟
	secs = secs % 60;//余数秒

	QString position = QString("%1:%2").arg(QString::number(mins), 2, '0').arg(QString::number(secs), 2, '0');
	ui->m_horizontalSlider->setValue(currentTime);
	ui->m_currentTime->setText(position);
}

void MainWindow::changePlayIcon()
{
	if (!m_work)
	{
		ui->m_play->setStyleSheet("QPushButton{image: url(:/image/play.png);background-color: rgb(255, 255, 255);"
			"border:1px solid;border-radius:10px;border-color: rgb(129, 129, 129);}"
			"QPushButton:hover{background-color: lightgray;}"
			"QPushButton:pressed{background-color: gray;}");
	}
	else
	{
		ui->m_play->setStyleSheet("QPushButton{image: url(:/image/pause.png);background-color:rgb(255, 255, 255);"
			"border:1px solid;border-radius:10px;border-color: rgb(129, 129, 129);}"
			"QPushButton:hover{background-color: lightgray;}"
			"QPushButton:pressed{background-color: gray;}");
	}
}

void MainWindow::changeSliderIcon()
{
	if (ui->m_verticalSlider->value() == 0)
	{
		ui->m_adjustVoice->setStyleSheet("QPushButton{image: url(:/image/closevoice.png);background-color: rgb(255, 255, 255);"
			"border:1px solid;border-radius:10px;border-color: rgb(129, 129, 129);}"
			"QPushButton:hover{background-color: lightgray;}"
			"QPushButton:pressed{background-color: gray;}");
	}
	else
	{
		ui->m_adjustVoice->setStyleSheet("QPushButton{image: url(:/image/addvoice.png);background-color: rgb(255, 255, 255);"
			"border:1px solid;border-radius:10px;border-color: rgb(129, 129, 129);}"
			"QPushButton:hover{background-color: lightgray;}"
			"QPushButton:pressed{background-color: gray;}");
	}
}

void MainWindow::playOrStop()
{
	m_work = !m_work;
	changePlayIcon();
	m_turnTable->onStartorStopRote();
}

void MainWindow::playOrStopOpen()
{
	m_work = true;
	changePlayIcon();
	m_turnTable->onStartorStopRoteOpen();
}




