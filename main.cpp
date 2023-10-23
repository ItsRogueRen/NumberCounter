#include "numbercounter.h"
#include <QtWidgets/QApplication>
#include <QFontDatabase>
#include <QDateTime>

int main(int argc, char *argv[])
{
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication a(argc, argv);
	QFont systemFont = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
	QApplication::setFont(systemFont);
	srand(QDateTime::currentDateTime().toMSecsSinceEpoch());
    NumberCounter w;
    w.show();
    return a.exec();
}
