#include "modularmath.hpp"
#include "paillierpublickey.hpp"

PaillierPublicKey::PaillierPublicKey(QCA::BigInteger num, QCA::BigInteger gen)
{
    n = num;
    g = gen;
    n_sq = n*=n;

}

QCA::BigInteger PaillierPublicKey::encrypt(QCA::BigInteger msg){

    /* Not sure about the best way to generate
     * a random number between (1, n) using
     * QCA. Would this way work?
     */
    QCA::BigInteger rand;
    bool randflag = 0;
    while (randflag == 0){
        rand = QCA::Random::randomInt();
        if(rand>1 && rand<n){
            randflag = 1;
        }
    };

    QCA::BigInteger step1 = ModularMath::modexp(g,msg,n_sq);
    QCA::BigInteger step2 = ModularMath::modexp(rand,n,n_sq);

    QCA::BigInteger result(step1);
    result *= step2;
    result %= n;

    return  result;

}
