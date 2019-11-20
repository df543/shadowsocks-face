#ifndef SHAREDIALOG_H
#define SHAREDIALOG_H

#include "pch.hpp"
#include "model/config.h"

namespace Ui {
class ShareDialog;
}

class ShareDialog : public QDialog {
    Q_OBJECT

public:
    explicit ShareDialog(const Config &config, QWidget *parent = nullptr);
    ~ShareDialog();

private:
    Ui::ShareDialog *ui;
};

#endif // SHAREDIALOG_H
