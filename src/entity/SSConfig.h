#ifndef SSCONFIG_H
#define SSCONFIG_H

#include "tools/latencytester.h"

class SSConfig
{
public:
    qint64 id = 0;
    QString remarks;
    QString server_address;
    int server_port;
    QString local_address = "127.0.0.1";
    int local_port = 1080;
    QString password;
    int timeout = 5;
    QString method;
    bool fastopen = false;
    QString mode = "tcp_only";
    int latencyMs = NOTEST;

    QString getName() const
    {
        if (remarks.trimmed().isEmpty())
            return QString("%1:%2").arg(server_address).arg(server_port);
        else return remarks;
    }

    QString toUri() const
    {
        QString userInfo = QString("%1:%2").arg(method, password).toUtf8().toBase64();
        QString res = QString("ss://%1@%2:%3").arg(userInfo, server_address).arg(server_port);
        if (!remarks.trimmed().isEmpty())
            res += "#" + remarks.toUtf8().toPercentEncoding();
        return res;
    }

    QString fileName() const
    {
        return QString("%1.json").arg(id);
    }

    QJsonObject toJsonObject() const
    {
        QJsonObject ret;
        ret["id"] = id;
        ret["remarks"] = remarks;
        ret["server"] = server_address;
        ret["server_port"] = server_port;
        ret["local_address"] = local_address;
        ret["local_port"] = local_port;
        ret["password"] = password;
        ret["timeout"] = timeout;
        ret["method"] = method;
        ret["fastopen"] = fastopen;
        ret["mode"] = mode;
        return ret;
    }

    static SSConfig fromJsonObject(const QJsonObject &json)
    {
        SSConfig ret;
        if (json.contains("id")) ret.id = json["id"].toInt();
        ret.remarks = json["remarks"].toString();
        ret.server_address = json["server"].toString();
        ret.server_port = json["server_port"].toInt();
        if (json.contains("local_address")) ret.local_address = json["local_address"].toString();
        if (json.contains("local_port")) ret.local_port = json["local_port"].toInt();
        ret.password = json["password"].toString();
        if (json.contains("timeout")) ret.timeout = json["timeout"].toInt();
        ret.method = json["method"].toString();
        if (json.contains("fastopen")) ret.fastopen = json["fastopen"].toBool();
        if (json.contains("mode")) ret.mode = json["mode"].toString();
        return ret;
    }
};

#endif // SSCONFIG_H
