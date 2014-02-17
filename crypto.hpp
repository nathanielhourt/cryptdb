#ifndef CRYPTO_H
#define CRYPTO_H

#include <QtCrypto/qca.h>

#include "database.hpp"

class Crypto
{

public:
    explicit Crypto(){}
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

    /**
     * @brief Pre-Encrypt a word before it is sent to the server.
     * @param Wd the word to be encrypted
     *
     * @return X, the encrypted word.
     */
    DB::Word PreEncrypt(DB::Word wi);
    const static quint32 N_BYTES;
    const static quint32 M_BYTES;
};

#endif // CRYPTO_H
