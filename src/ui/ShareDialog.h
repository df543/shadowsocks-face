#ifndef SHARE_DIALOG_H
#define SHARE_DIALOG_H

#include "entity/SSConfig.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ShareDialog; }
QT_END_NAMESPACE

class ShareDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShareDialog(const SSConfig &config, QWidget *parent = nullptr);
    ~ShareDialog() override;

private:
    Ui::ShareDialog *ui;
};

#endif // SHARE_DIALOG_H
