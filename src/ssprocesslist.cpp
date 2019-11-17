#include "ssprocesslist.h"

SSProcessList::SSProcessList(QString dirPath, QObject *parent): QObject(parent), _configDir(dirPath) {
    if (!_configDir.exists())
        throw std::runtime_error("invalid config dir");
}

QProcess *SSProcessList::start(QString name) {
    QFileInfo json(_configDir.filePath(name + ".json"));
    if (!json.exists())
        return 0;
    QProcess *ssproc = new QProcess(this);
    _processes[name] = ssproc;
    connect(ssproc, &QProcess::started, this, &SSProcessList::procChanged);
    connect(ssproc, static_cast<void (QProcess::*)(int)>(&QProcess::finished), this, &SSProcessList::procChanged);
    ssproc->start("ss-local", {"-c", json.filePath()});
    return ssproc;
}

void SSProcessList::terminate(QString name) {
    if (_processes.find(name) == _processes.end())
        throw std::runtime_error("process not found");
    _processes[name]->kill();
    _processes.remove(name);
}

bool SSProcessList::isRunning(QString name) {
    if (_processes.find(name) != _processes.end()) {
        if (_processes[name]->state() != QProcess::NotRunning)
            return true;
        else
            _processes.remove(name);
    }
    return false;
}
