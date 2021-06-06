#ifndef CONNECTION_MODEL_H
#define CONNECTION_MODEL_H

#include <entity/SSConnection.h>

class ConnectionModel: public QAbstractTableModel
{
    Q_OBJECT

private:
    QList<SSConnection *> connections;
    QStringList header = { tr("Name"), tr("Server"), tr("Local") };

public:
    enum OutputType {
        STDOUT, STDERR
    };

signals:
    void output(const SSConfig &config, OutputType outputType, const QString &msg);

public slots:
    void add(const SSConfig &config)
    {
        auto connection = new SSConnection(config, this);
        connect(connection, &SSConnection::finished, [this, connection] {
            int row = connections.indexOf(connection);
            if (row == -1) return;
            beginRemoveRows(QModelIndex(), row, row);
            connections.removeAt(row);
            endRemoveRows();
            connection->deleteLater();
        });
        connect(&connection->process, &QProcess::readyReadStandardOutput, [this, connection] {
            emit output(connection->ss_config, OutputType::STDOUT, connection->process.readAllStandardOutput());
        });
        connect(&connection->process, &QProcess::readyReadStandardError, [this, connection] {
            emit output(connection->ss_config, OutputType::STDERR, connection->process.readAllStandardError());
        });
        connection->start();
        beginInsertRows(QModelIndex(), connections.size(), connections.size());
        connections.append(connection);
        endInsertRows();
    }

    void del(const QModelIndex &i)
    {
        if (!i.parent().isValid() && i.row() >= 0 && i.row() < connections.size())
            connections[i.row()]->terminate();
    }

public:
    explicit ConnectionModel(QObject *parent = nullptr):
        QAbstractTableModel(parent) {}

    int rowCount(const QModelIndex &parent) const override
    { return parent.isValid() ? 0 : connections.size(); }

    int columnCount(const QModelIndex &) const override
    { return header.size(); }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override
    {
        if (role == Qt::DisplayRole && orientation == Qt::Orientation::Horizontal && section >= 0 && section < header.size())
            return header[section];
        return QVariant();
    }

    QVariant data(const QModelIndex &index, int role) const override
    {
        if (role == Qt::DisplayRole && !index.parent().isValid()
            && index.column() >= 0 && index.column() < header.size()
            && index.row() >= 0 && index.row() < connections.size()
           ) {
            const SSConfig &config = connections[index.row()]->ss_config;
            switch (index.column()) {
            case 0:
                return config.name;
            case 1:
                return QString("%1:%2").arg(config.server_address).arg(config.server_port);
            case 2:
                if (config.local_address == "127.0.0.1")
                    return config.local_port;
                else
                    return QString("%1:%2").arg(config.local_address).arg(config.local_port);
            default:
                throw std::runtime_error("unknown column");
            }
        }
        return QVariant();
    }
};

#endif // CONNECTION_MODEL_H
