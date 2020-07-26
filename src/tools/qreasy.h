#ifndef QREASY_H
#define QREASY_H

#include "common.hpp"
#include "third_party/QR-Code-generator/cpp/QrCode.hpp"

class QrEasy
{
public:
    QrEasy(const std::string &s)
        : qr(qrcodegen::QrCode::encodeText(s.data(), qrcodegen::QrCode::Ecc::LOW)) {}
    virtual ~QrEasy() {}
    int size() { return qr.getSize(); }
    bool at(uint8_t x, uint8_t y) { return qr.getModule(y, x); }

private:
    qrcodegen::QrCode qr;
};

#endif // QREASY_H
