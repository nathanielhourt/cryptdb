#ifndef PAILLIERPUBLICKEY_H
#define PAILLIERPUBLICKEY_H

#include <QtCrypto>

class PaillierPublicKey
{
private:
    QCA::BigInteger n;
    QCA::BigInteger g;
    QCA::BigInteger n_sq;
public:
    PaillierPublicKey(QCA::BigInteger num, QCA::BigInteger gen);
    QCA::BigInteger encrypt(QCA::BigInteger msg);
};

#endif // PAILLIERPUBLICKEY_H
