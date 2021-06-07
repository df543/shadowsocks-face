#ifndef CONNECTION_MODEL_H
#define CONNECTION_MODEL_H

#include "data/entity/SSConnection.h"

class ConnectionModel: public QAbstractTableModel
{
    Q_OBJECT

private:
    QList<SSConnection *> connections;
    QStringList header = { tr("Server"), tr("Local") };

    bool checkIndex(const QModelIndex &i) const
    {
        return !i.parent().isValid()                                // parent is root
               && i.row() >= 0 && i.row() < connections.size()      // row
               && i.column() >= 0 && i.column() < header.size();    // column
    }

public:
    enum OutputType {
        STDOUT, STDERR
    };

signals:
    void output(const SSConfig &config, ConnectionModel::OutputType outputType, const QString &msg);

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
            const SSConfig &config = connections[index.row()]->ss_config;
            switch (index.column()) {
            case 0:
                if (role == Qt::DisplayRole)
                    return config.getName();
                break;
            case 1:
                if (role == Qt::DisplayRole)
                    return config.getLocal();
                break;
            default:
                throw std::runtime_error("unknown column");
            }
        }
        return QVariant();
    }
};

#endif // CONNECTION_MODEL_H
