#ifndef CONFIG_H
#define CONFIG_H

#include "pch.hpp"

class Config {
public:
    // data
    int id = 0;
    QString remarks;
    QString server;
    int server_port;
    QString local_address = "127.0.0.1";
    int local_port = 1080;
    QString password;
    int timeout = 5;
    QString method;
    bool fastopen = false;
    QString mode = "tcp_only";

    // method
    QJsonObject toJsonObject() const;
    static Config fromJsonObject(const QJsonObject &json);
    QString fileName() const;
};

#endif // CONFIG_H
