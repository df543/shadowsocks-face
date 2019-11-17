#ifndef QREASY_H
#define QREASY_H

#include "qrcode.h"
#include <algorithm>
#include <iterator>
#include <cstring>

class QrEasy {
public:
    int size() { return qr->size; }
    bool at(uint8_t x, uint8_t y) { return qrcode_getModule(qr, x, y); }
    ~QrEasy() {
        delete qr;
        delete[] qrdata;
    }
    static QrEasy encode(const char *data) {
        QrEasy ret;
        ret.qr = new QRCode();

        static int a[] = {0, 0, 0, 53, 78, 106, 134, 154, 192, 230, 271, 321, 367, 425, 458, 520, 586, 644, 718, 792, 858, 929, 1003, 1091, 1171, 1273, 1367, 1465, 1528, 1628, 1732, 1840, 1952, 2068, 2188, 2303, 2431, 2563, 2699, 2809, 2953};
        int version = std::upper_bound(std::begin(a), std::end(a), std::strlen(data)) - std::begin(a);
        if (version > 40)
            throw std::runtime_error("data too long");
        ret.qrdata = new uint8_t[qrcode_getBufferSize(version)];

        qrcode_initText(ret.qr, ret.qrdata, version, ECC_LOW, data);
        return ret;
    }

private:
    QRCode *qr = 0;
    uint8_t *qrdata = 0;
};

#endif // QREASY_H
