#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include "pch.hpp"

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog {
    Q_OBJECT
public:
    explicit EditDialog(QString name, QJsonObject config, bool isNew, QWidget *parent = nullptr);
    ~EditDialog();

signals:
    void submit(QString new_name, QJsonObject new_config, QString old_name);

private:
    Ui::EditDialog *ui;
    QString old_name;
    void onSave();
};

#endif // EDITDIALOG_H
