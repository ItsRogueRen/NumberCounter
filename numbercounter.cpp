#include "numbercounter.h"
#include <QPainter>
#include <QScrollBar>
#include <QFileDialog>
#include <QSettings>
#include <QRegularExpression>

NumberCounter::NumberCounter(QWidget *parent)
    : QWidget(parent)
{
	this->LoadUserData();

	if (!m_stUserData.blDark)
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

NumberCounter::~NumberCounter()
{
	this->SaveUserData();
	this->SaveData2File();
}

void NumberCounter::Init()
{
    this->InitCtrl();
    this->InitVar();
}

void NumberCounter::MenuBarStyle(QMenuBar* pMenuBar)
{
	QString strMenubarStyle = "QMenuBar { background-color: %2; color :%1; border-top : 1px solid %1 ; padding-top: 4px ; border-bottom : 1px solid %1  }  QMenuBar::item:selected {  background-color: %2; color :%1;  }";
	strMenubarStyle = strMenubarStyle.arg(m_colorForeground.name()).arg(m_colorBackground.name());
	pMenuBar->setStyleSheet(strMenubarStyle);
}

void NumberCounter::MenuStyle(QMenu* pMenu)
{
	QString strMenuStyle = "QMenu { background-color: %2;  margin-left: 0px ; border : 1px solid %1}" \
		"QMenu::separator {background-color: %1 ; height: 1px;}" \
		"QMenu::item { color: %1 ; background-color: %2;}" \
		;
	strMenuStyle = strMenuStyle.arg(m_colorForeground.name()).arg(m_colorBackground.name());
	pMenu->setStyleSheet(strMenuStyle);
}

void NumberCounter::InitCtrl()
{
	m_pStandardModel = new QStandardItemModel(this);
	m_pTreeView = ui.treeView;
	StyleCommonTreeView(m_pTreeView, m_pStandardModel);
	this->TreeviewStyle(m_pTreeView);

	QFont ftMenu = qApp->font();
	ftMenu.setPixelSize(14);
	m_pMenuBar = new QMenuBar(ui.frame_menubar);
	this->MenuBarStyle(m_pMenuBar);

	m_pMenuBar->setFont(ftMenu);
	m_pMenuFile = m_pMenuBar->addMenu("File");
	this->MenuStyle(m_pMenuFile);

	QObject::connect(m_pMenuFile, &QMenu::triggered, [this](QAction* pAct) {
		
		if (pAct == m_pActionDark)
		{
			if (m_stUserData.blDark) 
			{
				m_pActionDark->setChecked(true);
				return;
			}

			m_stUserData.blDark = true;
			m_pActionLight->setChecked(false);
			this->ChangeTheme();

		}
		else if (pAct == m_pActionLight)
		{
			if (!m_stUserData.blDark)
			{
				m_pActionLight->setChecked(true);
				return;
			}

			m_pActionDark->setChecked(false);
			m_stUserData.blDark = false;
			this->ChangeTheme();

		}
		else if (pAct == m_pActionNewCounter)
		{
			this->AddNewCounter("");
			this->m_listItemWidgets.back()->SetUnsigned(m_pActionToggleUnsigned->isChecked());
		}
		else if (pAct == m_pActionToggleUnsigned)
		{
			for (auto &item : this->m_listItemWidgets)
			{
				item->SetUnsigned(m_pActionToggleUnsigned->isChecked());
			}
		}
		else if (pAct == m_pActionQuit)
		{
			qApp->quit();
		}
		
		});

	QAction* pAction = nullptr;
	pAction = new QAction("Dark");
	m_pActionDark = pAction;
	pAction->setFont(ftMenu);
	pAction->setCheckable(true);
	m_pMenuFile->addAction(pAction);

	pAction = new QAction("Light");
	m_pActionLight = pAction;
	pAction->setFont(ftMenu);
	pAction->setCheckable(true);
	m_pMenuFile->addAction(pAction);

	m_pMenuFile->addSeparator();

	pAction = new QAction("New Counter");
	m_pActionNewCounter = pAction;
	pAction->setFont(ftMenu);
	pAction->setCheckable(false);
	m_pMenuFile->addAction(pAction);

	pAction = new QAction("Set Positive Only");
	m_pActionToggleUnsigned = pAction;
	pAction->setFont(ftMenu);
	pAction->setCheckable(true);
	m_pMenuFile->addAction(pAction);

	pAction = new QAction("Quit");
	m_pActionQuit = pAction;
	pAction->setFont(ftMenu);
	pAction->setCheckable(false);
	m_pMenuFile->addAction(pAction);

	this->ButCommonSet(ui.pushButton_change);
	this->ButStyle(ui.pushButton_change);

	QObject::connect(ui.pushButton_change, &QPushButton::clicked, [this]() {
		
		QString selectedDirectory = QFileDialog::getExistingDirectory(nullptr, "Select Directory", m_stUserData.strLocation);
		if (!selectedDirectory.isEmpty()) {

			ui.label_location->setText(QDir::toNativeSeparators(selectedDirectory));
			m_stUserData.strLocation = selectedDirectory;
			for (auto it : m_listCounterFileNames)
			{
				QFile::remove(it);
			}

			m_listCounterFileNames.clear();

			for (auto it : m_listItemWidgets)
			{
				it->SaveVal2File();
			}

		}
		else {
	
		}		

		});

	QFont ftLabel = qApp->font();
	ftLabel.setBold(false);
	ftLabel.setPixelSize(14);
	ui.label_location->setFont(ftLabel);
	this->LabelStyle(ui.label_location);

}

void NumberCounter::InitVar()
{
	QWidget::setFixedWidth(400);
	ui.label_location->setText(QDir::toNativeSeparators(m_stUserData.strLocation));
	m_pActionDark->setChecked(m_stUserData.blDark);
	m_pActionLight->setChecked(!m_stUserData.blDark);
	this->LoadCounterData();
}

void NumberCounter::AddNewCounter(const QString& strFileName)
{
	int nRowCn = m_pStandardModel->rowCount();
	m_pStandardModel->insertRow(nRowCn);
	QStandardItem* pItem = nullptr;
	int nCol = 0;

	pItem = new QStandardItem();
	m_pStandardModel->setItem(nRowCn, nCol, pItem);
	NumberCounterItemWidget* pItemWidget = new NumberCounterItemWidget(strFileName,this);
	m_listItemWidgets.append(pItemWidget);
	m_pTreeView->setIndexWidget(m_pStandardModel->indexFromItem(pItem), pItemWidget);
}


void NumberCounter::mousePressEvent(QMouseEvent* event)
{
	QWidget::mousePressEvent(event);
}

void NumberCounter::resizeEvent(QResizeEvent* event)
{
	m_pMenuBar->resize(ui.frame_menubar->size() + QSize(-15,0));
	m_pMenuBar->move(0, 0);
	QWidget::resizeEvent(event);
}

void  NumberCounter::StyleCommonTreeView(QTreeView* pTreeView, QAbstractItemModel* pSourceModel)
{
	pSourceModel->setParent(pTreeView);
	pTreeView->setSortingEnabled(true);
	pTreeView->setSortingEnabled(false);
	pTreeView->setModel(pSourceModel);
	pTreeView->setAlternatingRowColors(false);
	pTreeView->setAnimated(true);
	pTreeView->setRootIsDecorated(false);
	pTreeView->setFocusPolicy(Qt::NoFocus);
	pTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	pTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
	pTreeView->setSelectionMode(QAbstractItemView::NoSelection);
	pTreeView->setUniformRowHeights(true);
	pTreeView->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
	pTreeView->setTextElideMode(Qt::ElideRight);
	pTreeView->setWordWrap(true);
	pTreeView->header()->setStretchLastSection(true);
	QHeaderView* pHeadView = pTreeView->header();
	pHeadView->setVisible(false);
	pHeadView->setDefaultAlignment(Qt::AlignCenter);
	pHeadView->setSectionsMovable(false);
	pHeadView->sectionResizeMode(QHeaderView::ResizeToContents);
	pHeadView->setSortIndicatorShown(true);
	pHeadView->setTextElideMode(Qt::ElideRight);
	pHeadView->setSectionResizeMode(QHeaderView::Stretch);
}

void NumberCounter::TreeviewStyle(QTreeView* pTreeview)
{
	QString strAppStyle = "  " \
		" QTreeView, QTreeView:disabled { border-left: 0px solid rgb(156,156,156);  border-top: 0px solid rgb(156,156,156); background-color: %2; } "\
		" QTreeView::item, QTreeView::item:disabled { height:40px; border-bottom: 0px solid rgb(213,213,213); border-right: 0px solid rgb(213,213,213);padding-bottom: 4px; padding-top: 4px;} " \
		" QTreeView::item:selected { border-radius: 0px; background : rgba(200,200,200,150); } " \
		" QTreeView::item:hover { border-radius: 0px; background-color : transparent;  }";
	strAppStyle = strAppStyle.arg(m_colorBackground.name());

	QString strVScrollBar = "" \
		" QScrollBar:vertical {width: 14px ;  margin:0px; background : %1; border-radius:0px; } " \
		" QScrollBar::sub-line:vertical { border:0px; } " \
		" QScrollBar::add-line:vertical { border:0px; } " \
		" QScrollBar::sub-page:vertical { background-color: %1; border-top-left-radius:0px; border-top-right-radius:0px; } " \
		" QScrollBar::add-page:vertical { background-color: %1; border-bottom-left-radius:0px; border-bottom-right-radius: 0px; } " \
		" QScrollBar::handle:vertical {min-height:36px; background-color:%2; border-radius:0px; width:8px; } " ;
	strVScrollBar = strVScrollBar.arg(m_colorBackground.name()).arg(m_colorForeground.name());

	pTreeview->verticalScrollBar()->setStyleSheet(strVScrollBar);
	pTreeview->setStyleSheet(strAppStyle);
}

void NumberCounter::LoadUserData()
{
	QString strAppDir = QApplication::applicationDirPath();
	if (strAppDir.right(1) != QDir::separator())
	{
		strAppDir += QDir::separator();
	}
	QString strConfigFile = QString("%1config.ini").arg(strAppDir);

	if (!QFile::exists(strConfigFile))
	{
		QSettings settins(strConfigFile, QSettings::IniFormat, this);
		settins.setValue("app/mode", m_stUserData.blDark);
		settins.setValue("app/location", m_stUserData.strLocation);
		settins.sync();
	}
	else
	{
		QSettings settins(strConfigFile, QSettings::IniFormat, this);
		m_stUserData.blDark = settins.value("app/mode").toBool();
		m_stUserData.strLocation = settins.value("app/location").toString();
	}

	QDir dir(m_stUserData.strLocation);
	if (!dir.exists())
	{
		dir.mkdir(m_stUserData.strLocation);
	}
}

void NumberCounter::SaveUserData()
{
	QString strAppDir = QApplication::applicationDirPath();
	if (strAppDir.right(1) != QDir::separator())
	{
		strAppDir += QDir::separator();
	}
	QString strConfigFile = QString("%1config.ini").arg(strAppDir);
	QSettings settins(strConfigFile, QSettings::IniFormat, this);
	settins.setValue("app/mode", m_stUserData.blDark);
	settins.setValue("app/location", m_stUserData.strLocation);
	settins.sync();
}

void NumberCounter::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


void NumberCounter::ChangeTheme()
{
	QColor colorTmp = m_colorForeground;
	m_colorForeground = m_colorBackground;
	m_colorBackground = colorTmp;

	QString strWidgetStyle = " QWidget { border: none; background: %1; }";
	strWidgetStyle = strWidgetStyle.arg(m_colorBackground.name());
	this->setStyleSheet(strWidgetStyle);

	this->MenuBarStyle(m_pMenuBar);
	this->MenuStyle(m_pMenuFile);
	this->ButStyle(ui.pushButton_change);
	this->TreeviewStyle(m_pTreeView);
	this->LabelStyle(ui.label_location);

	for (auto it : m_listItemWidgets)
	{
		it->ChangeTheme();
	}


}

void NumberCounter::ButStyle(QPushButton* pBut)
{
	QString strButStyle = "QPushButton {color:%1; border-radius: 0px; border: 1px solid %2;;  background-color : %3 ;}";
	strButStyle = strButStyle.arg(m_colorForeground.name()).arg(m_colorForeground.name()).arg(m_colorBackground.name());
	pBut->setStyleSheet(strButStyle);
}

void NumberCounter::ButCommonSet(QPushButton* pBut)
{
	pBut->setFixedSize(QSize(80, 32));
	QFont ftBut = qApp->font();
	ftBut.setBold(true);
	ftBut.setPixelSize(18);
	pBut->setFont(ftBut);
	pBut->setFocusPolicy(Qt::NoFocus);
}

void NumberCounter::LabelStyle(QLabel* pLabel)
{
	QString strLabelStyle = "QLabel {color:%1; border-radius: 0px; border:1px solid %2; background-color : %3 ;}";

	strLabelStyle = strLabelStyle.arg(m_colorForeground.name()).arg(m_colorForeground.name()).arg(m_colorBackground.name());

	pLabel->setStyleSheet(strLabelStyle);
}

void NumberCounter::SaveData2File()
{
	for (auto it : m_listItemWidgets)
	{
		it->SaveVal2File();
	}
}

void NumberCounter::LoadCounterData()
{
	QString strDir = m_stUserData.strLocation;
	if (strDir.right(1) != QDir::separator())
	{
		strDir += QDir::separator();
	}
	QStringList filters;
	filters << "*.txt" << "*.TXT";
	QDir directory(m_stUserData.strLocation);
	directory.setNameFilters(filters);
	QStringList files = directory.entryList(QDir::Files | QDir::NoDotAndDotDot);
	for (auto it : files)
	{
		QString fileFullName = QString("%1%2").arg(strDir).arg(it);
		this->AddNewCounter(fileFullName);
	}
}

bool NumberCounter::CheckNewFileName(const QString& strNewName, const QString& strOldName, QString& strErrMsg)
{
	bool blRet = true;

	if (strNewName.isEmpty())
	{
		strErrMsg = "The file name cannot be empty.";
		blRet = false;
		return blRet;
	}

	QString strFileDir = m_stUserData.strLocation;
	if (strFileDir.right(1) != QDir::separator())
	{
		strFileDir += QDir::separator();
	}
	QString strNewFileNameExe = QString("%1%2.txt").arg(strFileDir).arg(strNewName);
	QString strOldFileNameExe = QString("%1%2.txt").arg(strFileDir).arg(strOldName);
	if (m_listCounterFileNames.contains(strNewFileNameExe))
	{
		strErrMsg = "The filename is already in use.";
		blRet = false;
		return blRet;
	}

	QRegularExpression regex("^[a-zA-Z0-9_-]+[a-zA-Z0-9_-]*$");
	QRegularExpressionMatch match = regex.match(strNewName);
	if (match.hasMatch() && match.captured(0) == strNewName) {

	}
	else
	{
		strErrMsg = "The filename is not a valid name.";
		blRet = false;
		return blRet;
	}

	QFile::remove(strOldFileNameExe);
	m_listCounterFileNames.removeOne(strOldFileNameExe);
	m_listCounterFileNames.append(strNewFileNameExe);
	return blRet;
}

void NumberCounter::AddCounterFileName(const QString& strFileName)
{
	if (!m_listCounterFileNames.contains(strFileName))
	{
		m_listCounterFileNames.append(strFileName);
	}
}
