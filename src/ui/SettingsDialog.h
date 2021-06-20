#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

QT_BEGIN_NAMESPACE
namespace Ui { class SettingsDialog; }
QT_END_NAMESPACE

class SettingsDialog: public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog() override;

private:
    Ui::SettingsDialog *ui;

private slots:
    void on_buttonBox_accepted();
    void on_toolButton_client_clicked();
    void on_pushButton_default_clicked();
};

#endif // SETTINGS_DIALOG_H
