
#include "global.h"
#include "sspm/sspm.h"
#include "ui/mainwindow.h"
#include "tools/onceguard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(QString::fromStdString(global::about::abbr));
    a.setApplicationDisplayName(QString::fromStdString(global::about::name));
    a.setApplicationVersion(QString::fromStdString(global::about::version));

    // parts

    QTranslator translator(&a);
    translator.load(QLocale::system(), "", "", ":/translations");
    a.installTranslator(&translator);

    Sspm sspm;
    global::sspm = &sspm;

    MainWindow w;
    if (!w.isHideFirst())
        w.show();

    OnceGuard onceguard(a.applicationName());
    QObject::connect(&onceguard, &OnceGuard::knocked, &w, &MainWindow::focus);
    if (!onceguard.lock()) {
        std::cerr << "Another instance found, quiting.\n";
        return 2;
    }

    // parts end

    return a.exec();
}
