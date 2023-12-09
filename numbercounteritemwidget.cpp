#include "numbercounteritemwidget.h"
#include <QStyleOption>
#include <QPainter>
#include <QDir>
#include <QMessageBox>
#include "numbercounter.h"

NumberCounterItemWidget::NumberCounterItemWidget(const QString& strFileName,QWidget *parent)
	: QWidget(parent),m_pMainWnd((NumberCounter*)parent), m_strFileName(strFileName)
{
	if (!m_pMainWnd->m_stUserData.blDark)
	{
		m_colorForeground = Qt::black;
		m_colorBackground = Qt::white;
	}

	QString strWidgetStyle = " QWidget { border: none; background: %1; }";
	strWidgetStyle = strWidgetStyle.arg(m_colorBackground.name());
	this->setStyleSheet(strWidgetStyle);
	ui.setupUi(this);
	this->Init();
}

NumberCounterItemWidget::~NumberCounterItemWidget()
{

}

void NumberCounterItemWidget::Init()
{
	this->InitCtrl();
	this->InitVar();
}

void NumberCounterItemWidget::InitCtrl()
{
	m_pLineeditFileName = ui.lineEdit_filename;
	m_pLineeditFileName->setToolTip("Double click to edit the file name.");
	QFont ftLineEdit = qApp->font();
	ftLineEdit.setPixelSize(16);
	ftLineEdit.setBold(false);
	m_pLineeditFileName->setFont(ftLineEdit);
	this->FileNameLineEditStyle(m_pLineeditFileName);

	m_pButIncrease = ui.pushButton_Increase;
	this->ButCommonSet(m_pButIncrease);
	this->ButStyle(m_pButIncrease);

	m_pButDecrease = ui.pushButton_Decrease;
	this->ButCommonSet(m_pButDecrease);
	this->ButStyle(m_pButDecrease);

	m_pLabelDigit = ui.label;
	QFont ftLabel = qApp->font();
	ftLabel.setBold(true);
	ftLabel.setPixelSize(75);
	m_pLabelDigit->setFont(ftLabel);

	this->LabelStyle(m_pLabelDigit);


	QObject::connect(m_pButIncrease, &QPushButton::clicked, [this]() {
		this->ValChanged(1);

		});

	QObject::connect(m_pButDecrease, &QPushButton::clicked, [this]() {		
		this->ValChanged(-1);		
		});


	QObject::connect(m_pLineeditFileName, &FileNameLineEdit::SignalNameChanged, [this](const QString& strNewName, const QString& strOldName) {

		QString strErrMsg;
		if (!m_pMainWnd->CheckNewFileName(strNewName,strOldName,strErrMsg))
		{
			QMessageBox errorBox;
			errorBox.setIcon(QMessageBox::Critical);
			errorBox.setWindowTitle("Error");
			errorBox.setText(strErrMsg); 
			errorBox.setStandardButtons(QMessageBox::Ok);
			errorBox.exec();
			m_pLineeditFileName->setText(strOldName);

		}			
		
		});

}

void NumberCounterItemWidget::InitVar()
{
	if (m_strFileName.isEmpty())
	{
		QString strDefaultFileName = QString("Name_%1").arg(QString::number(10 + rand() % 10000));
		m_pLineeditFileName->setText(strDefaultFileName);
		m_pLabelDigit->setText(QString::number(0));

		QString strLoc = m_pMainWnd->m_stUserData.strLocation;
		if (strLoc.right(1) != QDir::separator())
		{
			strLoc += QDir::separator();
		}
		QString strFullFileName = QString("%1%2.txt").arg(strLoc).arg(strDefaultFileName);
		m_strFileName = strFullFileName;
	}
	else
	{
		QFile file(m_strFileName);
		if (file.open(QIODevice::ReadOnly))
		{
			QByteArray byteData = file.readAll();
			m_pLabelDigit->setText(byteData);
			file.close();
		}
		QFileInfo fileInfo(m_strFileName);
		m_pLineeditFileName->setText(fileInfo.baseName());
	}
	m_pMainWnd->AddCounterFileName(m_strFileName);
}


void NumberCounterItemWidget::ButStyle(QPushButton* pBut)
{
	QString strButStyle = "QPushButton {color:%1; border-radius: 12px; border: 4px solid %2;;  background-color : %3 ;}";

	strButStyle = strButStyle.arg(m_colorForeground.name()).arg(m_colorForeground.name()).arg(m_colorBackground.name());

	pBut->setStyleSheet(strButStyle);
}

void NumberCounterItemWidget::LabelStyle(QLabel* pLabel)
{
	QString strLabelStyle = "QLabel {color:%1; border-radius: 12px; border: 4px solid %2; background-color : %3 ;}";

	strLabelStyle = strLabelStyle.arg(m_colorForeground.name()).arg(m_colorForeground.name()).arg(m_colorBackground.name());

	pLabel->setStyleSheet(strLabelStyle);
}


void NumberCounterItemWidget::ButCommonSet(QPushButton* pBut)
{
	pBut->setFixedSize(QSize(80, 36));
	QFont ftBut = qApp->font();
	ftBut.setBold(true);
	ftBut.setPixelSize(50);
	pBut->setFont(ftBut);
	pBut->setFocusPolicy(Qt::NoFocus);
}

void NumberCounterItemWidget::ChangeTheme()
{
	QColor colorTmp = m_colorForeground;
	m_colorForeground = m_colorBackground;
	m_colorBackground = colorTmp;
	QString strWidgetStyle = " QWidget { border: none; background: %1; }";
	strWidgetStyle = strWidgetStyle.arg(m_colorBackground.name());
	this->setStyleSheet(strWidgetStyle);

	this->ButStyle(m_pButDecrease);
	this->ButStyle(m_pButIncrease);
	this->LabelStyle(m_pLabelDigit);
	this->FileNameLineEditStyle(m_pLineeditFileName);

}

void NumberCounterItemWidget::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void NumberCounterItemWidget::FileNameLineEditStyle(FileNameLineEdit* pLineEdit)
{
	QString strLineEditStyle = "QLineEdit { padding-left: 8px ; color:%1; border: none; background-color : %2 ;}";

	strLineEditStyle = strLineEditStyle.arg(m_colorForeground.name()).arg(m_colorBackground.name());

	pLineEdit->setStyleSheet(strLineEditStyle);
}

void NumberCounterItemWidget::ValChanged(int nVal)
{
	int nCurrVal = m_pLabelDigit->text().trimmed().toInt();
	nCurrVal += nVal;
	if (m_isUnsigned && nCurrVal < 0)
	{
		nCurrVal = 0;
	}
	m_pLabelDigit->setText(QString::number(nCurrVal));
	this->SaveVal2File();
}

void NumberCounterItemWidget::SaveVal2File()
{
	QString strFileName = m_pLineeditFileName->text().trimmed();
	int nVal = m_pLabelDigit->text().trimmed().toInt();

	QString strLoc = m_pMainWnd->m_stUserData.strLocation;
	if (strLoc.right(1) != QDir::separator())
	{
		strLoc += QDir::separator();
	}
	QString strFullFileName = QString("%1%2.txt").arg(strLoc).arg(strFileName);
	QFile file(strFullFileName);
	if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		file.write(QByteArray::number(nVal));
		file.close();
	}
	m_pMainWnd->AddCounterFileName(strFullFileName);

}

void NumberCounterItemWidget::SetUnsigned(bool isUnsigned)
{
	m_isUnsigned = isUnsigned;
}
