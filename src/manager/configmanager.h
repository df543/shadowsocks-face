#ifndef SSCONFIGSTORAGE_H
#define SSCONFIGSTORAGE_H

#include "pch.hpp"
#include "model/config.h"

class ConfigManager: public QObject
{
    Q_OBJECT

public:
    explicit ConfigManager(QString dirPath, QObject *parent = nullptr);

public slots:
    void add(Config &config); // may change id
    void remove(const Config &config);
    void edit(const Config &config);
    QList<Config> query();
    void importGUIConfig(QString guiConfigPath);
    void exportGUIConfig(QString guiConfigPath);

private:
    QDir configDir;
    int incCnt = 0;
    void saveConfig(const Config &config);
};

#endif // SSCONFIGSTORAGE_H
