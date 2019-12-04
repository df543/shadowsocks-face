#ifndef LATENCYTESTER_H
#define LATENCYTESTER_H

#include "pch.hpp"

#define NOTEST -1
#define TIMEOUT -2

class LatencyTester : public QObject
{
    Q_OBJECT

public:
    explicit LatencyTester(QNetworkProxy proxy, QUrl url, QObject *parent = nullptr);

public slots:
    void start();

signals:
    void testFinished(int latencyMs);

private:
    QUrl url;
    QNetworkProxy proxy;
    QNetworkAccessManager net;
    QElapsedTimer timer;
    int retest = 5;
    QList<int> statistics;
    int timeoutSec = 10;
    QTimer watchdog;
};

#endif // LATENCYTESTER_H
