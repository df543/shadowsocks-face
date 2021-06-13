#include "ShareDialog.h"
#include "ui_ShareDialog.h"

#include "tools/QRMatrix.h"

ShareDialog::ShareDialog(const SSConfig &config, QWidget *parent):
    QDialog(parent), ui(new Ui::ShareDialog), config(config)
{
    ui->setupUi(this);
    ui->textBrowser->setWordWrapMode(QTextOption::WrapAnywhere);

    QString title = config.name.isEmpty() ? tr("<Unnamed Config>") : config.name;
    ui->label_Name->setText(title);

    ui->radioButton_Base64->setChecked(true);
}

ShareDialog::~ShareDialog()
{ delete ui; }

void ShareDialog::updateURI(SSConfig::URIType uriType)
{
    QString uri = config.toURI(uriType);
    ui->textBrowser->setText(uri);

    auto qr = QRMatrix(uri);
    int size = qr.size();
    QImage qrImage(size, size, QImage::Format_Mono);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            qrImage.setPixel(i, j, !qr.at(i, j));
    ui->imageWidget->setImage(qrImage);
}

void ShareDialog::on_radioButton_Base64_toggled(bool checked)
{
    if (checked)
        updateURI(SSConfig::URIType::ORIGINAL_BASE64);
}

void ShareDialog::on_radioButton_SIP002_toggled(bool checked)
{
    if (checked)
        updateURI(SSConfig::URIType::SIP002);
}
