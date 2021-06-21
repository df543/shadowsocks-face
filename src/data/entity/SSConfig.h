#ifndef SS_CONFIG_H
#define SS_CONFIG_H

class SSConfig
{
public:
    qint64 id = 0;

    QString method;
    QString password;
    QString server_address;
    int server_port = 0;
    QString name;

    QString local_address = "127.0.0.1";
    int local_port = 1080;
    QString mode = "tcp_only";
    int timeout = 5;
    bool fast_open = false;

    QString getName() const
    {
        if (name.isEmpty())
            return QString("%1:%2").arg(server_address).arg(server_port);
        else return name;
    }

    QString getLocal() const
    {
        if (local_address == "127.0.0.1")
            return QString::number(local_port);
        else
            return QString("%1:%2").arg(local_address).arg(local_port);
    }

    enum URIType {
        ORIGINAL_BASE64,
        SIP002
    };

    QString toURI(URIType uriType) const
    {
        QString tag_part = name.isEmpty() ? "" : "#" + QString(name.toUtf8().toPercentEncoding());

        switch (uriType) {
        case ORIGINAL_BASE64: {
            QString content = QString("%1:%2@%3:%4").arg(method, password, server_address).arg(server_port);
            QString content_base64 = content.toUtf8().toBase64(QByteArray::OmitTrailingEquals);
            return QString("ss://%1%2").arg(content_base64, tag_part);
        }
        case SIP002: {
            QString userInfo = QString("%1:%2").arg(method, password);
            QString userInfo_base64 = userInfo.toUtf8().toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);
            return QString("ss://%1@%2:%3%4").arg(userInfo_base64, server_address).arg(server_port).arg(tag_part);
        }
        default:
            throw std::runtime_error("internal error: unknown URI type");
        }
    }

    static SSConfig fromURI(const QString &s)
    {
        QRegularExpression original_base64_pattern(QRegularExpression::anchoredPattern("ss://([A-Za-z0-9+/]*=*)(#.*)?"));
        QRegularExpression sip002_pattern(QRegularExpression::anchoredPattern("ss://([A-Za-z0-9-_]*=*)@(.*?):(\\d*)/?.*?(#.*)?"));

        if (auto match = original_base64_pattern.match(s); match.hasMatch()) {
            SSConfig res;

            QString content_base64 = match.captured(1);
            QString content = QByteArray::fromBase64(content_base64.toUtf8());
            QRegularExpression content_pattern(QRegularExpression::anchoredPattern("(.*?):(.*)@(.*):(\\d*)"));
            auto content_match = content_pattern.match(content);
            if (content_match.lastCapturedIndex() != 4)
                goto ORIGINAL_BASE64_FAILED;
            res.method = content_match.captured(1);
            res.password = content_match.captured(2);
            res.server_address = content_match.captured(3);
            res.server_port = content_match.captured(4).toInt();

            QString tag_part = match.captured(2);
            res.name = QByteArray::fromPercentEncoding(tag_part.right(tag_part.size() - 1).toUtf8());

            return res;
        }
ORIGINAL_BASE64_FAILED:

        if (auto match = sip002_pattern.match(s); match.hasMatch()) {
            SSConfig res;

            QString userInfo_base64 = match.captured(1);
            QString userInfo = QByteArray::fromBase64(userInfo_base64.toUtf8(), QByteArray::Base64UrlEncoding);
            QRegularExpression userInfo_pattern(QRegularExpression::anchoredPattern("(.*?):(.*)"));
            auto userInfo_match = userInfo_pattern.match(userInfo);
            if (userInfo_match.lastCapturedIndex() != 2)
                goto SIP002_FAILED;
            res.method = userInfo_match.captured(1);
            res.password = userInfo_match.captured(2);

            res.server_address = match.captured(2);
            res.server_port = match.captured(3).toInt();

            QString tag_part = match.captured(4);
            res.name = QByteArray::fromPercentEncoding(tag_part.right(tag_part.size() - 1).toUtf8());

            return res;
        }
SIP002_FAILED:

        throw std::invalid_argument("incorrect uri");
    }

    QJsonObject toJsonObject() const
    {
        QJsonObject res;
        res["server"] = server_address;
        res["server_port"] = server_port;
        res["local_address"] = local_address;
        res["local_port"] = local_port;
        res["password"] = password;
        res["method"] = method;
        res["mode"] = mode;
        res["timeout"] = timeout;
        res["fast_open"] = fast_open;
        res["remarks"] = name;
        return res;
    }

    static SSConfig fromJsonObject(const QJsonObject &json)
    {
        SSConfig res;
        res.name            = json.contains("remarks")          ? json["remarks"].toString()        : res.name;
        res.server_address  = json.contains("server")           ? json["server"].toString()         : res.server_address;
        res.server_port     = json.contains("server_port")      ? json["server_port"].toInt()       : res.server_port;
        res.local_address   = json.contains("local_address")    ? json["local_address"].toString()  : res.local_address;
        res.local_port      = json.contains("local_port")       ? json["local_port"].toInt()        : res.local_port;
        res.password        = json.contains("password")         ? json["password"].toString()       : res.password;
        res.method          = json.contains("method")           ? json["method"].toString()         : res.method;
        res.mode            = json.contains("mode")             ? json["mode"].toString()           : res.mode;
        res.timeout         = json.contains("timeout")          ? json["timeout"].toInt()           : res.timeout;
        res.fast_open       = json.contains("fast_open")        ? json["fast_open"].toBool()        : res.fast_open;
        return res;
    }
};

#endif // SS_CONFIG_H
