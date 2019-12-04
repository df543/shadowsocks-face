#include "latencytester.h"

LatencyTester::LatencyTester(QNetworkProxy proxy, QUrl url, QObject *parent)
    : QObject(parent), url(url), proxy(proxy), net(this), watchdog(this)
{
    watchdog.setInterval(timeoutSec * 1000);
    watchdog.setSingleShot(true);
    connect(&watchdog, &QTimer::timeout, [this] {
        emit testFinished(TIMEOUT);
        deleteLater();
    });

    net.setProxy(proxy);
    connect(&net, &QNetworkAccessManager::finished, [this] {
        auto n = timer.elapsed();
        statistics.append(n);
        if (retest)
            QTimer::singleShot(300, this, &LatencyTester::start);
        else
        {
            statistics.pop_front();
            double avg = 0;
            for (auto i : statistics)
                avg += i;
            avg /= statistics.size();
            emit testFinished(avg);
            deleteLater();
        }
    });
}

void LatencyTester::start()
{
    retest--;
    watchdog.start();
    timer.restart();
    auto reply = net.get(QNetworkRequest(url));
    connect(&watchdog, &QTimer::timeout, reply, &QNetworkReply::abort);
}
