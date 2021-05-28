#ifndef SSCONFIGSTORAGE_H
#define SSCONFIGSTORAGE_H


#include "entity/SSConfig.h"

class ConfigManager: public QObject
{
    Q_OBJECT

public:
    explicit ConfigManager(QString dirPath, QObject *parent = nullptr);

public slots:
    void add(SSConfig &config); // may change id
    void remove(const SSConfig &config);
    void edit(const SSConfig &config);
    QList<SSConfig> query();
    void importGUIConfig(QString guiConfigPath);
    void exportGUIConfig(QString guiConfigPath);

private:
    QDir configDir;
    int incCnt = 0;
    void saveConfig(const SSConfig &config);
};

#endif // SSCONFIGSTORAGE_H
