#ifndef CRYPTO_H
#define CRYPTO_H

#include <QtCrypto>

#include "database.hpp"

class Crypto
{
    explicit Crypto(){}
public:
    /**
     * @brief Generate an S_i value for the given index
     * @param ks The client's secret key for S values
     * @param i The index of the S to generate
     *
     * This function generates an S_i value using the secret key ks, and returns it.
     *
     * @return The S value at the given index
     */
    static QCA::SecureArray generateS(QCA::SecureArray ks, DB::Index i);

    const static quint32 N_BYTES;
    const static quint32 M_BYTES;
};

#endif // CRYPTO_H
