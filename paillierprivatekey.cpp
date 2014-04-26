#include <QDebug>
#include <gmp.h>
#include <QtCrypto>

#include "paillierprivatekey.hpp"
#include "modularmath.hpp"
#include "bigintmath.hpp"

PaillierPrivateKey::PaillierPrivateKey(unsigned int bits):
    pub(nullptr)
{
    QCA::RSAPrivateKey delegate = QCA::KeyGenerator().createRSA(bits).toRSA();
    p = delegate.p();
    q = delegate.q();
    n = delegate.n();
    n2 = n*n;
    l = (p-1) * (q-1);
    g = randomInRange(1, n2);
    mu = ModularMath::invmod((ModularMath::modexp(g, l, n2)-1)/n, n);
}

PaillierPrivateKey::~PaillierPrivateKey()
{
    delete pub;
}

PaillierPublicKey PaillierPrivateKey::derivePublicKey()
{
    if (pub == nullptr)
        pub = new PaillierPublicKey(n,g);
    return *pub;
}

QCA::BigInteger PaillierPrivateKey::decrypt(QCA::BigInteger cipher)
{
    return ((ModularMath::modexp(cipher, l, n2)-1)/n)*mu % n;
}
