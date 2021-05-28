#ifndef SSPROCESSLIST_H
#define SSPROCESSLIST_H


#include "entity/SSConfig.h"

class ProcessManager: public QObject
{
    Q_OBJECT

public:
    explicit ProcessManager(QString dirPath, QObject *parent = nullptr);
    QProcess *start(qint64 id);
    void terminate(qint64 id);
    bool isRunning(qint64 id);

signals:
    void procChanged();

private:
    QHash<qint64, QProcess *> processes;
    QDir configDir;
};

#endif // SSPROCESSLIST_H
