#include "kfilter.h"

KFilter::KFilter()
{
	m_layout = new QVBoxLayout(this);
	m_groupBox =new QGroupBox("Filter");
	m_groupLayout =new QVBoxLayout(m_groupBox);

	m_includeLabel = new QLabel("RegEx: Include: ");
	m_groupLayout->addWidget(m_includeLabel);
	
	m_include = new QLineEdit;
	m_include->setPlaceholderText("txt,docx,");
	m_groupLayout->addWidget(m_include);

	m_excludeLabel = new QLabel("RegEx: Exclude: ");
	m_groupLayout->addWidget(m_excludeLabel);

	m_exclude = new QLineEdit;
	m_exclude->setPlaceholderText("jpg,");
	m_groupLayout->addWidget(m_exclude);

	m_layout->addWidget(m_groupBox);
	setLayout(m_layout);

	(void)connect(m_include, &QLineEdit::editingFinished, this, &KFilter::takeSuffixes);
	(void)connect(m_exclude, &QLineEdit::editingFinished, this, &KFilter::takeSuffixes);
}

KFilter::~KFilter()
{
	delete m_layout;
	delete m_groupBox;
}

QSet<QString> KFilter::getInSuff()
{
	return m_inSuffixes;
}

QSet<QString> KFilter::getExSuff()
{
	return m_exSuffixes;
}

void KFilter::takeSuffixes()
{

	m_inSuffixes.clear();
	m_exSuffixes.clear();
	QString instr = m_include->text();
	QString temp = "";
	for (int i = 0; i < instr.size(); i++)
	{
		if (instr[i] == '\u0000')
			continue;
		if (instr[i] == ',')
		{
			m_inSuffixes.insert(temp);
			temp = "";
			continue;
		}
		temp += instr[i];
	}
	if (!temp.isEmpty())
		m_inSuffixes.insert(temp);
	temp = "";
	emit inSuffIsOk(m_inSuffixes);
	QString exstr = m_exclude->text();
	for (int i = 0; i < exstr.size(); i++)
	{
		if (exstr[i] == '\u0000')
			continue;
		if (exstr[i] == ',')
		{
			m_exSuffixes.insert(temp);
			temp = "";
			continue;
		}
		temp += exstr[i];
	}
	if (!temp.isEmpty())
		m_exSuffixes.insert(temp);

	emit exSuffIsOk(m_exSuffixes);
}

