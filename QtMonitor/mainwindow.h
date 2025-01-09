#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "kfilter.h"
#include "kfilemonitor.h"
#include "kdatabase.h"

#include <QMainWindow>
#include <QTreeView>
#include <QFileSystemModel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QDesktopWidget>
#include <QFile>
#include <QTextStream>
#include <QHeaderView>
#include <QStackedWidget>
#include <QDebug>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void addLog(const QString& eventInfo);
public slots:
	void onSaveConfigButtonClicked();
	void onLoadConfigButtonClicked();
    void updateUi();
    void on_minimizeButton_clicked();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void showMessage(const QString& eventInfo);
    void openFileDialog();
signals:
    void userHandle(const QString& time, const QString& action, const QString& object);
private:
    Ui::MainWindow *ui;
    QFileSystemModel* m_model;
    QTreeView* m_treeView;
    QLabel* m_selectPathLabel;
    KFilter* m_filter;

    KFileMonitor* m_fileMonitor;
    QPushButton* m_start;
    QPushButton* m_stop;
    QPushButton* m_saveConfigButton;
    QPushButton* m_loadConfigButton;
    QPushButton* m_minimize;
    QTextEdit* m_log;

    QCheckBox* m_subFolders;
    QCheckBox* m_fileCreate;
	QCheckBox* m_fileDelete;
	QCheckBox* m_fileChange;
    QCheckBox* m_fileRename;

    QVBoxLayout* m_layout;
    QHBoxLayout* m_saveLayout;
    QVBoxLayout* m_treeLayout;
    QHBoxLayout* m_buttonLayout;
    QGridLayout* m_eventLayout;

    QLabel* m_saveLogPath;
    QPushButton* m_saveLog;

    QStackedWidget* stackedWidget;

    QWidget* m_centralWidget;
    QString m_filePath;
    QSystemTrayIcon* m_trayIcon;
    KDataBase* m_dataBase;
    KDataBase* m_userBase;
};
#endif 
