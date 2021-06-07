#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "data/configmanager.h"
#include "model/ConnectionModel.h"
#include "global.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    bool isHideFirst() const
    { return hideFirst; }

public slots:
    void focus();

private:
    Ui::MainWindow *ui;
    ConfigManager *configManager;
    QSystemTrayIcon systray;
    bool hideFirst = false;
    void testLatency(SSConfig &config);
    QList<SSConfig> configData;
    ConnectionModel connectionModel{this};

private slots:
    void sync();
    void reloadConfig();
    void checkStatus();
    void loadAutoConnect();
    void saveAutoConnect();

    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();
    void on_actionEdit_triggered();
    void on_actionShare_triggered();
    void on_actionImport_triggered();
    void on_actionExport_triggered();
    void on_actionQuit_triggered();
    void on_actionAbout_triggered();
    void on_actionAboutQt_triggered();
    void on_actionAddManually_triggered();
    void on_actionRemove_triggered();
    void on_actionRefresh_triggered();
    void on_actionAddFromURI_triggered();
    void on_actionShow_triggered();
    void on_actionTestLatency_triggered();

protected slots:
    void closeEvent(QCloseEvent *e) override;
};

#endif // MAIN_WINDOW_H
