#ifndef GLOBAL_H
#define GLOBAL_H

#include "version.h"

namespace global
{

inline const QString name = "Shadowsocks Face";
inline const QString shortName = "ss-face";
inline const QString version = SS_FACE_APP_VERSION;

inline QVariantHash settings{
    {"ss_client", "sslocal-rust --log-without-time"},
    {"ss_command_type", "json_file"}
};

inline void init() {
    QString savePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QString dbName = "saves.db";

    if (!QDir(savePath).mkpath("."))
        throw std::runtime_error("cannot create save path");
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir(savePath).filePath(dbName));
    if (!db.open())
        throw std::runtime_error("db open error");
}

}

#endif // GLOBAL_H
