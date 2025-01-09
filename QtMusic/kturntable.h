#include <QWidget>
#include <QTimer>
#include <QPushButton>
#include <QPainterPath>
#include <QDir>

class KturnTable: public QWidget
{
    Q_OBJECT

public:
    KturnTable(QWidget *parent = nullptr);
    ~KturnTable();

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void upDateRote();
    void onStartorStopRote();
    void onStartorStopRoteOpen();
    void setImg(QByteArray& data);
private:
    int m_Rote=0;
    QPushButton *m_pbtn=nullptr;
    bool m_bStart=false;
    bool m_img = false;
    QByteArray m_imageData;
    QTimer* m_pTimer;
};

