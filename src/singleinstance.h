#ifndef SINGLEINSTANCE_H
#define SINGLEINSTANCE_H

#include <QObject>

class SingleInstance: public QObject {
    Q_OBJECT
public:
    explicit SingleInstance(QString key, QObject *parent = nullptr): QObject(parent), _key(key) {}
    bool runGuard();

signals:
    void newInstance();

private:
    QString _key;
};

#endif // SINGLEINSTANCE_H
