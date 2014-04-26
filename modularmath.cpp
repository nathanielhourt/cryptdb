#include <gmp.h>

#include "modularmath.hpp"

ModularMath::ModularMath()
{
}

QCA::BigInteger ModularMath::gcd(QCA::BigInteger oper1,QCA::BigInteger oper2)
{
    /*
     *
     *
        void mpz_gcd (mpz_t rop, const mpz_t op1, const mpz_t op2)
        Set rop to the greatest common divisor of op1 and op2. The result is always
        positive even if one or both input operands are negative.
        Except if both inputs are zero; then this function defines gcd(0,0) = 0.
     */
    mpz_t r, op1, op2;

    mpz_init_set_str(op1, oper1.toString().toLocal8Bit().data(), 0);
    mpz_init_set_str(op2, oper2.toString().toLocal8Bit().data(), 0);
    mpz_init(r);

    mpz_gcd(r,op1,op2);

    char* result = mpz_get_str(nullptr, 10, r);

    QCA::BigInteger qresult(result);

    mpz_clear(r);
    mpz_clear(op1);
    mpz_clear(op2);
    delete [] result;

    return qresult;
}

QCA::BigInteger ModularMath::invmod(QCA::BigInteger oper1, QCA::BigInteger oper2)
{
    /*
        int mpz_invert (mpz_t rop, const mpz_t op1, const mpz_t op2)
        Compute the inverse of op1 modulo op2 and put the result in rop.
        If the inverse exists, the return value is non-zero and rop will
        satisfy 0 < rop < abs(op2). If an inverse doesn't exist the return
        value is zero and rop is undefined. The behaviour of this function
        is undefined when op2 is zero.
     */
    mpz_t r, op1, op2;

    mpz_init_set_str(op1, oper1.toString().toLocal8Bit().data(), 0);
    mpz_init_set_str(op2, oper2.toString().toLocal8Bit().data(), 0);
    mpz_init(r);

    mpz_invert(r,op1,op2);

    char* result = mpz_get_str(nullptr, 10, r);

    QCA::BigInteger qresult(result);

    mpz_clear(r);
    mpz_clear(op1);
    mpz_clear(op2);
    delete [] result;

    return qresult;
}

QCA::BigInteger ModularMath::modexp(QCA::BigInteger num, QCA::BigInteger exponent, QCA::BigInteger prime)
{
    mpz_t r, a, b, m;

    mpz_init_set_str(a, num.toString().toLocal8Bit().data(), 0);
    mpz_init_set_str(b, exponent.toString().toLocal8Bit().data(), 0);
    mpz_init_set_str(m, prime.toString().toLocal8Bit().data(), 0);
    mpz_init(r);

    mpz_powm(r, a, b, m);

    char* result = mpz_get_str(nullptr, 10, r);

    QCA::BigInteger qresult(result);

    mpz_clear(r);
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(m);
    delete [] result;

    return qresult;
}
