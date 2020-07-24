#include "onceguard.h"

bool OnceGuard::lock()
{
    QLocalSocket socket;
    socket.connectToServer(name);
    if (socket.waitForConnected(500))
        return false;

    QLocalServer *server = new QLocalServer(this);
    server->setSocketOptions(QLocalServer::WorldAccessOption);
    connect(server, &QLocalServer::newConnection, this, &OnceGuard::knocked);
    if (server->listen(name))
        return true;
    else
        throw std::runtime_error("can not start local server");
}
