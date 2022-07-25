#include "dialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Set the app style sheet
    //QFile styleSheetFile("./Combinear.qss");
    //styleSheetFile.open(QFile::ReadOnly);
    //QString styleSheet=QLatin1String(styleSheetFile.readAll());
    //a.setStyleSheet(styleSheet);
    Dialog w;
    w.show();
    return a.exec();
}
