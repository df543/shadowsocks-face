#ifndef CONFIG_MODEL_H
#define CONFIG_MODEL_H

#include "data/dao/SSConfigDAO.h"

class ConfigModel: public QAbstractTableModel
{
    Q_OBJECT

private:
    QList<SSConfig> configs;
    QStringList header = { tr("Server"), tr("Local") };
    SSConfigDAO dao;

    bool checkIndex(const QModelIndex &i) const
    {
        return !i.parent().isValid()                                // parent is root
               && i.row() >= 0 && i.row() < configs.size()          // row
               && i.column() >= 0 && i.column() < header.size();    // column
    }

public slots:
    const SSConfig &getAt(const QModelIndex &i) const
    {
        if (checkIndex(i))
            return configs[i.row()];
        throw std::runtime_error("index not found");
    }

    void readAll()
    {
        beginResetModel();
        configs = dao.selectAll();
        endResetModel();
    }

    QList<SSConfig> getByIds(const QSet<decltype(SSConfig::id)> &idSet)
    {
        QList<SSConfig> res;
        for (const auto &i : configs)
            if (idSet.contains(i.id))
                res.push_back(i);
        return res;
    }

    void add(SSConfig config)
    {
        config.id = dao.insert(config);
        beginInsertRows(QModelIndex(), configs.size(), configs.size());
        configs.append(config);
        endInsertRows();
    }

    void edit(const SSConfig &config)
    {
        dao.update(config);
        auto p = std::find_if(configs.begin(), configs.end(), [&config](const SSConfig & c) {
            return c.id == config.id;
        });
        if (p != configs.end()) {
            *p = config;
            emit dataChanged(QModelIndex(), QModelIndex());
        }
    }

    void removeAt(const QModelIndex &i)
    {
        if (checkIndex(i)) {
            auto row = i.row();
            auto id = configs[row].id;
            dao.deleteById(id);
            beginRemoveRows(QModelIndex(), row, row);
            configs.removeAt(row);
            endRemoveRows();
        }
    }

    void importGUIConfig(const QString &guiConfigPath)
    {
        QFile f(guiConfigPath);
        f.open(QIODevice::ReadOnly);
        QJsonObject json = QJsonDocument::fromJson(f.readAll()).object();
        QJsonArray jsonConfigs = json["configs"].toArray();

        for (const auto &i : jsonConfigs) {
            add(SSConfig::fromJsonObject(i.toObject()));
        }
    }

    void exportGUIConfig(const QString &guiConfigPath) const
    {
        QJsonArray jsonConfigs;
        for (const auto &i : configs)
            jsonConfigs.push_back(i.toJsonObject());
        QJsonObject json;
        json["configs"] = jsonConfigs;

        QFile f(guiConfigPath);
        f.open(QIODevice::WriteOnly);
        f.write(QJsonDocument(json).toJson());
    }

public:
    explicit ConfigModel(QObject *parent = nullptr):
        QAbstractTableModel(parent), dao(this)
    { readAll(); }

    int rowCount(const QModelIndex &parent) const override
    { return parent.isValid() ? 0 : configs.size(); }

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
            const SSConfig &config = configs[index.row()];
            if (role == Qt::DisplayRole) {
                switch (index.column()) {
                case 0:
                    return config.getName();
                case 1:
                    return config.getLocal();
                }
            }
        }
        return QVariant();
    }
};

#endif // CONFIG_MODEL_H
