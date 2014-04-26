#ifndef PAILLIERPUBLICKEY_H
#define PAILLIERPUBLICKEY_H

#include <QtCrypto>

class PaillierPublicKey
{
private:
    QCA::BigInteger n;
    QCA::BigInteger g;
    QCA::BigInteger n2;
public:
    PaillierPublicKey(QCA::BigInteger num, QCA::BigInteger gen);

    QCA::BigInteger encrypt(QCA::BigInteger msg);
    QCA::BigInteger add(const QCA::BigInteger &a, const QCA::BigInteger &b);
    QCA::BigInteger increment(const QCA::BigInteger &n);
    QCA::BigInteger mutate(const QCA::BigInteger &n);
};

#endif // PAILLIERPUBLICKEY_H
