#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include "common.hpp"
#include "model/ssconfig.h"

namespace Ui
{
class EditDialog;
}

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    // config.id == 0 means new config.
    explicit EditDialog(SsConfig &config, QWidget *parent = nullptr);
    ~EditDialog();

private:
    Ui::EditDialog *ui;
    SsConfig &config;
    void onSave();
};

#endif // EDITDIALOG_H
