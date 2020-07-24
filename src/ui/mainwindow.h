#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.hpp"
#include "data/configmanager.h"
#include "sspm/processmanager.h"
#include "app/global.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool isHideFirst()
    {
        return hideFirst;
    }

public slots:
    void focus();

private:
    Ui::MainWindow *ui;
    ConfigManager *configManager;
    ProcessManager *processManager;
    void setRow(int row, bool bold);
    QSystemTrayIcon systray;
    bool hideFirst = false;
    void startConfig(Config &config); // update latency
    void testLatency(Config &config);
    QList<Config> configData;
    QString dirPath;

private slots:
    void sync();
    void reloadConfig();
    void checkCurrentRow();
    void loadAutoConnect();
    void saveAutoConnect();
    void onConnect();
    void onDisconnect();
    void onEdit();
    void onShare();
    void onImport();
    void onExport();
    void onAbout();
    void onManually();
    void onPaste();
    void onRemove();
    void onRefresh();
    void onTestLatency();

protected slots:
    void closeEvent(QCloseEvent *e);
};

#endif // MAINWINDOW_H
