#ifndef QREASY_H
#define QREASY_H

#include "common.hpp"
#include "third_party/qrcode/src/qrcode.h"

class QrEasy {
public:
    QrEasy(const char *data, int len);
    QrEasy(const std::string &s);
    virtual ~QrEasy();
    int size();
    bool at(uint8_t x, uint8_t y);

private:
    QRCode *qr;
    uint8_t *qrdata;
};

#endif // QREASY_H
