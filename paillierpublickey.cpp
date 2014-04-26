#include <QDebug>

#include "modularmath.hpp"
#include "paillierpublickey.hpp"
#include "bigintmath.hpp"

PaillierPublicKey::PaillierPublicKey(QCA::BigInteger num, QCA::BigInteger gen)
{
    n = num;
    g = gen;
    n2 = n*n;

    qDebug() << "Created public key; n:" << n.toString() << "and g:" << g.toString();
}

QCA::BigInteger PaillierPublicKey::encrypt(QCA::BigInteger msg)
{
    QCA::BigInteger rand = randomInRange(1, n);

    QCA::BigInteger step1 = ModularMath::modexp(g,msg,n2);
    QCA::BigInteger step2 = ModularMath::modexp(rand,n,n2);

    return  (step1 * step2) % n2;
}

QCA::BigInteger PaillierPublicKey::add(const QCA::BigInteger &a, const QCA::BigInteger &b)
{
    return a * b % n2;
}

QCA::BigInteger PaillierPublicKey::increment(const QCA::BigInteger &n)
{
    return add(n, encrypt(1));
}

QCA::BigInteger PaillierPublicKey::mutate(const QCA::BigInteger &n)
{
    return add(n, encrypt(0));
}
