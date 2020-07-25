#include "sspm.h"

Sspm::Sspm(QObject *parent) : QObject(parent) {}

void Sspm::start(int id, const SsConfig &config)
{

}

void Sspm::terminate(int id)
{

}

bool Sspm::test()
{
    QProcess test(this);
    test.start("ss-local", QStringList());
    test.waitForFinished();
    return test.error() != QProcess::FailedToStart;
}
