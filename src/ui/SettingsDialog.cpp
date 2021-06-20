#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

#include "global.h"

SettingsDialog::SettingsDialog(QWidget *parent):
    QDialog(parent), ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    ui->lineEdit_client->setText(global::settings["ss_client"].toString());
    ui->comboBox_commandType->setCurrentText(global::settings["ss_command_type"].toString());

    connect(ui->lineEdit_client, &QLineEdit::textChanged, [this] {
        if (ui->lineEdit_client->text().isEmpty())
            ui->lineEdit_client->setStyleSheet("background-color: pink");
        else
            ui->lineEdit_client->setStyleSheet("background-color:");
    });
}

SettingsDialog::~SettingsDialog()
{ delete ui; }

void SettingsDialog::on_toolButton_client_clicked()
{
    auto path = QFileDialog::getOpenFileName(this);
    if (!path.isNull())
        ui->lineEdit_client->setText(path);
}

void SettingsDialog::on_pushButton_default_clicked()
{
    ui->lineEdit_client->setText(global::defaultSettings["ss_client"].toString());
    ui->comboBox_commandType->setCurrentText(global::defaultSettings["ss_command_type"].toString());
}

void SettingsDialog::on_buttonBox_accepted()
{
    if (ui->lineEdit_client->text().isEmpty())
        return;

    using namespace global;
    settings["ss_client"] = ui->lineEdit_client->text();
    settings["ss_command_type"] = ui->comboBox_commandType->currentText();
    kvDAO->set("settings", QJsonDocument(QJsonObject::fromVariantHash(settings)).toJson());

    this->accept();
}
