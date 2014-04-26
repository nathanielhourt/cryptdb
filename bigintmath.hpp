#ifndef BIGINTMATH_HPP
#define BIGINTMATH_HPP

#include <QtCrypto>

QCA::BigInteger operator-(const QCA::BigInteger &a, const QCA::BigInteger &b);
QCA::BigInteger operator/(const QCA::BigInteger &a, const QCA::BigInteger &b);
QCA::BigInteger operator%(const QCA::BigInteger &a, const QCA::BigInteger &b);
QCA::BigInteger operator*(const QCA::BigInteger &a, const QCA::BigInteger &b);
QCA::BigInteger operator+(const QCA::BigInteger &a, const QCA::BigInteger &b);

#endif // BIGINTMATH_HPP
