#ifndef SSCONFIG_H
#define SSCONFIG_H


#include "tools/latencytester.h"

class SsConfig
{
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
    int latencyMs = NOTEST;

    // method
    QString getName() const;
    QJsonObject toJsonObject() const;
    static SsConfig fromJsonObject(const QJsonObject &json);
    QString toUri() const;
    QString fileName() const;
};

#endif // SSCONFIG_H
