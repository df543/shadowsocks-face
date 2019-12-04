#include "sharedialog.h"
#include "ui_sharedialog.h"

#include "qrlib/qreasy.h"

ShareDialog::ShareDialog(const Config &config, QWidget *parent)
    : QDialog(parent), ui(new Ui::ShareDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Share '%1'").arg(config.remarks));
    ui->textBrowser->setWordWrapMode(QTextOption::WrapAnywhere);

    QString uri = config.toUri();
    ui->textBrowser->setText(uri);

    auto qr = QrEasy::encode(uri.toStdString().data());
    int size = 512;
    QImage qrImage(size, size, QImage::Format_Mono);
    qrImage.fill(1);
    auto scale = double(size) / qr.size();
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            qrImage.setPixel(i, j, !qr.at(i / scale, j / scale));
    ui->imageWidget->setImage(qrImage);
}

ShareDialog::~ShareDialog()
{
    delete ui;
}
