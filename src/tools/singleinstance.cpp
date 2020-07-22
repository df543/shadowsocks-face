#include "singleinstance.h"

bool SingleInstance::runGuard()
{
    QLocalSocket socket;
    socket.connectToServer(_key);
    if (socket.waitForConnected(500))
        return false;

    QLocalServer *server = new QLocalServer(this);
    server->setSocketOptions(QLocalServer::WorldAccessOption);
    connect(server, &QLocalServer::newConnection, this, &SingleInstance::newInstance);
    if (server->listen(_key)) {
        if (server->serverError() == QAbstractSocket::AddressInUseError && QFile::exists(server->serverName())) {
            QFile::remove(server->serverName());
            server->listen(_key);
        }
        return true;
    }
    throw std::runtime_error("local server start error");
}
