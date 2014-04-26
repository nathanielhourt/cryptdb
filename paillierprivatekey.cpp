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
    l = (p-1) * (q-1);
    g = randomInRange(1, n*n);
    mu = ModularMath::invmod(l,g);
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
