#ifndef PROXY_LATENCY_TESTER_H
#define PROXY_LATENCY_TESTER_H

class ProxyLatencyTester : public QObject
{
    Q_OBJECT

public:
    explicit ProxyLatencyTester(const QNetworkProxy &proxy, QObject *parent = nullptr,
                                QUrl url = QUrl("https://google.com"), int timeout_ms = 10000, int test_round = 3):
        QObject(parent), url(std::move(url)), test_round(test_round + 1) // drop first test result
    {
        net.setProxy(proxy);
        net.setTransferTimeout(timeout_ms);
        connect(&net, &QNetworkAccessManager::finished, this, &ProxyLatencyTester::retest);
    }

public slots:
    void start_test()
    {
        test_round -= 1;
        timer.restart();
        net.get(QNetworkRequest(url));
    }

signals:
    // negative on error
    void result(int latency_ms);

private slots:
    void retest(QNetworkReply *reply)
    {
        auto time = timer.elapsed();
        auto error = reply->error();
        reply->deleteLater();

        if (error == QNetworkReply::NetworkError::NoError)
            statistics.append(time);

        if (test_round) {
            // continue
            start_test();
        } else if (statistics.size() <= 1) {
            // end, error
            emit result(-1);
        } else {
            // end, success
            statistics.pop_front(); // drop first test result
            long double avg = 0;
            for (auto i : statistics)
                avg += i;
            avg /= statistics.size();
            emit result(int(avg));
        }
    }

private:
    QUrl url;
    int test_round;

    QNetworkAccessManager net;
    QList<qint64> statistics;
    QElapsedTimer timer;
};

#endif // PROXY_LATENCY_TESTER_H
