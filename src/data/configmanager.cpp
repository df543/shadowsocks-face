#include "configmanager.h"

ConfigManager::ConfigManager(QString dirPath, QObject *parent)
    : QObject(parent), configDir(dirPath)
{
    if (!configDir.exists())
        throw std::runtime_error("invalid config dir");

    configDir.setNameFilters({"*.json"});
    for (auto i : configDir.entryInfoList())
        incCnt = std::max(incCnt, i.baseName().toInt());
    incCnt++;
}

void ConfigManager::saveConfig(const SSConfig &config)
{
    QFile jsonFile(configDir.filePath(QString("%1.json").arg(config.id)));
    if (!jsonFile.open(QIODevice::WriteOnly))
        throw std::runtime_error("couldn't write file");
    jsonFile.write(QJsonDocument(config.toJsonObject()).toJson());
    jsonFile.close();
}

void ConfigManager::add(SSConfig &config)
{
    configDir.refresh();
    config.id = incCnt++;
    saveConfig(config);
}

void ConfigManager::remove(const SSConfig &config)
{
    configDir.refresh();
    configDir.remove(QString("%1.json").arg(config.id));
}

void ConfigManager::edit(const SSConfig &config)
{
    configDir.refresh();
    saveConfig(config);
}

QList<SSConfig> ConfigManager::query()
{
    configDir.refresh();
    QList<SSConfig> ret;
    for (auto i : configDir.entryInfoList()) {
        QFile jsonFile(i.filePath());
        if (!jsonFile.open(QIODevice::ReadOnly))
            throw std::runtime_error("couldn't open config file");
        ret.append(SSConfig::fromJsonObject(QJsonDocument::fromJson(jsonFile.readAll()).object()));
        jsonFile.close();
    }
    return ret;
}

void ConfigManager::importGUIConfig(const QString &guiConfigPath)
{
    QFile f(guiConfigPath);
    f.open(QIODevice::ReadOnly);
    QJsonObject json = QJsonDocument::fromJson(f.readAll()).object();
    QJsonArray jsonConfigs = json.find("configs")->toArray();

    for (auto i : jsonConfigs) {
        SSConfig toAdd = SSConfig::fromJsonObject(i.toObject());
        add(toAdd);
    }
}

void ConfigManager::exportGUIConfig(const QString &guiConfigPath)
{
    QJsonArray jsonConfigs;
    for (const auto &i : query())
        jsonConfigs.push_back(i.toJsonObject());
    QJsonObject json;
    json["configs"] = jsonConfigs;

    QFile f(guiConfigPath);
    f.open(QIODevice::WriteOnly);
    f.write(QJsonDocument{json}.toJson());
}
