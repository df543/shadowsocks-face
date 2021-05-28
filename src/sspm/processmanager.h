#ifndef SSPROCESSLIST_H
#define SSPROCESSLIST_H


#include "model/ssconfig.h"

class ProcessManager: public QObject
{
    Q_OBJECT

public:
    explicit ProcessManager(QString dirPath, QObject *parent = nullptr);
    QProcess *start(int id);
    void terminate(int id);
    bool isRunning(int id);

signals:
    void procChanged();

private:
    QHash<int, QProcess *> processes;
    QDir configDir;
};

#endif // SSPROCESSLIST_H
