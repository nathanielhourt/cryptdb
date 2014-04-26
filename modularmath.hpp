#ifndef MODULARMATH_HPP
#define MODULARMATH_HPP

#include <QtCrypto>

class ModularMath
{
    ModularMath();
public:
    static QCA::BigInteger gcd(QCA::BigInteger oper1,QCA::BigInteger oper2);
    static QCA::BigInteger invmod(QCA::BigInteger oper1, QCA::BigInteger oper2);
    static QCA::BigInteger modexp(QCA::BigInteger num, QCA::BigInteger exponent, QCA::BigInteger prime);
};

#endif // MODULARMATH_HPP
