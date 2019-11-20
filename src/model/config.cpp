#include "config.h"

QJsonObject Config::toJsonObject() const {
    QJsonObject ret;
    ret["id"] = id;
    ret["remarks"] = remarks;
    ret["server"] = server;
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

Config Config::fromJsonObject(const QJsonObject &json) {
    Config ret;
    if (json.contains("id")) ret.id = json["id"].toInt();
    ret.remarks = json["remarks"].toString();
    ret.server = json["server"].toString();
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

QString Config::fileName() const {
    return QString("%1.json").arg(id);
}
