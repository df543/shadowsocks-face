#include "qreasy.h"

using namespace std;

static int checkQrVersion(int len)
{
    static vector<int> a = {0, 0, 0, 53, 78, 106, 134, 154, 192, 230, 271, 321, 367, 425, 458, 520, 586, 644, 718, 792, 858, 929, 1003, 1091, 1171, 1273, 1367, 1465, 1528, 1628, 1732, 1840, 1952, 2068, 2188, 2303, 2431, 2563, 2699, 2809, 2953};
    int version = upper_bound(a.begin(), a.end(), len) - a.begin();
    if (version > 40)
        throw runtime_error("data too long");
    return version;
}

QrEasy::QrEasy(const char *data, int len)
{
    qr = new QRCode();
    int version = checkQrVersion(len);
    qrdata = new uint8_t[qrcode_getBufferSize(version)];
    qrcode_initText(qr, qrdata, version, ECC_LOW, data);
}

QrEasy::QrEasy(const string &s): QrEasy(s.data(), s.size()) {}

QrEasy::~QrEasy()
{
    delete qr;
    delete[] qrdata;
}

int QrEasy::size()
{
    return qr->size;
}

bool QrEasy::at(uint8_t x, uint8_t y)
{
    return qrcode_getModule(qr, x, y);
}
