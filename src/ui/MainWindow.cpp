#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "EditDialog.h"
#include "ShareDialog.h"
#include "global.h"
#include "tools/latencytester.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow),
    dirPath(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation))
{
    ui->setupUi(this);
    setWindowTitle(global::name);
    ui->configTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    connect(ui->configTable, &QTableWidget::itemSelectionChanged, this, &MainWindow::checkStatus);

    ui->tableViewConnections->setModel(&connectionModel);
    connect(ui->tableViewConnections->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::checkStatus);

    connect(&connectionModel, &ConnectionModel::output, [this](const SSConfig & config, ConnectionModel::OutputType outputType, QString msg) {
        QString textStyle;
        switch (outputType) {
        case ConnectionModel::OutputType::STDOUT:
            textStyle += "color:DimGray;";
            break;
        case ConnectionModel::OutputType::STDERR:
            textStyle += "color:BlueViolet;";
            break;
        default:
            throw std::runtime_error("unknown process output type");
        }
        auto logEntry = QString("<b>%1</b><br>"
                                "<span style='%2'>%3</span>")
                        .arg(config.getName(), textStyle, msg.replace("\n", "<br>"));
        ui->textBrowserLog->append(logEntry);
    });

    if (!QDir(dirPath).mkpath("."))
        throw std::runtime_error("couldn't create config dir");
    configManager = new ConfigManager(dirPath, this);

    reloadConfig();
    loadAutoConnect();
    sync();
    checkStatus();

    connect(ui->configTable, &QTableWidget::itemDoubleClicked, ui->actionEdit, &QAction::triggered);

    for (auto i : ui->toolBarConfig->actions())
        ui->configTable->addAction(i);
    for (auto i : ui->toolBarConnection->actions())
        ui->tableViewConnections->addAction(i);

    systray.setIcon(QIcon(":/icons/app"));
    auto *systrayMenu = new QMenu(this);
    systrayMenu->addAction(ui->actionShow);
    systrayMenu->addAction(ui->actionQuit);
    systray.setContextMenu(systrayMenu);
    connect(&systray, &QSystemTrayIcon::activated, this, &MainWindow::focus);
    systray.show();
}

MainWindow::~MainWindow()
{ delete ui; }

void MainWindow::focus()
{
    show();
    activateWindow();
}

void MainWindow::testLatency(SSConfig &config)
{
//    if (processManager->isRunning(config.id)) {
//        auto *tester = new LatencyTester(
//            QNetworkProxy(QNetworkProxy::Socks5Proxy, config.local_address, config.local_port),
//            QUrl("https://google.com"),
//            this
//        );
//        connect(tester, &LatencyTester::testFinished, [&config, this](int latencyMs) {
//            if (processManager->isRunning(config.id)) {
//                config.latencyMs = latencyMs;
//                sync();
//            }
//        });
//        tester->start();
//    }
}

void MainWindow::sync()
{
    ui->configTable->setRowCount(configData.size());
    for (int i = 0; i < configData.size(); i++) {
        const SSConfig &config = configData[i];
        ui->configTable->setItem(i, 0, new QTableWidgetItem(config.getName()));

        QString latency;
//        if (config.latencyMs == TIMEOUT)
//            latency = tr("timeout");
//        else if (config.latencyMs >= 0)
//            latency = QString("%1 ms").arg(config.latencyMs);
        ui->configTable->setItem(i, 1, new QTableWidgetItem(latency));

        QString local;
        if (config.local_address != "127.0.0.1") local += config.local_address + ":";
        local += QString::number(config.local_port);
        ui->configTable->setItem(i, 2, new QTableWidgetItem(local));
    }
    ui->configTable->clearSelection();
}

void MainWindow::reloadConfig()
{
    // save latency
//    QHash<qint64, int> latency;
//    for (const auto &i : configData)
//        latency[i.id] = i.latencyMs;
    // reload
    configData = configManager->query();
    // restore latency
//    for (auto &i : configData)
//        if (latency.contains(i.id))
//            i.latencyMs = latency[i.id];
    // refresh view
    sync();
}

void MainWindow::checkStatus()
{
    bool configSelected = !ui->configTable->selectedRanges().empty();
    ui->actionConnect->setEnabled(configSelected);
    ui->actionEdit->setEnabled(configSelected);
    ui->actionRemove->setEnabled(configSelected);
    ui->actionShare->setEnabled(configSelected);

    bool connectionSelected = ui->tableViewConnections->selectionModel()->hasSelection();
    ui->actionDisconnect->setEnabled(connectionSelected);
    ui->actionTestLatency->setEnabled(connectionSelected);
}

void MainWindow::loadAutoConnect()
{
//    QFile f{QDir{dirPath}.filePath("last_connected.txt")};
//    QSet<qint64> startIds;
//    if (f.open(QIODevice::ReadOnly)) {
//        QTextStream in(&f);
//        for (;;) {
//            QString line = in.readLine();
//            if (line.isNull()) break;
//            else startIds.insert(line.toInt());
//        }
//        f.close();
//    }
//    if (!startIds.empty()) hideFirst = true;
//    for (auto &i : configData)
//        if (startIds.contains(i.id))
//            startConfig(i);
}

void MainWindow::saveAutoConnect()
{
//    QFile f{QDir{dirPath}.filePath("last_connected.txt")};
//    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
//        QTextStream out(&f);
//        for (const auto &i : configData)
//            if (processManager->isRunning(i.id))
//                out << i.id << "\n";
//        f.close();
//    }
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (e->spontaneous()) {
        e->ignore();
        hide();
    } else {
        QMainWindow::closeEvent(e);
    }
}

void MainWindow::on_actionConnect_triggered()
{
    int row = ui->configTable->currentRow();
    SSConfig &toConnect = configData[row];

    connectionModel.add(toConnect);
}

void MainWindow::on_actionDisconnect_triggered()
{
    connectionModel.del(ui->tableViewConnections->selectionModel()->selection().indexes().at(0));
}

void MainWindow::on_actionEdit_triggered()
{
    int row = ui->configTable->currentRow();
    SSConfig &toEdit = configData[row];

    EditDialog editDialog(toEdit, this);
    if (editDialog.exec() == QDialog::Accepted) {
        configManager->edit(toEdit);
        sync();
    }
}

void MainWindow::on_actionShare_triggered()
{
    int row = ui->configTable->currentRow();
    SSConfig &toShare = configData[row];
    auto *shareDialog = new ShareDialog(toShare, this);
    shareDialog->show();
}

void MainWindow::on_actionImport_triggered()
{
    configManager->importGUIConfig(
        QFileDialog::getOpenFileName(
            this,
            QString(),
            QString(),
            "GUI Config(*.json)"
        )
    );
    reloadConfig();
}

void MainWindow::on_actionExport_triggered()
{
    configManager->exportGUIConfig(
        QFileDialog::getSaveFileName(
            this,
            QString(),
            "gui-config.json",
            "GUI Config(*.json)"
        )
    );
}

void MainWindow::on_actionQuit_triggered()
{ QApplication::quit(); }

void MainWindow::on_actionAbout_triggered()
{
    QString content{tr(
                        "<h1>%1</h1>"

                        "<p><i>%2</i> is a <a href='https://github.com/shadowsocks/shadowsocks-libev'>shadowsocks-libev</a> client wrapper using qt5.</p>"

                        "<p>"
                        "Special thanks to <a href='https://github.com/shadowsocks/shadowsocks-qt5'>Shadowsocks-Qt5</a> project;<br>"
                        "Use <a href='https://github.com/nayuki/QR-Code-generator'>nayuki/QR-Code-generator</a> (<a href='https://opensource.org/licenses/MIT'>MIT</a>) to generate QR Code."
                        "</p>"

                        "<hr>"
                        "Version: %3<br>"
                        "License: <a href='https://www.gnu.org/licenses/gpl.html'>GPL-3.0</a><br>"
                        "Project Home: <a href='https://github.com/df543/Shadowsocks-Face'>df543/Shadowsocks-Face</a>"
                    ).arg(
                        qApp->applicationDisplayName(),
                        qApp->applicationName(),
                        qApp->applicationVersion()
                    )};
    QMessageBox::about(this, tr("About"), content);
}

void MainWindow::on_actionAboutQt_triggered()
{ QMessageBox::aboutQt(this); }

void MainWindow::on_actionManually_triggered()
{
    SSConfig toAdd;
    EditDialog editDialog(toAdd, this);
    if (editDialog.exec() == QDialog::Accepted) {
        configManager->add(toAdd);
        configData.append(toAdd);
        sync();
    }
}

void MainWindow::on_actionRemove_triggered()
{
    int row = ui->configTable->currentRow();
    SSConfig &toRemove = configData[row];
    if (QMessageBox::question(
            this,
            tr("Confirm removing"),
            tr("Are you sure to remove config '%1'?").arg(toRemove.getName()),
            QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No),
            QMessageBox::No
        ) == QMessageBox::Yes) {
        configManager->remove(toRemove);
        configData.removeAt(row);
        sync();
    }
}

void MainWindow::on_actionRefresh_triggered()
{ reloadConfig(); }

void MainWindow::on_actionPaste_triggered()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString s = clipboard->text();

    SSConfig toAdd;
    try {
        toAdd = SSConfig::fromURI(s);
    } catch (const std::invalid_argument &) {
        return;
    }

    EditDialog editDialog(toAdd, this);
    if (editDialog.exec() == QDialog::Accepted) {
        configManager->add(toAdd);
        configData.append(toAdd);
        sync();
    }
}

void MainWindow::on_actionShow_triggered()
{ focus(); }

void MainWindow::on_actionTestLatency_triggered()
{
    int row = ui->configTable->currentRow();
    SSConfig &config = configData[row];
    testLatency(config);
}
