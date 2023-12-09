#pragma once

#include <QtWidgets/QWidget>
#include "ui_numbercounter.h"
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMenu>
#include <QMenuBar>
#include <QStandardPaths>
#include <QDir>
#include "numbercounteritemwidget.h"

typedef struct _stUserData
{
    _stUserData()
    {
        blDark = true;
		strLocation = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
		if (strLocation.right(1) != QDir::separator())
		{
            strLocation += QDir::separator();
		}
        QString strAppName = QApplication::applicationName();
        strLocation += strAppName;

    }
    bool blDark;
    QString strLocation;
} stUserData;

class NumberCounter : public QWidget
{
    Q_OBJECT

public:
    NumberCounter(QWidget *parent = nullptr);
    ~NumberCounter();

    stUserData m_stUserData;

    void AddCounterFileName(const QString& strFileName);
    bool CheckNewFileName(const QString& strNewName, const QString& strOldName, QString& strErrMsg);

protected:

    void Init();
    void InitCtrl();
    void InitVar();
    void StyleCommonTreeView(QTreeView* pTreeView, QAbstractItemModel* pSourceModel);
    void AddNewCounter(const QString& strFileName);

    void LoadUserData();
    void SaveUserData();
    void SaveData2File();
    void LoadCounterData();

    void ChangeTheme();

    void MenuBarStyle(QMenuBar* pMenuBar);
    void MenuStyle(QMenu* pMenu);
    void ButStyle(QPushButton* pBut);
    void ButCommonSet(QPushButton* pBut);
    void LabelStyle(QLabel* pLabel);
    void TreeviewStyle(QTreeView* pTreeview);

    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;


private:
    Ui::NumberCounterClass ui;

    QTreeView* m_pTreeView;
    QStandardItemModel* m_pStandardModel;

    QMenuBar* m_pMenuBar;
    QMenu* m_pMenuFile;
    QAction* m_pActionDark;
    QAction* m_pActionLight;
    QAction* m_pActionNewCounter;
    QAction* m_pActionToggleUnsigned;
    QAction* m_pActionQuit;

	QColor m_colorForeground = Qt::white;
	QColor m_colorBackground = Qt::black;

    QList<NumberCounterItemWidget*> m_listItemWidgets;
    QStringList m_listCounterFileNames;

};
