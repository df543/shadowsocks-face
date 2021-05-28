#ifndef SHAREDIALOG_H
#define SHAREDIALOG_H


#include "model/ssconfig.h"

namespace Ui
{
class ShareDialog;
}

class ShareDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShareDialog(const SsConfig &config, QWidget *parent = nullptr);
    ~ShareDialog();

private:
    Ui::ShareDialog *ui;
};

#endif // SHAREDIALOG_H
