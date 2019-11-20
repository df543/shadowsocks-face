#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pch.hpp"
#include "manager/configmanager.h"
#include "manager/processmanager.h"
#include "version.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool isHideFirst() {
        return hideFirst;
    }

private:
    Ui::MainWindow *ui;
    ConfigManager *configManager;
    ProcessManager *processManager;
    void setRow(int row, bool bold);
    QSystemTrayIcon systray;
    bool hideFirst = false;
    bool startConfig(const Config &config);
    QList<Config> configData;

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
    void onActivate();

protected slots:
    void closeEvent(QCloseEvent *e);
};

#endif // MAINWINDOW_H
