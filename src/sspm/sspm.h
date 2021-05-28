#ifndef SSPM_H
#define SSPM_H


#include "model/ssconfig.h"

class Sspm : public QObject
{
    Q_OBJECT
public:
    explicit Sspm(QObject *parent = nullptr);
    void start(int id, const SsConfig &config);
    void terminate(int id);
    bool test();

signals:
    void changed();
    void out(const SsConfig &config, QByteArray data);
    void err(const SsConfig &config, QByteArray data);

private:
    struct Ss {
        SsConfig config;
        QProcess process;
        QTemporaryFile temp;

        Ss(const SsConfig &config, QObject *parent = nullptr)
            : config(config), process(parent), temp(parent)
        {
            // TODO: write json
        }
    };
    QMap<int, Ss> map;
};

#endif // SSPM_H
