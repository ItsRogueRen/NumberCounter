#pragma once
#include <QString>
#include <QLineEdit>

class FileNameLineEdit  : public QLineEdit
{
	Q_OBJECT

public:
	FileNameLineEdit(QWidget *parent = nullptr);
	~FileNameLineEdit();


protected:

	void InitLineEdit();
	void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
	void mouseDoubleClickEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void focusOutEvent(QFocusEvent* event) Q_DECL_OVERRIDE;

Q_SIGNALS:

	void SignalNameChanged(const QString& strNewName,const QString& strOldName);

private:
	QString m_strOldName;
	QString m_strNewName;

};
