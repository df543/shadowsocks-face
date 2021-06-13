#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "data/model/ConfigModel.h"
#include "data/model/ConnectionModel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void checkShow();

public slots:
    void focus();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon systray;
    ConnectionModel connectionModel{this};
    ConfigModel configModel{this};

private slots:
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
