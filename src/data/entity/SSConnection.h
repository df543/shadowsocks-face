#ifndef SS_CONNECTION_H
#define SS_CONNECTION_H

#include "SSConfig.h"
#include "global.h"

class SSConnection: public QObject
{
    Q_OBJECT

public:
    const SSConfig ss_config;
    QProcess process;
    QStringList command;

    explicit SSConnection(const SSConfig &ss_config, QObject *parent = nullptr):
        QObject(parent), ss_config(ss_config), process(this)
    { connect(&process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &SSConnection::finished); }

    ~SSConnection() override
    {
        terminate();
        process.waitForFinished(-1);
    }

signals:
    void finished();

public slots:
    void start()
    {
        command = QProcess::splitCommand(global::settings["ss_client"].toString());
        if (command.empty())
            throw std::runtime_error("shadowsocks client program cannot be empty");

        if (global::settings["ss_command_type"].toString() == "json_file") {
            auto json_file = new QTemporaryFile(this);
            if (!json_file->open())
                throw std::runtime_error("cannot open temporary file");
            json_file->write(QJsonDocument(ss_config.toJsonObject()).toJson());
            json_file->close();
            command.append({"-c", json_file->fileName()});
        } else {
            throw std::runtime_error("unknown argument to call client");
        }

        process.setProgram(command[0]);
        process.setArguments(command.mid(1));
        process.start();
    }

    void terminate()
    { process.kill(); }
};

#endif // SS_CONNECTION_H
