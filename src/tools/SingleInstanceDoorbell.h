#ifndef SINGLE_INSTANCE_DOORBELL_H
#define SINGLE_INSTANCE_DOORBELL_H

class SingleInstanceDoorbell: public QObject
{
    Q_OBJECT

public:
    explicit SingleInstanceDoorbell(const QString &id, QObject *parent = nullptr):
        QObject(parent), id(id) {}

    ~SingleInstanceDoorbell() override
    { delete server; }

    bool setup()
    {
        // try to ping the previous server
        QLocalSocket socket;
        socket.connectToServer(id);
        if (socket.waitForConnected(500))
            return false;

        // setup a new server
        server = new QLocalServer(this);
        server->setSocketOptions(QLocalServer::WorldAccessOption);
        connect(server, &QLocalServer::newConnection, this, &SingleInstanceDoorbell::rang);
        if (server->listen(id))
            return true;
        else
            throw std::runtime_error("can not start local server");
    }

signals:
    void rang();

private:
    QString id;
    QLocalServer *server = nullptr;
};

#endif // SINGLE_INSTANCE_DOORBELL_H
