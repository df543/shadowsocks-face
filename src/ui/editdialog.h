#ifndef EDITDIALOG_H
#define EDITDIALOG_H


#include "entity/SSConfig.h"

namespace Ui
{
class EditDialog;
}

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    // config.id == 0 means new config.
    explicit EditDialog(SSConfig &config, QWidget *parent = nullptr);
    ~EditDialog();

private:
    Ui::EditDialog *ui;
    SSConfig &config;
    void onSave();
};

#endif // EDITDIALOG_H
