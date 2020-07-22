#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include "common.hpp"
#include "model/config.h"

namespace Ui
{
class EditDialog;
}

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    // config.id == 0 means new config.
    explicit EditDialog(Config &config, QWidget *parent = nullptr);
    ~EditDialog();

private:
    Ui::EditDialog *ui;
    Config &config;
    void onSave();
};

#endif // EDITDIALOG_H
