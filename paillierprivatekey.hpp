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
    QCA::BigInteger n2;
    QCA::BigInteger l;
    QCA::BigInteger mu;
    QCA::BigInteger g;
    mutable PaillierPublicKey *pub;

public:
    PaillierPrivateKey(unsigned int bits = 1024);
    ~PaillierPrivateKey();

    PaillierPublicKey derivePublicKey() const;

    QCA::BigInteger decrypt(QCA::BigInteger cipher) const;
};

#endif // PAILLIERPRIVATEKEY_H
