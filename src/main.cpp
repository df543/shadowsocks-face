#include "global.h"
#include "sspm/sspm.h"
#include "ui/mainwindow.h"
#include "tools/SingleInstanceDoorbell.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName(global::abbr);
    a.setApplicationDisplayName(global::name);
    a.setApplicationVersion(global::version);

    QTranslator translator(&a);
    translator.load(QLocale::system(), "", "", ":/translations");
    a.installTranslator(&translator);

    Sspm sspm;
    global::sspm = &sspm;

    MainWindow w;
    if (!w.isHideFirst())
        w.show();

    SingleInstanceDoorbell onceguard(a.applicationName());
    QObject::connect(&onceguard, &SingleInstanceDoorbell::rang, &w, &MainWindow::focus);
    if (!onceguard.setup()) {
        std::cerr << "Found another instance, the program is quiting.\n";
        return 2;
    }

    return a.exec();
}
