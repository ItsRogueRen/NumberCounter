#include "filenamelineedit.h"
#include <QKeyEvent>

FileNameLineEdit::FileNameLineEdit(QWidget *parent)
	: QLineEdit(parent)
{
	this->InitLineEdit();
}

FileNameLineEdit::~FileNameLineEdit()
{

}

void FileNameLineEdit::InitLineEdit()
{
	this->setReadOnly(true);
	this->setMaxLength(256);
}


void FileNameLineEdit::focusOutEvent(QFocusEvent* event)
{
	if (!this->isReadOnly())
	{
		this->setReadOnly(true);
		m_strNewName = QLineEdit::text().trimmed();
		emit SignalNameChanged(m_strNewName,m_strOldName);
	}
	QLineEdit::focusOutEvent(event);
}

void FileNameLineEdit::mouseDoubleClickEvent(QMouseEvent* event)
{
	this->setReadOnly(false);
	this->setFocus();
	m_strOldName = QLineEdit::text().trimmed();
	QLineEdit::mouseDoubleClickEvent(event);
}

void  FileNameLineEdit::keyPressEvent(QKeyEvent* event)
{
	int nKey =  event->key();
	bool blEndEdit = nKey == Qt::Key_Return || nKey == Qt::Key_Enter || nKey == Qt::Key_Escape || nKey == Qt::Key_Tab;
	if (blEndEdit)
	{
		this->clearFocus();
	}
	QLineEdit::keyPressEvent(event);
}