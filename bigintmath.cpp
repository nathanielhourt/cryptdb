#include "bigintmath.hpp"

QCA::BigInteger operator-(const QCA::BigInteger &a, const QCA::BigInteger &b)
{
    QCA::BigInteger c(a);
    c -= b;
    return c;
}

QCA::BigInteger operator/(const QCA::BigInteger &a, const QCA::BigInteger &b)
{
    QCA::BigInteger c(a);
    c /= b;
    return c;
}

QCA::BigInteger operator%(const QCA::BigInteger &a, const QCA::BigInteger &b)
{
    QCA::BigInteger c(a);
    c %= b;
    return c;
}

QCA::BigInteger operator*(const QCA::BigInteger &a, const QCA::BigInteger &b)
{
    QCA::BigInteger c(a);
    c *= b;
    return c;
}

QCA::BigInteger operator+(const QCA::BigInteger &a, const QCA::BigInteger &b)
{
    QCA::BigInteger c(a);
    c += b;
    return c;
}
