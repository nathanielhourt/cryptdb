#include <QDebug>

#include "modularmath.hpp"
#include "paillierpublickey.hpp"
#include "bigintmath.hpp"

PaillierPublicKey::PaillierPublicKey(QCA::BigInteger num, QCA::BigInteger gen)
{
    n = num;
    g = gen;
    n_sq = n*n;

    qDebug() << "Created public key; n:" << n.toString() << "and g:" << g.toString();
}

QCA::BigInteger PaillierPublicKey::encrypt(QCA::BigInteger msg)
{
    QCA::BigInteger rand = randomInRange(1, n);
    qDebug() << "Encrypting with random number" << rand.toString();

    QCA::BigInteger step1 = ModularMath::modexp(g,msg,n_sq);
    QCA::BigInteger step2 = ModularMath::modexp(rand,n,n_sq);

    return  (step1 * step2) % n;
}
