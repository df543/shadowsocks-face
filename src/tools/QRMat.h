#ifndef QR_MAT_H
#define QR_MAT_H

#include <QrCode.hpp>

class QRMat
{
public:
    explicit QRMat(const QString &s):
        qr(qrcodegen::QrCode::encodeText(s.toUtf8().data(), qrcodegen::QrCode::Ecc::MEDIUM)) {}
    int size() const { return qr.getSize(); }
    bool at(uint8_t x, uint8_t y) const { return qr.getModule(y, x); }

private:
    qrcodegen::QrCode qr;
};

#endif // QR_MAT_H
