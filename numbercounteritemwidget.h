#pragma once

#include <QWidget>
#include "ui_numbercounteritemwidget.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
class NumberCounter;

class NumberCounterItemWidget : public QWidget
{
	Q_OBJECT

public:
	NumberCounterItemWidget(const QString& strFileName ,QWidget *parent = nullptr);
	~NumberCounterItemWidget();

	void ChangeTheme();
	void SaveVal2File();

	void SetUnsigned(bool isUnsigned);

protected:

	void Init();
	void InitCtrl();
	void InitVar();

	void ButStyle(QPushButton* pBut);
	void ButCommonSet(QPushButton* pBut);
	void LabelStyle(QLabel* pLabel);
	void FileNameLineEditStyle(FileNameLineEdit* pLineEdit);
	void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;

	void ValChanged(int nVal);

private:
	Ui::NumberCounterItemWidgetClass ui;

	FileNameLineEdit* m_pLineeditFileName;
	QPushButton* m_pButIncrease;
	QPushButton* m_pButDecrease;
	QLabel* m_pLabelDigit;

	QColor m_colorForeground = Qt::white;
	QColor m_colorBackground = Qt::black;
	NumberCounter* m_pMainWnd;
	QString m_strFileName;

	bool m_isUnsigned = false;
};
