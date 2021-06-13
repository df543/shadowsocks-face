#ifndef CONNECTION_MODEL_H
#define CONNECTION_MODEL_H

#include "data/entity/SSConnection.h"

class ConnectionModel: public QAbstractTableModel
{
    Q_OBJECT

private:
    QList<SSConnection *> connections;
    QStringList header = { tr("Server"), tr("Local"), tr("Latency"), tr("PID") };

    bool checkIndex(const QModelIndex &i) const
    {
        return !i.parent().isValid()                                // parent is root
               && i.row() >= 0 && i.row() < connections.size()      // row
               && i.column() >= 0 && i.column() < header.size();    // column
    }

signals:
    void output(const SSConfig &config, SSConnection::OutputType outputType, const QString &msg);

public slots:
    void add(const SSConfig &config)
    {
        auto connection = new SSConnection(config, this);
        connect(connection, &SSConnection::terminated, [this, connection] {
            auto row = connections.indexOf(connection);
            if (row == -1) return;
            beginRemoveRows(QModelIndex(), row, row);
            connections.removeAt(row);
            endRemoveRows();
            connection->deleteLater();
        });
        connect(connection, &SSConnection::output, this, &ConnectionModel::output);
        connect(connection, &SSConnection::latencyTestFinished, [this] {
            emit dataChanged(QModelIndex(), QModelIndex());
        });
        beginInsertRows(QModelIndex(), connections.size(), connections.size());
        connections.append(connection);
        endInsertRows();
        connection->start();
    }

    void del(const QModelIndex &i)
    {
        if (checkIndex(i))
            connections[i.row()]->terminate();
    }

    void testLatency(const QModelIndex &i)
    {
        if (checkIndex(i))
            connections[i.row()]->testLatency();
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
        if (checkIndex(index)) {
            const auto &connection = connections[index.row()];
            if (role == Qt::DisplayRole) {
                switch (index.column()) {
                case 0:
                    return connection->ss_config.getName();
                case 1:
                    return connection->ss_config.getLocal();
                case 2:
                    switch (connection->latency_ms) {
                    case SSConnection::LatencyType::NO_TEST:
                        return QVariant();
                    case SSConnection::LatencyType::TEST_ERROR:
                        return tr("Test Error");
                    default:
                        return QString("%1 ms").arg(connection->latency_ms);
                    }
                case 3:
                    return connection->process.processId();
                }
            }
        }
        return QVariant();
    }
};

#endif // CONNECTION_MODEL_H
