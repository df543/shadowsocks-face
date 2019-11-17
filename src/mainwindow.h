#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pch.hpp"
#include "ssconfigstorage.h"
#include "ssprocesslist.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool isHideFirst() { return hideFirst; }

private:
    Ui::MainWindow *ui;
    SSConfigStorage *ss_configStorage;
    SSProcessList *ss_processlist;
    void setRow(int row, bool bold);
    QSystemTrayIcon systray;
    bool hideFirst = false;
    bool _start(QString name);

private slots:
    void sync();
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
