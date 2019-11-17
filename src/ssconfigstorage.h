#ifndef SSCONFIGSTORAGE_H
#define SSCONFIGSTORAGE_H

#include "pch.hpp"

class SSConfigStorage: public QObject {
    Q_OBJECT
public:
    explicit SSConfigStorage(QString dirPath, QObject *parent = nullptr);
    int size() const { return _configs.size(); }
    QMap<QString, QJsonObject>::const_iterator
    cbegin() const {
        return _configs.cbegin();
    }
    QMap<QString, QJsonObject>::const_iterator
    cend() const {
        return _configs.cend();
    }
    QMap<QString, QJsonObject>::const_iterator
    cget(QString name) const {
        return _configs.find(name);
    }

signals:
    void dataChanged();

public slots:
    void refresh();
    void add(QString name, QJsonObject config);
    void remove(QString name);
    void edit(QString new_name, QJsonObject new_config, QString old_name);
    void importGUIConfig(QString guiConfigPath);
    void exportGUIConfig(QString guiConfigPath);

private:
    QMap<QString, QJsonObject> _configs;
    QDir _configDir;
    void _add(QString name, QJsonObject config);
    void _remove(QString name);
};

#endif // SSCONFIGSTORAGE_H
