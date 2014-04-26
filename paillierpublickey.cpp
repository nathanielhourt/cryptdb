#include <gmpxx.h>

#include "modularmath.hpp"
#include "paillierpublickey.hpp"
#include "bigintmath.hpp"

PaillierPublicKey::PaillierPublicKey(QCA::BigInteger num, QCA::BigInteger gen)
{
    n = num;
    g = gen;
    n_sq = n*n;
}

QCA::BigInteger PaillierPublicKey::encrypt(QCA::BigInteger msg)
{
    gmp_randclass rng(gmp_randinit_default);
    rng.seed(mpz_class(QCA::BigInteger(QCA::Random::randomArray(32)).toString().toLocal8Bit().data()));
    QCA::BigInteger rand = mpz_class(rng.get_z_range(mpz_class((n-2).toString().toLocal8Bit().data()))).get_str().c_str();
    //rand is now in [0,n-1); shift it up to [1, n)
    rand += 1;

    QCA::BigInteger step1 = ModularMath::modexp(g,msg,n_sq);
    QCA::BigInteger step2 = ModularMath::modexp(rand,n,n_sq);

    return  (step1 * step2) % n;
}
