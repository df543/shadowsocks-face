#ifndef ONCEGUARD_H
#define ONCEGUARD_H



/* Single instance doorbell
 *
 * lock(): connect to `$name` pipe, and occupy it if not in use
 * signal knocked: emitted when a new instance come.
 */

class OnceGuard: public QObject
{
    Q_OBJECT
public:
    explicit OnceGuard(QString name, QObject *parent = nullptr): QObject(parent), name(name) {}
    bool lock();

signals:
    void knocked();

private:
    QString name;
};

#endif // ONCEGUARD_H
