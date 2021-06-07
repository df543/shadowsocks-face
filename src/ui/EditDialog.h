#ifndef EDIT_DIALOG_H
#define EDIT_DIALOG_H

#include "data/entity/SSConfig.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EditDialog; }
QT_END_NAMESPACE

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    enum DialogType {
        NEW_CONFIG, EDIT_CONFIG
    };
    explicit EditDialog(const SSConfig &config, DialogType dialogType,QWidget *parent = nullptr);
    ~EditDialog() override;

signals:
    void saveConfig(SSConfig &config);

private:
    Ui::EditDialog *ui;
    SSConfig config;

private slots:
    void on_buttonBox_accepted();
};

#endif // EDIT_DIALOG_H
