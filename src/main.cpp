#include "pch.hpp"
#include "ui/mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setApplicationDisplayName(QString("Shadowsocks Face"));

    QTranslator translations(&a);
    translations.load(QLocale::system(), "", "", ":/translations");
    a.installTranslator(&translations);

    MainWindow w;
    if (!w.isHideFirst())
        w.show();

    return a.exec();
}
