#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "data/configmanager.h"
#include "sspm/processmanager.h"
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
    ProcessManager *processManager;
    void setRow(int row, bool bold);
    QSystemTrayIcon systray;
    bool hideFirst = false;
    void startConfig(SSConfig &config); // update latency
    void testLatency(SSConfig &config);
    QList<SSConfig> configData;
    QString dirPath;
    bool test_client() {
        QProcess test(this);
        test.start("ss-local", QStringList());
        test.waitForFinished();
        return test.error() != QProcess::FailedToStart;
    }

private slots:
    void sync();
    void reloadConfig();
    void checkCurrentRow();
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
    void on_actionManually_triggered();
    void on_actionRemove_triggered();
    void on_actionRefresh_triggered();
    void on_actionPaste_triggered();
    void on_actionShow_triggered();
    void on_actionTestLatency_triggered();

protected slots:
    void closeEvent(QCloseEvent *e) override;
};

#endif // MAIN_WINDOW_H
