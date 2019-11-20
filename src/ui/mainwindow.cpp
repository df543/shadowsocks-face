#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "editdialog.h"
#include "singleinstance.h"
#include "sharedialog.h"

#ifdef Q_OS_WIN
static QString dirPath = QDir::homePath() + "\\AppData\\Local\\ss-face";
#else
static QString dirPath = QDir::homePath() + "/.config/ss-face";
#endif

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    SingleInstance *guard = new SingleInstance(QApplication::applicationName(), this);
    if (!guard->runGuard()) {
        std::cout << "Another instance found, program is quiting.\n";
        exit(0);
    }
    connect(guard, &SingleInstance::newInstance, this, &MainWindow::onActivate);

    QProcess testStart(this);
    testStart.start("ss-local");
    testStart.waitForFinished();
    if (testStart.error() == QProcess::FailedToStart) {
        QMessageBox::critical(this, tr("Start Error"), tr("Failed to start ss-local process. Please check shadowsocks-libev installation."));
        exit(1);
    }

    ui->setupUi(this);
    ui->configTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    connect(ui->configTable, &QTableWidget::itemSelectionChanged, this, &MainWindow::checkCurrentRow);

    if (!QDir(dirPath).mkpath("."))
        throw std::runtime_error("couldn't create config dir");
    configManager = new ConfigManager(dirPath, this);
    processManager = new ProcessManager(dirPath, this);
    connect(processManager, &ProcessManager::procChanged, this, &MainWindow::saveAutoConnect);
    connect(processManager, &ProcessManager::procChanged, this, &MainWindow::sync);

    reloadConfig();
    loadAutoConnect();
    sync();
    checkCurrentRow();

    connect(ui->actionConnect,      &QAction::triggered, this, &MainWindow::onConnect);
    connect(ui->actionDisconnect,   &QAction::triggered, this, &MainWindow::onDisconnect);
    connect(ui->actionEdit,         &QAction::triggered, this, &MainWindow::onEdit);
    connect(ui->actionShare,        &QAction::triggered, this, &MainWindow::onShare);
    connect(ui->actionImport,       &QAction::triggered, this, &MainWindow::onImport);
    connect(ui->actionExport,       &QAction::triggered, this, &MainWindow::onExport);
    connect(ui->actionQuit,         &QAction::triggered, qApp, &QApplication::quit);
    connect(ui->actionAbout,        &QAction::triggered, this, &MainWindow::onAbout);
    connect(ui->actionAboutQt,      &QAction::triggered, [this] {QMessageBox::aboutQt(this);});
    connect(ui->actionManually,     &QAction::triggered, this, &MainWindow::onManually);
    connect(ui->actionPaste,        &QAction::triggered, this, &MainWindow::onPaste);
    connect(ui->actionRemove,       &QAction::triggered, this, &MainWindow::onRemove);
    connect(ui->actionRefresh,      &QAction::triggered, this, &MainWindow::onRefresh);
    connect(ui->actionShow,         &QAction::triggered, this, &MainWindow::onActivate);

    connect(ui->configTable, &QTableWidget::itemDoubleClicked, this, &MainWindow::onEdit);
    for (auto i : ui->mainToolBar->actions())
        ui->configTable->addAction(i);

    systray.setIcon(QIcon(":/prog-icon/icon"));
    QMenu *systrayMenu = new QMenu(this);
    systrayMenu->addAction(ui->actionShow);
    systrayMenu->addAction(ui->actionQuit);
    systray.setContextMenu(systrayMenu);
    connect(&systray, &QSystemTrayIcon::activated, this, &MainWindow::onActivate);
    systray.show();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onActivate() {
    show();
    activateWindow();
}

void MainWindow::sync() {
    ui->configTable->setRowCount(configData.size());
    for (int i = 0; i < configData.size(); i++) {
        const Config &config = configData[i];
        ui->configTable->setItem(i, 0, new QTableWidgetItem(config.remarks));
        ui->configTable->setItem(i, 1, new QTableWidgetItem(config.server + ":" + QString::number(config.server_port)));
        QString local;
        if (config.local_address != "127.0.0.1") local += config.local_address + ":";
        local += QString::number(config.local_port);
        ui->configTable->setItem(i, 2, new QTableWidgetItem(local));
        if (processManager->isRunning(config.id))
            setRow(i, true);
    }
    ui->configTable->clearSelection();
}

void MainWindow::reloadConfig() {
    configData = configManager->query();
    sync();
}

void MainWindow::setRow(int row, bool bold) {
    QFont font;
    font.setBold(bold);
    for (int i = 0; i < 3; i++)
        ui->configTable->item(row, i)->setFont(font);
}

void MainWindow::checkCurrentRow() {
    if (ui->configTable->selectedRanges().size()) {
        int row = ui->configTable->currentRow();
        int id = configData[row].id;
        bool connected = processManager->isRunning(id);
        ui->actionConnect->setEnabled(!connected);
        ui->actionDisconnect->setEnabled(connected);
        ui->actionEdit->setEnabled(!connected);
        ui->actionRemove->setEnabled(!connected);
        ui->actionShare->setEnabled(true);
    } else {
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(false);
        ui->actionEdit->setEnabled(false);
        ui->actionRemove->setEnabled(false);
        ui->actionShare->setEnabled(false);
    }
}

bool MainWindow::startConfig(const Config &config) {
    auto p = processManager->start(config.id);
    if (p) {
        connect(p, &QProcess::readyReadStandardOutput, [this, config, p] {
            ui->logArea->append(
                "<b>" + config.remarks + "</b><br/>"
                "<span style='color:DimGray'>" + QString(p->readAllStandardOutput()).replace("\n", "<br/>") + "</span>"
            );
        });
        connect(p, &QProcess::readyReadStandardError, [this, config, p] {
            ui->logArea->append(
                "<b>" + config.remarks + "</b><br/>"
                "<span style='color:BlueViolet'>" + QString(p->readAllStandardError()).replace("\n", "<br/>") + "</span>"
            );
        });
        return true;
    } else {
        ui->logArea->append(
            "<b>" + config.remarks + "</b><br />"
            "<span style='color:Red'>Failed to start</span>"
        );
        return false;
    }
}

void MainWindow::onConnect() {
    int row = ui->configTable->currentRow();
    if (!startConfig(configData[row]))
        sync();
}

void MainWindow::onDisconnect() {
    int row = ui->configTable->currentRow();
    int id = configData[row].id;
    processManager->terminate(id);
}

void MainWindow::onManually() {
    Config toAdd;
    EditDialog editDialog(toAdd, this);
    if (editDialog.exec() == QDialog::Accepted) {
        configManager->add(toAdd);
        configData.append(toAdd);
        sync();
    }
}

void MainWindow::onPaste() {
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString s = clipboard->text();
    s = s.trimmed().replace("/?#", "#");

    QRegExp sip001("ss://(.+)");
    QRegExp sip002("ss://(.+)@(.+):(\\d+)(#.+)?");
    QString name;
    Config toAdd;
    if (sip002.exactMatch(s)) {
        name = sip002.cap(4);
        toAdd.server = sip002.cap(2);
        toAdd.server_port = sip002.cap(3).toInt();
        QString hostinfo = QByteArray::fromBase64(sip002.cap(1).toUtf8());
        QRegExp reg("(.+):(.+)");
        reg.indexIn(hostinfo);
        toAdd.method = reg.cap(1);
        toAdd.password = reg.cap(2);
    } else if (sip001.exactMatch(s)) {
        QString info = sip001.cap(1);
        if (info.count('#')) {
            int i = info.indexOf('#');
            name = info.right(info.size() - i - 1);
            info = info.left(i);
        }
        info = QByteArray::fromBase64(info.toUtf8());
        QRegExp reg("(.+):(.+)@(.+):(\\d+)");
        reg.indexIn(info);
        toAdd.method = reg.cap(1);
        toAdd.password = reg.cap(2);
        toAdd.server = reg.cap(3);
        toAdd.server_port = reg.cap(4).toInt();
    } else
        return;

    if (name.startsWith("#"))
        name = name.right(name.size() - 1);
    name = QByteArray::fromPercentEncoding(name.toUtf8());
    toAdd.remarks = name;

    EditDialog editDialog(toAdd, this);
    if (editDialog.exec() == QDialog::Accepted) {
        configManager->add(toAdd);
        configData.append(toAdd);
        sync();
    }
}

void MainWindow::onEdit() {
    int row = ui->configTable->currentRow();
    Config &toEdit = configData[row];
    if (!processManager->isRunning(toEdit.id)) {
        EditDialog editDialog(toEdit, this);
        if (editDialog.exec() == QDialog::Accepted) {
            configManager->edit(toEdit);
            sync();
        }
    }
}

void MainWindow::onRemove() {
    int row = ui->configTable->currentRow();
    Config &toRemove = configData[row];
    if (QMessageBox::question(
                this,
                tr("Confirm removing"),
                tr("Are you sure to remove config '%1'?").arg(toRemove.remarks),
                QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No),
                QMessageBox::No
            ) == QMessageBox::Yes) {
        configManager->remove(toRemove);
        configData.removeAt(row);
        sync();
    }
}

void MainWindow::onRefresh() {
    reloadConfig();
}

void MainWindow::onShare() {
    int row = ui->configTable->currentRow();
    Config &toShare = configData[row];
    ShareDialog *shareDialog = new ShareDialog{toShare, this};
    shareDialog->show();
}

void MainWindow::onImport() {
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

void MainWindow::onExport() {
    configManager->exportGUIConfig(
        QFileDialog::getSaveFileName(
            this,
            QString(),
            "gui-config.json",
            "GUI Config(*.json)"
        )
    );
}

void MainWindow::onAbout() {
    QString content{tr(
                        "<h1>Shadowsocks Face</h1>"
                        "<b>Version %1</b>"
                        "<p>A light weight <a href='https://github.com/shadowsocks/shadowsocks-libev'>"
                        "shadowsocks-libev</a> GUI wrapper</p>"

                        "<p>License: <a href='http://www.gnu.org/licenses/gpl.html'>GPL-3.0</a><br/>"
                        "Special thanks to <a href='https://github.com/shadowsocks/shadowsocks-qt5'>"
                        "Shadowsocks-Qt5</a> project<br/>"
                        "Use <a href='https://github.com/ricmoo/QRCode'>ricmoo/QRCode</a> "
                        "(<a href='https://opensource.org/licenses/MIT'>MIT</a>) to generate QR Code</p>"
                    ).arg(VERSION)};
    QMessageBox::about(this, tr("About"), content);
}

void MainWindow::loadAutoConnect() {
    QFile f{QDir{dirPath}.filePath("last_connected.txt")};
    QSet<int> startIds;
    if (f.open(QIODevice::ReadOnly)) {
        QTextStream in(&f);
        for (;;) {
            QString line = in.readLine();
            if (line.isNull()) break;
            else startIds.insert(line.toInt());
        }
        f.close();
    }
    if (startIds.size()) hideFirst = true;
    for (auto i : configData)
        if (startIds.contains(i.id))
            startConfig(i);
}

void MainWindow::saveAutoConnect() {
    QFile f{QDir{dirPath}.filePath("last_connected.txt")};
    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&f);
        for (const auto &i : configData)
            if (processManager->isRunning(i.id))
                out << i.id << "\n";
        f.close();
    }
}

void MainWindow::closeEvent(QCloseEvent *e) {
    if (e->spontaneous()) {
        e->ignore();
        hide();
    } else {
        QMainWindow::closeEvent(e);
    }
}
