#ifndef __K_MAINWINDOW_H_
#define __K_MAINWINDOW_H_

#include <QtWidgets/QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QSvgWidget>
#include <QSvgGenerator>
#include <QPaintDevice>
#include <QBuffer>
#include "ksvgmainwindow.h"
#include "ktoolbar.h"
#include "kcanvasparamsbar.h"
#include "kcanvas.h"
#include "kshape.h"
#include "ui_kmainwindow.h"

class KMainWindow : public QWidget
{
    Q_OBJECT

public:
    KMainWindow(QWidget *parent = Q_NULLPTR);
    void initWindows();
    void initConnection();
  

private slots:
    void validateCanvasParams();
    void changeParams();
    void changePenWidth();
    void changePenStyle(int pos);
    void changePenColor();
    void changeBrushColor();
    void onNewFile();
    void openSvgFile();
    void saveSvgFile();
    void saveAsPng();
private:
    Ui::KMainWindowClass ui;
    QVBoxLayout *m_pVLayout;
    QHBoxLayout *m_pHLayout;
    KToolBar *m_pToolBar;
    KSvgMainWindow *m_pSvgMainWin;
    KCanvasParamsBar *m_pCanvasParamsBar;
    QMenuBar *m_pMenuBar;
    QMenu *m_pMenu;
};

#endif
