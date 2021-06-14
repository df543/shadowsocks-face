#ifndef KEY_VALUE_DAO_H
#define KEY_VALUE_DAO_H

class KeyValueDAO: public QObject
{
    Q_OBJECT

public:
    explicit KeyValueDAO(QObject *parent = nullptr):
        QObject(parent), db(QSqlDatabase::database())
    {
        QSqlQuery query(db);
        query.prepare(sql_create_table);
        if (!query.exec())
            throw std::runtime_error("db create table error");
    }

    QString get(const QString &key)
    {
        QSqlQuery query(db);
        query.prepare(sql_select_by_key);
        query.bindValue(":key", key);
        if (!query.exec())
            throw std::runtime_error("select value error");

        if (query.next())
            return query.value(0).toString();
        else
            return QString();
    }

    void set(const QString &key, const QString &value)
    {
        QSqlQuery query(db);
        query.prepare(sql_replace);
        query.bindValue(":key", key);
        query.bindValue(":value", value);
        if (!query.exec())
            throw std::runtime_error("replace key-value error");
    }

private:
    QSqlDatabase db;
    QString tableName = "kv_store";

    QString sql_create_table =
        QString("CREATE TABLE IF NOT EXISTS %1 ("
                "key TEXT PRIMARY KEY,"
                "value TEXT);").arg(tableName);
    QString sql_select_by_key =
        QString("SELECT value FROM %1 where key=:key;").arg(tableName);
    QString sql_replace =
        QString("REPLACE INTO %1 (key, value) VALUES (:key, :value);").arg(tableName);
};

#endif // KEY_VALUE_DAO_H
