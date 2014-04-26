#ifndef PAILLIERPRIVATEKEY_H
#define PAILLIERPRIVATEKEY_H

#include <QtCrypto>
#include "paillierpublickey.hpp"

class PaillierPrivateKey
{
private:
    QCA::BigInteger p;
    QCA::BigInteger q;
    QCA::BigInteger n;
    QCA::BigInteger l;
    QCA::BigInteger mu;
    QCA::BigInteger g;
    PaillierPublicKey *pub;

public:
    PaillierPrivateKey();
    ~PaillierPrivateKey();

    PaillierPublicKey derivePublicKey();
};

#endif // PAILLIERPRIVATEKEY_H
