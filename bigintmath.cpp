#include <gmpxx.h>

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


QCA::BigInteger randomInRange(const QCA::BigInteger &min, const QCA::BigInteger &max)
{
    gmp_randclass rng(gmp_randinit_default);
    rng.seed(mpz_class(QCA::BigInteger(QCA::Random::randomArray(32)).toString().toLocal8Bit().data()));
    QCA::BigInteger rand = mpz_class(rng.get_z_range(mpz_class((max - min).toString().toLocal8Bit().data()))).get_str().c_str();
    //rand is now in [0,max-min); shift it up to [min, max)
    return rand + min;
}


QCA::SecureArray randomBytes(unsigned int numBytes, QCA::SecureArray seed)
{
    gmp_randclass rng(gmp_randinit_default);
    rng.seed(mpz_class(QCA::BigInteger(seed).toString().toLocal8Bit().data()));
    QCA::SecureArray result = QCA::BigInteger(mpz_class(rng.get_z_bits(numBytes*8 + 1)).get_str().c_str()).toArray();
    result.resize(numBytes);
    return result;
}
