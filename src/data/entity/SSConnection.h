#ifndef SS_CONNECTION_H
#define SS_CONNECTION_H

#include "SSConfig.h"
#include "global.h"
#include "tools/ProxyLatencyTester.h"

class SSConnection: public QObject
{
    Q_OBJECT

public:
    const SSConfig ss_config;
    QProcess process;
    QStringList command;

    enum LatencyType {
        NO_TEST = -2, TEST_ERROR = -1
    };
    int latency_ms = LatencyType::NO_TEST;

    enum OutputType {
        STDOUT, STDERR
    };

    explicit SSConnection(const SSConfig &ss_config, QObject *parent = nullptr):
        QObject(parent), ss_config(ss_config), process(this)
    {
        connect(&process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &SSConnection::terminated);
        connect(&process, &QProcess::errorOccurred, [this](QProcess::ProcessError error) {
            if (error == QProcess::FailedToStart) {
                emit output(this->ss_config, OutputType::STDERR,
                            tr("Error: process failed to start\nCheck client command settings\n"));
                emit terminated();
            }
        });
        connect(&process, &QProcess::readyReadStandardOutput, [this] {
            emit output(this->ss_config, OutputType::STDOUT, process.readAllStandardOutput());
        });
        connect(&process, &QProcess::readyReadStandardError, [this] {
            emit output(this->ss_config, OutputType::STDERR, process.readAllStandardError());
        });
    }

    ~SSConnection() override
    {
        terminate();
        process.waitForFinished(-1);
    }

signals:
    void terminated();
    void output(const SSConfig &config, OutputType outputType, const QString &msg);
    void latencyTestFinished();

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

        QTimer::singleShot(300, this, &SSConnection::testLatency);
    }

    void terminate()
    { process.kill(); }

    void testLatency()
    {
        auto tester = new ProxyLatencyTester(
            QNetworkProxy(QNetworkProxy::Socks5Proxy, ss_config.local_address, ss_config.local_port),
            this);
        connect(tester, &ProxyLatencyTester::result, [this, tester](int _latency_ms) {
            if (_latency_ms < 0)
                latency_ms = LatencyType::TEST_ERROR;
            else
                latency_ms = _latency_ms;
            emit latencyTestFinished();
            tester->deleteLater();
        });
        tester->start_test();
    }
};

#endif // SS_CONNECTION_H
