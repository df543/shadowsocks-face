#ifndef SSCONFIGSTORAGE_H
#define SSCONFIGSTORAGE_H


#include "model/ssconfig.h"

class ConfigManager: public QObject
{
    Q_OBJECT

public:
    explicit ConfigManager(QString dirPath, QObject *parent = nullptr);

public slots:
    void add(SsConfig &config); // may change id
    void remove(const SsConfig &config);
    void edit(const SsConfig &config);
    QList<SsConfig> query();
    void importGUIConfig(QString guiConfigPath);
    void exportGUIConfig(QString guiConfigPath);

private:
    QDir configDir;
    int incCnt = 0;
    void saveConfig(const SsConfig &config);
};

#endif // SSCONFIGSTORAGE_H
