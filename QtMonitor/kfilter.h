#ifndef KFILTER_H
#define KFILTER_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QSet>
#include <QDebug>
#include <QLineEdit>


class KFilter :public QWidget
{
	Q_OBJECT
	friend class MainWindow;
public:
	KFilter();
	~KFilter();
	QSet<QString> getInSuff();
	QSet<QString> getExSuff();

signals:
	void inSuffIsOk(const QSet<QString>& suff);
	void exSuffIsOk(const QSet<QString>& suff);
private:
	void takeSuffixes();

	QVBoxLayout *m_layout;
	QGroupBox *m_groupBox;
	QVBoxLayout *m_groupLayout;
	QLabel *m_includeLabel;
	QLabel *m_excludeLabel;
	QLineEdit* m_include;
	QLineEdit* m_exclude;

	QSet<QString> m_inSuffixes;
	QSet<QString> m_exSuffixes;
};

#endif // KFILTER_H