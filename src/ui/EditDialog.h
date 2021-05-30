#ifndef EDIT_DIALOG_H
#define EDIT_DIALOG_H

#include "entity/SSConfig.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EditDialog; }
QT_END_NAMESPACE

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    // config.id == 0 means new config.
    explicit EditDialog(SSConfig &config, QWidget *parent = nullptr);
    ~EditDialog() override;

private:
    Ui::EditDialog *ui;
    SSConfig &config;
    void onSave();
};

#endif // EDIT_DIALOG_H
