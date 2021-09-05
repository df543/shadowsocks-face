#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "EditDialog.h"
#include "ShareDialog.h"
#include "SettingsDialog.h"
#include "global.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(global::name);
    ui->textBrowserLog->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    ui->tableViewConfig->setModel(&configModel);
    ui->tableViewConnections->setModel(&connectionModel);
    connect(ui->tableViewConfig->selectionModel(),      &QItemSelectionModel::selectionChanged, this, &MainWindow::checkStatus);
    connect(ui->tableViewConnections->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::checkStatus);
    checkStatus();
    connect(ui->tableViewConfig, &QTableView::doubleClicked, ui->actionEdit, &QAction::trigger);
    for (auto i : ui->toolBarConfig->actions())
        ui->tableViewConfig->addAction(i);
    for (auto i : ui->toolBarConnection->actions())
        ui->tableViewConnections->addAction(i);

    connect(&connectionModel, &ConnectionModel::output, [this](const SSConfig & config, SSConnection::OutputType outputType, QString msg) {
        QString textStyle =
            outputType == SSConnection::OutputType::STDOUT
            ? "color:DimGray;"
            : outputType == SSConnection::OutputType::STDERR
            ? "color:BlueViolet;" : QString();
        auto logEntry = QString("<b>%1</b><br>"
                                "<span style='%2'>%3</span>")
                        .arg(config.getName(), textStyle, msg.replace("\n", "<br>"));
        ui->textBrowserLog->append(logEntry);
    });

    compatibility();
    loadLastConnected();

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

void MainWindow::checkShow()
{
    // conditions for displaying the main window
    auto check = [this] {
        if (global::isFirstStart) {
            // show main window and settings window at first startup
            focus();
            on_actionSettings_triggered();
        } else if (connectionModel.rowCount(QModelIndex()) == 0) {
            // show the main window when there is no connection
            focus();
        }
    };

    // add startup delay to prevent connection failure
    QTimer::singleShot(300, check);
}

void MainWindow::focus()
{
    show();
    activateWindow();
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

void MainWindow::checkStatus()
{
    bool configSelected = ui->tableViewConfig->selectionModel()->hasSelection();
    ui->actionConnect->setEnabled(configSelected);
    ui->actionEdit->setEnabled(configSelected);
    ui->actionRemove->setEnabled(configSelected);
    ui->actionShare->setEnabled(configSelected);

    bool connectionSelected = ui->tableViewConnections->selectionModel()->hasSelection();
    ui->actionDisconnect->setEnabled(connectionSelected);
    ui->actionTestLatency->setEnabled(connectionSelected);
}

void MainWindow::loadLastConnected()
{
    QSet<decltype(SSConfig::id)> idSet;
    for (const auto &i : QJsonDocument::fromJson(global::kvDAO->get("last_connected").toUtf8()).array())
        idSet.insert(decltype(SSConfig::id)(i.toDouble()));
    for (const auto &i : configModel.getByIds(idSet))
        connectionModel.add(i);
}

void MainWindow::on_actionConnect_triggered()
{
    auto i = ui->tableViewConfig->selectionModel()->selection().indexes().at(0);
    const SSConfig &config = configModel.getAt(i);
    connectionModel.add(config);
}

void MainWindow::on_actionDisconnect_triggered()
{
    auto connection_index = ui->tableViewConnections->selectionModel()->selection().indexes().at(0);
    connectionModel.del(connection_index);
}

void MainWindow::on_actionAddManually_triggered()
{
    EditDialog editDialog(SSConfig(), EditDialog::DialogType::NEW_CONFIG, this);
    connect(&editDialog, &EditDialog::saveConfig, &configModel, &ConfigModel::add);
    editDialog.exec();
}

void MainWindow::on_actionAddFromURI_triggered()
{
    QString s = QGuiApplication::clipboard()->text();
    SSConfig toAdd;
    try {
        toAdd = SSConfig::fromURI(s);
    } catch (const std::invalid_argument &) {
        return;
    }

    EditDialog editDialog(toAdd, EditDialog::DialogType::NEW_CONFIG, this);
    connect(&editDialog, &EditDialog::saveConfig, &configModel, &ConfigModel::add);
    editDialog.exec();
}

void MainWindow::on_actionRemove_triggered()
{
    auto i = ui->tableViewConfig->selectionModel()->selection().indexes().at(0);
    const SSConfig &config = configModel.getAt(i);
    if (QMessageBox::question(
            this,
            tr("Confirm Deletion"),
            tr("Are you sure to remove config '%1'?").arg(config.getName()),
            QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No),
            QMessageBox::No) == QMessageBox::Yes) {
        configModel.removeAt(i);
    }
}

void MainWindow::on_actionEdit_triggered()
{
    auto i = ui->tableViewConfig->selectionModel()->selection().indexes().at(0);
    const SSConfig &config = configModel.getAt(i);

    EditDialog editDialog(config, EditDialog::DialogType::EDIT_CONFIG, this);
    connect(&editDialog, &EditDialog::saveConfig, &configModel, &ConfigModel::edit);
    editDialog.exec();
}

void MainWindow::on_actionRefresh_triggered()
{
    configModel.readAll();
    checkStatus();
}

void MainWindow::on_actionImport_triggered()
{
    auto path = QFileDialog::getOpenFileName(
                    this,
                    QString(),
                    QString(),
                    "GUI Config(*.json)"
                );
    if (!path.isNull())
        configModel.importGUIConfig(path);
}

void MainWindow::on_actionExport_triggered()
{
    auto path = QFileDialog::getSaveFileName(
                    this,
                    QString(),
                    "gui-config.json",
                    "GUI Config(*.json)"
                );
    if (!path.isNull())
        configModel.exportGUIConfig(path);
}

void MainWindow::on_actionShare_triggered()
{
    auto i = ui->tableViewConfig->selectionModel()->selection().indexes().at(0);
    const SSConfig &config = configModel.getAt(i);
    auto *shareDialog = new ShareDialog(config, this);
    shareDialog->show();
}

void MainWindow::on_actionTestLatency_triggered()
{
    auto connection_index = ui->tableViewConnections->selectionModel()->selection().indexes().at(0);
    connectionModel.testLatency(connection_index);
}

void MainWindow::on_actionSettings_triggered()
{ SettingsDialog(this).exec(); }

void MainWindow::on_actionAbout_triggered()
{
    QString content = tr("<h1>%1</h1>\n"
                         "\n"
                         "<p>\n"
                         "<em>%2</em> is a simple, cross-platform GUI for shadowsocks client, supports multiple implementations.\n"
                         "</p>\n"
                         "\n"
                         "<p>\n"
                         "Special thanks to <a href=\"https://github.com/shadowsocks/shadowsocks-qt5\">Shadowsocks-Qt5</a> project.<br>\n"
                         "Use <a href=\"https://github.com/nayuki/QR-Code-generator\">nayuki/QR-Code-generator</a> (<a href=\"https://opensource.org/licenses/MIT\">MIT</a>) to generate QR Code.<br>\n"
                         "Icon set used in this application is <a href=\"https://github.com/KDE/breeze-icons\">KDE/breeze-icons</a>.\n"
                         "</p>\n"
                         "\n"
                         "<hr>\n"
                         "\n"
                         "<p>\n"
                         "Homepage: <a href=\"https://github.com/df543/shadowsocks-face\">df543/shadowsocks-face</a><br>\n"
                         "License: <a href=\"https://www.gnu.org/licenses/gpl-3.0.html\">GPL-3.0</a><br>\n"
                         "Version: %3\n"
                         "</p>"
                        ).arg(global::name, global::shortName, global::version);
    QMessageBox::about(this, tr("About"), content);
}

void MainWindow::on_actionAboutQt_triggered()
{ QMessageBox::aboutQt(this); }

void MainWindow::on_actionShow_triggered()
{ focus(); }

void MainWindow::on_actionQuit_triggered()
{ QMetaObject::invokeMethod(qApp, &QApplication::quit, Qt::QueuedConnection); }

void MainWindow::compatibility()
{
    typedef decltype(SSConfig::id) IDType;
    QDir configDir(global::savePath());

    // restore old configs

    QList<SSConfig> oldConfigs;
    configDir.setNameFilters({"*.json"});
    for (const auto &i : configDir.entryInfoList()) {
        QFile f(i.filePath());
        if (!f.open(QIODevice::ReadOnly))
            throw std::runtime_error("couldn't open config file");
        auto json = QJsonDocument::fromJson(f.readAll()).object();
        f.close();

        // patch an old bug
        if (json.contains("fastopen"))
            json["fast_open"] = json["fastopen"].toBool();

        // determine config id
        IDType id = -1;
        if (json.contains("id")) {
            id = IDType(json["id"].toDouble());
        } else {
            bool baseNameIsNumber = false;
            auto baseNameNumber = i.baseName().toLongLong(&baseNameIsNumber);
            if (baseNameIsNumber)
                id = baseNameNumber;
        }

        auto config = SSConfig::fromJsonObject(json);
        config.id = id;

        if (f.remove())
            oldConfigs.append(config);
    }

    QHash<IDType, IDType> idMap;

    for (auto &i : oldConfigs) {
        IDType oldID = i.id;
        configModel.add(i);
        if (oldID != -1)
            idMap[oldID] = i.id;
    }

    // restore old last_connected

    QList<IDType> ids;
    QFile last_connected_file(configDir.filePath("last_connected.txt"));
    if (last_connected_file.open(QIODevice::ReadOnly)) {
        QTextStream in(&last_connected_file);
        forever {
            QString line = in.readLine();
            if (line.isNull()) break;
            IDType oldID = line.toLongLong();
            if (idMap.contains(oldID))
                ids.push_back(idMap[oldID]);
        }
        last_connected_file.close();
    }

    if (last_connected_file.remove()) {
        auto last_connected = QJsonDocument::fromJson(global::kvDAO->get("last_connected").toUtf8()).array();
        for (auto id : ids)
            if (abs(id) <= 9007199254740992)
                last_connected.push_back(id);
        global::kvDAO->set("last_connected", QJsonDocument(last_connected).toJson());
    }
}
