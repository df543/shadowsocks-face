#include "processmanager.h"

ProcessManager::ProcessManager(QString dirPath, QObject *parent)
    : QObject(parent), configDir(dirPath)
{
    if (!configDir.exists())
        throw std::runtime_error("invalid config dir");
}

QProcess *ProcessManager::start(int id)
{
    QFileInfo json(configDir.filePath(QString("%1.json").arg(id)));
    if (!json.exists())
        return nullptr;
    QProcess *ssproc = new QProcess(this);
    processes[id] = ssproc;
    connect(ssproc, &QProcess::started, this, &ProcessManager::procChanged);
    connect(ssproc, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &ProcessManager::procChanged);
    ssproc->start("ss-local", {"-c", json.filePath()});
    return ssproc;
}

void ProcessManager::terminate(int id)
{
    if (processes.find(id) == processes.end())
        throw std::runtime_error("process not found");
    processes[id]->kill();
    processes.remove(id);
}

bool ProcessManager::isRunning(int id)
{
    if (processes.find(id) != processes.end()) {
        if (processes[id]->state() != QProcess::NotRunning)
            return true;
        else
            processes.remove(id);
    }
    return false;
}
