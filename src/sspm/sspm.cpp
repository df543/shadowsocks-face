#include "sspm.h"

Sspm::Sspm(QObject *parent) : QObject(parent) {}

void Sspm::start(qint64 id, const SSConfig &config)
{

}

void Sspm::terminate(qint64 id)
{

}

bool Sspm::test()
{
    QProcess test(this);
    test.start("ss-local", QStringList());
    test.waitForFinished();
    return test.error() != QProcess::FailedToStart;
}
