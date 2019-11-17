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

    if (!QDir(dirPath).mkpath("."))
        throw std::runtime_error("couldn't create config dir");
    ss_configStorage = new SSConfigStorage(dirPath, this);
    ss_processlist = new SSProcessList(dirPath, this);
    loadAutoConnect();
    connect(ss_processlist, &SSProcessList::procChanged, this, &MainWindow::saveAutoConnect);

    connect(ss_configStorage, &SSConfigStorage::dataChanged, this, &MainWindow::sync);
    connect(ss_processlist, &SSProcessList::procChanged, this, &MainWindow::sync);
    connect(ui->configTable, &QTableWidget::itemSelectionChanged, this, &MainWindow::checkCurrentRow);
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
    ui->configTable->setRowCount(ss_configStorage->size());
    int cnt = 0;
    for (auto i = ss_configStorage->cbegin(); i != ss_configStorage->cend(); ++i, ++cnt) {
        ui->configTable->setItem(cnt, 0, new QTableWidgetItem(i.key()));
        QString serverInfo = (*i)["server"].toVariant().toString() + ":" + (*i)["server_port"].toVariant().toString();
        ui->configTable->setItem(cnt, 1, new QTableWidgetItem(serverInfo));
        QString local;
        if (i->contains("local_address") && (*i)["local_address"].toString() != "127.0.0.1")
            local = (*i)["local_address"].toString() + ":" + (*i)["local_port"].toVariant().toString();
        else
            local = (*i)["local_port"].toVariant().toString();
        ui->configTable->setItem(cnt, 2, new QTableWidgetItem(local));

        if (ss_processlist->isRunning(i.key()))
            setRow(cnt, true);
    }

    ui->configTable->clearSelection();
}

void MainWindow::setRow(int row, bool bold) {
    QFont font;
    font.setBold(bold);
    for (int i = 0; i < 3; i++)
        ui->configTable->item(row, i)->setFont(font);
}

void MainWindow::checkCurrentRow() {
    if (ui->configTable->selectedRanges().size()) {
        QString name = ui->configTable->item(ui->configTable->currentRow(), 0)->text();
        bool connected = ss_processlist->isRunning(name);
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

bool MainWindow::_start(QString name) {
    auto p = ss_processlist->start(name);
    if (p) {
        connect(p, &QProcess::readyReadStandardOutput, [this, name, p] {
            ui->logArea->append("<b>" + name + "</b><br/>"
                                "<span style='color:DimGray'>" +
                                QString(p->readAllStandardOutput()).replace("\n", "<br/>") +
                                "</span>");
        });
        connect(p, &QProcess::readyReadStandardError, [this, name, p] {
            ui->logArea->append("<b>" + name + "</b><br/>"
                                "<span style='color:BlueViolet'>" +
                                QString(p->readAllStandardError()).replace("\n", "<br/>") +
                                "</span>");
        });
        return true;
    } else {
        ui->logArea->append("<b>" + name + "</b><br />"
                            "<span style='color:Red'>Failed to start</span>");
        return false;
    }
}

void MainWindow::onConnect() {
    int cr = ui->configTable->currentRow();
    QString name = ui->configTable->item(cr, 0)->text();
    if (!_start(name))
        onRefresh();
}

void MainWindow::onDisconnect() {
    int cr = ui->configTable->currentRow();
    QString name = ui->configTable->item(cr, 0)->text();
    ss_processlist->terminate(name);
}

void MainWindow::onManually() {
    EditDialog editDialog(QString(), QJsonObject(), true, this);
    connect(&editDialog, &EditDialog::submit, ss_configStorage, &SSConfigStorage::add);
    editDialog.exec();
}

void MainWindow::onPaste() {
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString s = clipboard->text();
    s = s.trimmed().replace("/?#", "#");

    QRegExp sip001("ss://(.+)");
    QRegExp sip002("ss://(.+)@(.+):(\\d+)(#.+)?");
    QString name;
    QJsonObject config;
    if (sip002.exactMatch(s)) {
        name = sip002.cap(4);
        config["server"] = sip002.cap(2);
        config["server_port"] = sip002.cap(3).toInt();
        QString hostinfo = QByteArray::fromBase64(sip002.cap(1).toUtf8());
        QRegExp reg("(.+):(.+)");
        reg.indexIn(hostinfo);
        config["method"] = reg.cap(1);
        config["password"] = reg.cap(2);
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
        config["method"] = reg.cap(1);
        config["password"] = reg.cap(2);
        config["server"] = reg.cap(3);
        config["server_port"] = reg.cap(4).toInt();
    } else
        return;

    if (name.startsWith("#"))
        name = name.right(name.size() - 1);
    name = QByteArray::fromPercentEncoding(name.toUtf8());

    EditDialog editDialog(name, config, true, this);
    connect(&editDialog, &EditDialog::submit, ss_configStorage, &SSConfigStorage::add);
    editDialog.exec();
}

void MainWindow::onEdit() {
    int cr = ui->configTable->currentRow();
    QString name = ui->configTable->item(cr, 0)->text();
    if (!ss_processlist->isRunning(name)) {
        QJsonObject toEdit{*ss_configStorage->cget(name)};
        EditDialog editDialog(name, toEdit, false, this);
        connect(&editDialog, &EditDialog::submit, ss_configStorage, &SSConfigStorage::edit);
        editDialog.exec();
    }
}

void MainWindow::onRemove() {
    int cr = ui->configTable->currentRow();
    QString name = ui->configTable->item(cr, 0)->text();
    if (QMessageBox::question(this,
                              tr("Confirm removing"),
                              tr("Are you sure to remove config '%1'?").arg(name),
                              QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No),
                              QMessageBox::No
                             ) == QMessageBox::Yes)
        ss_configStorage->remove(name);
}

void MainWindow::onRefresh() {
    ss_configStorage->refresh();
}

void MainWindow::onShare() {
    int cr = ui->configTable->currentRow();
    QString name = ui->configTable->item(cr, 0)->text();
    QJsonObject config{*ss_configStorage->cget(name)};
    ShareDialog *shareDialog = new ShareDialog{name, config, this};
    shareDialog->show();
}

void MainWindow::onImport() {
    ss_configStorage->importGUIConfig(QFileDialog::getOpenFileName(
                                          this,
                                          QString(),
                                          QString(),
                                          "GUI Config(*.json)"));
}

void MainWindow::onExport() {
    ss_configStorage->exportGUIConfig(QFileDialog::getSaveFileName(
                                          this,
                                          QString(),
                                          "gui-config.json",
                                          "GUI Config(*.json)"));
}

void MainWindow::onAbout() {
    QString content{tr(
                        "<h1>Shadowsocks Face</h1>"
                        "<p>A light weight <a href='https://github.com/shadowsocks/shadowsocks-libev'>"
                        "shadowsocks-libev</a> GUI wrapper</p>"

                        "<p>License: <a href='http://www.gnu.org/licenses/gpl.html'>GPL-3.0</a><br/>"
                        "Special thanks to <a href='https://github.com/shadowsocks/shadowsocks-qt5'>"
                        "Shadowsocks-Qt5</a> project<br/>"
                        "Use <a href='https://github.com/ricmoo/QRCode'>ricmoo/QRCode</a> "
                        "(<a href='https://opensource.org/licenses/MIT'>MIT</a>) to generate QR Code</p>"
                    )};
    QMessageBox::about(this, tr("About"), content);
}

void MainWindow::loadAutoConnect() {
    QFile f{QDir{dirPath}.filePath("last_connected.txt")};
    if (f.open(QIODevice::ReadOnly)) {
        QTextStream in(&f);
        for (;;) {
            QString line = in.readLine();
            if (line.isNull())
                break;
            else if (_start(line))
                hideFirst = true;
        }
        f.close();
    }
}

void MainWindow::saveAutoConnect() {
    QFile f{QDir{dirPath}.filePath("last_connected.txt")};
    if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&f);
        for (auto i = ss_configStorage->cbegin(); i != ss_configStorage->cend(); ++i)
            if (ss_processlist->isRunning(i.key()))
                out << i.key() << "\n";
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
