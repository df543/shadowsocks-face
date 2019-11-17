#include "ssconfigstorage.h"

SSConfigStorage::SSConfigStorage(QString dirPath, QObject *parent): QObject(parent), _configDir(dirPath) {
    if (!_configDir.exists())
        throw std::runtime_error("invalid config dir");

    _configDir.setNameFilters({"*.json"});
    refresh();
}

void SSConfigStorage::_add(QString name, QJsonObject config) {
    _configDir.refresh();

    QString endName = name;
    for (int i = 1; _configDir.exists(endName + ".json"); i++)
        endName = QString("%1_%2").arg(name).arg(i);

    QFile jsonFile(_configDir.filePath(endName + ".json"));
    if (!jsonFile.open(QIODevice::WriteOnly))
        throw std::runtime_error("couldn't write file");
    jsonFile.write(QJsonDocument(config).toJson());
    jsonFile.close();

    _configs[endName] = config;
}

void SSConfigStorage::_remove(QString name) {
    if (_configs.find(name) == _configs.end())
        throw std::runtime_error("config not exists");

    _configDir.remove(name + ".json");
    _configs.remove(name);
}

void SSConfigStorage::refresh() {
    _configs.clear();
    _configDir.refresh();
    for (auto i : _configDir.entryInfoList()) {
        QFile jsonFile(i.filePath());
        if (!jsonFile.open(QIODevice::ReadOnly))
            throw std::runtime_error("couldn't open config file");
        _configs[i.baseName()] = QJsonDocument::fromJson(jsonFile.readAll()).object();
        jsonFile.close();
    }
    emit dataChanged();
}

void SSConfigStorage::add(QString name, QJsonObject config) {
    _add(name, config);
    emit dataChanged();
}

void SSConfigStorage::remove(QString name) {
    _remove(name);
    emit dataChanged();
}

void SSConfigStorage::edit(QString new_name, QJsonObject new_config, QString old_name) {
    _remove(old_name);
    _add(new_name, new_config);
    emit dataChanged();
}

void SSConfigStorage::importGUIConfig(QString guiConfigPath) {
    QFile f{guiConfigPath};
    f.open(QIODevice::ReadOnly);
    QJsonObject json = QJsonDocument::fromJson(f.readAll()).object();
    f.close();

    int localPort = json.find("localPort")->toVariant().toInt();
    if (localPort == 0) localPort = 1080;
    bool shareOverLan = json.find("shareOverLan")->toBool();
    QJsonArray jsonConfigs = json.find("configs")->toArray();

    for (auto i : jsonConfigs) {
        QJsonObject newConfig, readConfig = i.toObject();
        for (auto s : {"method", "password", "server", "server_port"})
            newConfig[s] = readConfig[s];
        newConfig["local_port"] = localPort;
        if (shareOverLan)
            newConfig["local_address"] = "0.0.0.0";
        QString name = readConfig["remarks"].toString();
        _add(name, newConfig);
    }
    emit dataChanged();
}

void SSConfigStorage::exportGUIConfig(QString guiConfigPath) {
    QJsonArray jsonConfigs;
    for (auto i = _configs.begin(); i != _configs.end(); ++i) {
        QJsonObject oneConfig;
        for (auto s : {"method", "password", "server", "server_port"})
            oneConfig[s] = (*i)[s];
        oneConfig["remarks"] = i.key();
        jsonConfigs.push_back(oneConfig);
    }

    QJsonObject json;
    json["configs"] = jsonConfigs;
    json["localPort"] = 1080;
    json["shareOverLan"] = false;

    QFile f{guiConfigPath};
    f.open(QIODevice::WriteOnly);
    f.write(QJsonDocument{json}.toJson());
    f.close();
}
