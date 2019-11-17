#ifndef SSPROCESSLIST_H
#define SSPROCESSLIST_H

#include "pch.hpp"

class SSProcessList: public QObject {
    Q_OBJECT
public:
    explicit SSProcessList(QString dirPath, QObject *parent = nullptr);
    QProcess *start(QString name);
    void terminate(QString name);
    bool isRunning(QString name);

signals:
    void procChanged();

private:
    QHash<QString, QProcess *> _processes;
    QDir _configDir;
};

#endif // SSPROCESSLIST_H
