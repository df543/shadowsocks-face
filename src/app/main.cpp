#include "common.hpp"
#include "global.h"
#include "ui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationDisplayName(QString::fromStdString(global::about::name));

    QTranslator translations(&a);
    translations.load(QLocale::system(), "", "", ":/translations");
    a.installTranslator(&translations);

    MainWindow w;
    if (!w.isHideFirst())
        w.show();

    return a.exec();
}
