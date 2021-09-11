#ifndef GLOBAL_H
#define GLOBAL_H

#include "version.h"
#include "data/dao/KeyValueDAO.h"
#include "data/file/LocalFiles.h"

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
inline LocalFiles *localFiles = nullptr;

inline void init(QObject *parent)
{
    // init data path
    localFiles = new LocalFiles(parent);
    localFiles->clearTempConfig();

    // init database
    QString dbName = "saves.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(localFiles->saveDir().absoluteFilePath(dbName));
    if (!db.open())
        throw std::runtime_error("db open error");

    // init key-value storage
    kvDAO = new KeyValueDAO(parent);

    // sync settings
    // read
    auto savedSettingsString = kvDAO->get("settings").toUtf8();
    if (savedSettingsString.isEmpty()) {
        isFirstStart = true;
    } else {
        auto loadSettings = QJsonDocument::fromJson(savedSettingsString).object().toVariantHash();
        settings.insert(loadSettings);
    }
    // save
    kvDAO->set("settings", QJsonDocument(QJsonObject::fromVariantHash(settings)).toJson());
}

}

#endif // GLOBAL_H
