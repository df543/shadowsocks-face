#include "editdialog.h"
#include "ui_editdialog.h"

EditDialog::EditDialog(SsConfig &config, QWidget *parent)
    : QDialog(parent), ui(new Ui::EditDialog), config(config)
{
    ui->setupUi(this);

    if (config.id == 0) setWindowTitle(tr("New"));
    else setWindowTitle(tr("Edit"));

    ui->lineEdit_name->setText(config.remarks);
    ui->lineEdit_serverAddr->setText(config.server);
    ui->spinBox_serverPort->setValue(config.server_port);
    ui->comboBox_method->setCurrentText(config.method);
    ui->lineEdit_password->setText(config.password);
    ui->lineEdit_localAddr->setText(config.local_address);
    ui->spinBox_localPort->setValue(config.local_port);
    ui->spinBox_timeout->setValue(config.timeout);
    ui->comboBox_mode->setCurrentText(config.mode);
    ui->checkBox_fastopen->setChecked(config.fastopen);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &EditDialog::onSave);
}

EditDialog::~EditDialog()
{
    delete ui;
}

void EditDialog::onSave()
{
    bool valid = true;
    if (ui->lineEdit_serverAddr->text().size() == 0) {
        ui->lineEdit_serverAddr->setStyleSheet("background-color: pink");
        valid = false;
    } else ui->lineEdit_serverAddr->setStyleSheet("background-color:");

    if (ui->comboBox_method->currentIndex() == -1) {
        ui->comboBox_method->setStyleSheet("background-color: pink");
        valid = false;
    } else ui->comboBox_method->setStyleSheet("background-color:");

    if (ui->comboBox_mode->currentIndex() == -1) {
        ui->comboBox_mode->setStyleSheet("background-color: pink");
        valid = false;
    } else ui->comboBox_mode->setStyleSheet("background-color:");
    if (!valid) return;

    config.remarks = ui->lineEdit_name->text();
    config.server = ui->lineEdit_serverAddr->text();
    config.server_port = ui->spinBox_serverPort->value();
    config.local_port = ui->spinBox_localPort->value();
    config.password = ui->lineEdit_password->text();
    config.timeout = ui->spinBox_timeout->value();
    config.method = ui->comboBox_method->currentText();
    config.mode = ui->comboBox_mode->currentText();
    config.fastopen = ui->checkBox_fastopen->isChecked();
    config.local_address = ui->lineEdit_localAddr->text();

    this->accept();
}
