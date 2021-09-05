#ifndef GLOBAL_H
#define GLOBAL_H

#include "version.h"
#include "data/dao/KeyValueDAO.h"

namespace global
{

inline const QString name = "Shadowsocks Face";
inline const QString shortName = "ss-face";
inline const QString version = SS_FACE_APP_VERSION;

inline const QVariantHash defaultSettings{
    {"ss_client", "sslocal-rust --log-without-time"},
    {"ss_command_type", "json_file"}
};

inline QVariantHash settings = defaultSettings;
inline bool isFirstStart = false;

inline KeyValueDAO *kvDAO = nullptr;

inline QString savePath()
{
    QString res = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    if (!QDir(res).mkpath("."))
        throw std::runtime_error("cannot create save path");
    return res;
}

inline void init(QObject *parent)
{
    // database
    QString dbName = "saves.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir(savePath()).filePath(dbName));
    if (!db.open())
        throw std::runtime_error("db open error");

    // key-value storage
    kvDAO = new KeyValueDAO(parent);

    // read settings
    auto savedSettingsString = kvDAO->get("settings").toUtf8();
    if (savedSettingsString.isEmpty()) {
        isFirstStart = true;
    } else {
        auto loadSettings = QJsonDocument::fromJson(savedSettingsString).object().toVariantHash();
        settings.insert(loadSettings);
    }

    // save settings
    kvDAO->set("settings", QJsonDocument(QJsonObject::fromVariantHash(settings)).toJson());
}

}

#endif // GLOBAL_H
