#include "sharedialog.h"
#include "ui_sharedialog.h"

#include "qrlib/qreasy.h"

ShareDialog::ShareDialog(const Config &config, QWidget *parent): QDialog(parent), ui(new Ui::ShareDialog) {
    ui->setupUi(this);
    setWindowTitle(tr("Share '%1'").arg(config.remarks));

    QString uriContent = QString("%1:%2@%3:%4")
                         .arg(config.method)
                         .arg(config.password)
                         .arg(config.server)
                         .arg(config.server_port);
    QString uri = QString("ss://%1").arg(QString(uriContent.toUtf8().toBase64()).replace("=", ""));
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

ShareDialog::~ShareDialog() {
    delete ui;
}
