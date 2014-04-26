#ifndef BIGINTMATH_HPP
#define BIGINTMATH_HPP

#include <QtCrypto>

QCA::BigInteger operator-(const QCA::BigInteger &a, const QCA::BigInteger &b);
QCA::BigInteger operator/(const QCA::BigInteger &a, const QCA::BigInteger &b);
QCA::BigInteger operator%(const QCA::BigInteger &a, const QCA::BigInteger &b);
QCA::BigInteger operator*(const QCA::BigInteger &a, const QCA::BigInteger &b);
QCA::BigInteger operator+(const QCA::BigInteger &a, const QCA::BigInteger &b);

/**
 * @brief Generate a random number in the range [min,max)
 */
QCA::BigInteger randomInRange(const QCA::BigInteger &min, const QCA::BigInteger &max);

#endif // BIGINTMATH_HPP
