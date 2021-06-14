#ifndef SS_CONFIG_DAO_H
#define SS_CONFIG_DAO_H

#include "data/entity/SSConfig.h"
#include "global.h"

class SSConfigDAO: public QObject
{
    Q_OBJECT

public:
    explicit SSConfigDAO(QObject *parent = nullptr):
        QObject(parent), db(QSqlDatabase::database())
    {
        QSqlQuery query(db);
        query.prepare(sql_create_table);
        if (!query.exec())
            throw std::runtime_error("db create table error");
    }

    QList<SSConfig> selectAll()
    {
        QSqlQuery query(db);
        query.prepare(sql_select_all);
        if (!query.exec())
            throw std::runtime_error("select record error");

        QList<SSConfig> res;
        while (query.next())
            res.append(fromSqlQuery(query));
        return res;
    }

    decltype(SSConfig::id) insert(const SSConfig &config)
    {
        QSqlQuery query(db);
        query.prepare(sql_insert);
        query.bindValue(":method",          config.method);
        query.bindValue(":password",        config.password);
        query.bindValue(":server_address",  config.server_address);
        query.bindValue(":server_port",     config.server_port);
        query.bindValue(":name",            config.name);
        query.bindValue(":local_address",   config.local_address);
        query.bindValue(":local_port",      config.local_port);
        query.bindValue(":mode",            config.mode);
        query.bindValue(":timeout",         config.timeout);
        query.bindValue(":fast_open",       config.fast_open);
        if (!query.exec())
            throw std::runtime_error("insert record error");
        return query.lastInsertId().toLongLong();
    }

    void update(const SSConfig &config)
    {
        QSqlQuery query(db);
        query.prepare(sql_update);
        query.bindValue(":id", config.id); // id is valid when update
        query.bindValue(":method",          config.method);
        query.bindValue(":password",        config.password);
        query.bindValue(":server_address",  config.server_address);
        query.bindValue(":server_port",     config.server_port);
        query.bindValue(":name",            config.name);
        query.bindValue(":local_address",   config.local_address);
        query.bindValue(":local_port",      config.local_port);
        query.bindValue(":mode",            config.mode);
        query.bindValue(":timeout",         config.timeout);
        query.bindValue(":fast_open",       config.fast_open);
        if (!query.exec())
            throw std::runtime_error("update record error");
    }

    void deleteById(decltype(SSConfig::id) id)
    {
        QSqlQuery query(db);
        query.prepare(sql_delete_by_id);
        query.bindValue(":id", id);
        if (!query.exec())
            throw std::runtime_error("delete record error");
    }

private:
    QSqlDatabase db;
    QString tableName = "config";

    QString sql_create_table =
        QString("CREATE TABLE IF NOT EXISTS %1 ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "method TEXT, "
                "password TEXT, "
                "server_address TEXT, "
                "server_port INTEGER, "
                "name TEXT, "
                "local_address TEXT, "
                "local_port INTEGER, "
                "mode TEXT, "
                "timeout INTEGER, "
                "fast_open INTEGER);").arg(tableName);
    QString sql_select_all =
        QString("SELECT id, method, password, server_address, server_port, name, "
                "local_address, local_port, mode, timeout, fast_open FROM %1;").arg(tableName);
    QString sql_insert =
        QString("INSERT INTO %1 (method, password, server_address, server_port, name, "
                "local_address, local_port, mode, timeout, fast_open) VALUES (:method, "
                ":password, :server_address, :server_port, :name, :local_address, :local_port, "
                ":mode, :timeout, :fast_open);").arg(tableName);
    QString sql_update =
        QString("UPDATE %1 SET method=:method, password=:password, "
                "server_address=:server_address, server_port=:server_port, "
                "name=:name, local_address=:local_address, local_port=:local_port, "
                "mode=:mode, timeout=:timeout, fast_open=:fast_open WHERE id=:id;").arg(tableName);
    QString sql_delete_by_id =
        QString("DELETE FROM %1 WHERE id=:id;").arg(tableName);

    static SSConfig fromSqlQuery(const QSqlQuery &query)
    {
        SSConfig res;

        res.id              = query.value(0).toInt();
        res.method          = query.value(1).toString();
        res.password        = query.value(2).toString();
        res.server_address  = query.value(3).toString();
        res.server_port     = query.value(4).toInt();
        res.name            = query.value(5).toString();
        res.local_address   = query.value(6).toString();
        res.local_port      = query.value(7).toInt();
        res.mode            = query.value(8).toString();
        res.timeout         = query.value(9).toInt();
        res.fast_open       = query.value(10).toInt();

        return res;
    }
};

#endif // SS_CONFIG_DAO_H
