#include <QDebug>
#include <gmp.h>
#include <QtCrypto>

#include "paillierprivatekey.hpp"
#include "modularmath.hpp"
#include "bigintmath.hpp"

PaillierPrivateKey::PaillierPrivateKey():
    pub(nullptr)
{
    p = QCA::KeyGenerator().createRSA(1024).toRSA().p();
    q = QCA::KeyGenerator().createRSA(1024).toRSA().q();
    n = QCA::KeyGenerator().createRSA(1024).toRSA().n();
    l = (p-1) * (q-1);
    g = n += 1;
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
