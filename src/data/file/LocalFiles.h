#ifndef LOCAL_FILES_H
#define LOCAL_FILES_H

class LocalFiles: public QObject
{
    Q_OBJECT

public:
    explicit LocalFiles(QObject *parent = nullptr):
        QObject(parent),
        save(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)),
        running(save.absoluteFilePath("running"))
    {
        if (!save.mkpath("."))
            throw std::runtime_error("cannot create save path");
        if (!running.mkpath("."))
            throw std::runtime_error("cannot create temp config path");
    }

    void clearTempConfig()
    {
        for (const auto &f : running.entryList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
            running.remove(f);
        }
    }

    QDir saveDir()
    { return save; }

    QDir tempConfigDir()
    { return running; }

private:
    QDir save;
    QDir running;
};

#endif // LOCAL_FILES_H
