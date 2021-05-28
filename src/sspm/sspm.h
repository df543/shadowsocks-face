#ifndef SSPM_H
#define SSPM_H


#include "entity/SSConfig.h"

class Sspm : public QObject
{
    Q_OBJECT
public:
    explicit Sspm(QObject *parent = nullptr);
    void start(qint64 id, const SSConfig &config);
    void terminate(qint64 id);
    bool test();

signals:
    void changed();
    void out(const SSConfig &config, QByteArray data);
    void err(const SSConfig &config, QByteArray data);

private:
    struct Ss {
        SSConfig config;
        QProcess process;
        QTemporaryFile temp;

        Ss(const SSConfig &config, QObject *parent = nullptr)
            : config(config), process(parent), temp(parent)
        {
            // TODO: write json
        }
    };
    QMap<int, Ss> map;
};

#endif // SSPM_H
