#include "editdialog.h"
#include "ui_editdialog.h"

EditDialog::EditDialog(QString name, QJsonObject config, bool isNew, QWidget *parent): QDialog(parent), ui(new Ui::EditDialog), old_name(name) {
    ui->setupUi(this);

    if (isNew) setWindowTitle(tr("New"));
    else setWindowTitle(tr("Edit"));

    ui->lineEdit_name->setText(name);
    ui->lineEdit_serverAddr->setText(config["server"].toString());
    ui->spinBox_serverPort->setValue(config["server_port"].toVariant().toInt());
    if (config.contains("method")) {
        ui->comboBox_method->setCurrentIndex(-1);
        ui->comboBox_method->setCurrentText(config["method"].toString());
    }
    ui->lineEdit_password->setText(config["password"].toString());
    if (config.contains("local_address"))
        ui->lineEdit_localAddr->setText(config["local_address"].toString());
    if (config.contains("local_port"))
        ui->spinBox_localPort->setValue(config["local_port"].toVariant().toInt());
    if (config.contains("timeout"))
        ui->spinBox_timeout->setValue(config["timeout"].toVariant().toInt());
    ui->comboBox_mode->setCurrentText(config["mode"].toString());
    ui->checkBox_fastopen->setChecked(config["fast_open"].toBool());

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &EditDialog::onSave);
}

EditDialog::~EditDialog() {
    delete ui;
}

void EditDialog::onSave() {
    QString new_name = ui->lineEdit_name->text();

    bool valid = true;

    if (new_name.size() == 0) {
        ui->lineEdit_name->setStyleSheet("background-color: pink");
        valid = false;
    } else ui->lineEdit_name->setStyleSheet("background-color:");

    if (ui->comboBox_method->currentIndex() == -1) {
        ui->comboBox_method->setStyleSheet("background-color: pink");
        valid = false;
    } else ui->comboBox_method->setStyleSheet("background-color:");

    if (ui->comboBox_mode->currentIndex() == -1) {
        ui->comboBox_mode->setStyleSheet("background-color: pink");
        valid = false;
    } else ui->comboBox_mode->setStyleSheet("background-color:");

    if (!valid) return;

    QJsonObject config;
    config["server"] = ui->lineEdit_serverAddr->text();
    config["server_port"] = ui->spinBox_serverPort->value();
    config["local_port"] = ui->spinBox_localPort->value();
    config["password"] = ui->lineEdit_password->text();
    config["timeout"] = ui->spinBox_timeout->value();
    config["method"] = ui->comboBox_method->currentText();
    if (ui->comboBox_mode->currentText() != "tcp_only")
        config["mode"] = ui->comboBox_mode->currentText();
    if (ui->checkBox_fastopen->isChecked())
        config["fast_open"] = true;
    if (!ui->lineEdit_localAddr->text().isEmpty() && ui->lineEdit_localAddr->text() != "127.0.0.1")
        config["local_address"] = ui->lineEdit_localAddr->text();

    emit submit(new_name, config, old_name);
    this->accept();
}
