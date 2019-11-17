#ifndef SHAREDIALOG_H
#define SHAREDIALOG_H

#include "pch.hpp"

namespace Ui {
class ShareDialog;
}

class ShareDialog : public QDialog {
    Q_OBJECT

public:
    explicit ShareDialog(QString name, QJsonObject configToShare, QWidget *parent = nullptr);
    ~ShareDialog();

private:
    Ui::ShareDialog *ui;
};

#endif // SHAREDIALOG_H
