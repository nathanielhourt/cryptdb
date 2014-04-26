#ifndef PAILLIERPRIVATEKEY_H
#define PAILLIERPRIVATEKEY_H

#include <QtCrypto>
#include "paillierpublickey.h"

class paillierprivatekey
{
private:
    QCA::BigInteger p;
    QCA::BigInteger q;
    QCA::BigInteger n;
    QCA::BigInteger l;
    QCA::BigInteger mu;
    QCA::BigInteger g;
    paillierpublickey* pub;
public:
    paillierprivatekey();
    QCA::BigInteger gcd(QCA::BigInteger oper1,QCA::BigInteger oper2);
    QCA::BigInteger invmod(QCA::BigInteger oper1, QCA::BigInteger oper2);
    QCA::BigInteger modexp(QCA::BigInteger num, QCA::BigInteger exponent, QCA::BigInteger prime);

};

#endif // PAILLIERPRIVATEKEY_H
