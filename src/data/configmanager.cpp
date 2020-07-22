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

void ConfigManager::saveConfig(const Config &config)
{
    QFile jsonFile(configDir.filePath(config.fileName()));
    if (!jsonFile.open(QIODevice::WriteOnly))
        throw std::runtime_error("couldn't write file");
    jsonFile.write(QJsonDocument(config.toJsonObject()).toJson());
    jsonFile.close();
}

void ConfigManager::add(Config &config)
{
    configDir.refresh();
    config.id = incCnt++;
    saveConfig(config);
}

void ConfigManager::remove(const Config &config)
{
    configDir.refresh();
    configDir.remove(config.fileName());
}

void ConfigManager::edit(const Config &config)
{
    configDir.refresh();
    saveConfig(config);
}

QList<Config> ConfigManager::query()
{
    configDir.refresh();
    QList<Config> ret;
    for (auto i : configDir.entryInfoList()) {
        QFile jsonFile(i.filePath());
        if (!jsonFile.open(QIODevice::ReadOnly))
            throw std::runtime_error("couldn't open config file");
        ret.append(Config::fromJsonObject(QJsonDocument::fromJson(jsonFile.readAll()).object()));
        jsonFile.close();
    }
    return ret;
}

void ConfigManager::importGUIConfig(QString guiConfigPath)
{
    QFile f{guiConfigPath};
    f.open(QIODevice::ReadOnly);
    QJsonObject json = QJsonDocument::fromJson(f.readAll()).object();
    f.close();

    int localPort = json.find("localPort")->toVariant().toInt();
    if (localPort == 0) localPort = 1080;
    bool shareOverLan = json.find("shareOverLan")->toBool();
    QJsonArray jsonConfigs = json.find("configs")->toArray();

    for (auto i : jsonConfigs) {
        Config toAdd = Config::fromJsonObject(i.toObject());
        toAdd.local_port = localPort;
        if (shareOverLan)
            toAdd.local_address = "0.0.0.0";
        add(toAdd);
    }
}

void ConfigManager::exportGUIConfig(QString guiConfigPath)
{
    QJsonArray jsonConfigs;
    for (const auto &i : query()) {
        QJsonObject oneConfig;
        oneConfig["method"] = i.method;
        oneConfig["password"] = i.password;
        oneConfig["server"] = i.server;
        oneConfig["server_port"] = i.server_port;
        oneConfig["remarks"] = i.remarks;
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

