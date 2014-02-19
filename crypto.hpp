#ifndef CRYPTO_H
#define CRYPTO_H

#include <QtCrypto/qca.h>

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

    /**
     * @brief Generate a k_i value for the given L_i
     * @param kk The client's secret key for k_i values
     * @param Li The L_i value to create a k_i for
     *
     * This function generates a k_i value using the secret key ks and the left section of the pre-encrypted word.
     *
     * @return k_i for L_i using kk
     */
    static QCA::SecureArray generateKi(QCA::SecureArray kk, QCA::SecureArray Li);

    /**
     * @brief Pre-Encrypt a word before it is sent to the server.
     * @param wi the word to be encrypted
     * @param akey The key to pre-encrypt with
     * @param iv The IV to pre-encrypt with
     *
     * This function pre-encrypts a database word so that we can search the database without revealing to the server
     * what word we are searching for. The word is encrypted using the provided key and IV.
     *
     * @return X, the encrypted word.
     */
    static DB::Word preEncrypt(DB::Word wi, QCA::SymmetricKey akey, QCA::InitializationVector iv);

    /**
     * @brief Post-Decrypt a word after it is returned from the server.
     * @param ctxt The word to be decrypted
     * @param akey The key to decrypt with
     * @param iv The iv used for decryption
     *
     * This function post-decrypts a database word after it is returned from the server.
     *
     * @return W, the decrypted.
     */
    static DB::Word postDecrypt(DB::Word ctxt, QCA::SecureArray akey, QCA::InitializationVector iv);
    /**
     * @brief Generate a random string of bits given input and key k_i
     * @param k_i The key for the input function
     * @param Si The value to be run through the function
     *
     * @return FKi F_k_i(S_i) given Si and k_i
     */
    static QCA::SecureArray generateFki(QCA::SecureArray k_i, QCA::SecureArray Si);

    /**
     * @brief Determine if a word from the client matches a word in the database
     * @param clientWord A word from the client to search in the database
     * @param databaseWord A word in the database to compare with clientWord
     * @param k_i The key for clientWord, provided by the client
     *
     * When the server receives a word to search the database for, it will call this function for each database
     * word it wishes to compare against. This function performs a probabilistic comparison of the words, and
     * returns true if they appear to match.
     *
     * Note that while there is a probability of false-positive, this function is deterministic, so multiple
     * calls with the same arguments will yield the same result.
     *
     * @return True if the words appear to match, false otherwise
     */
    static bool clientWordMatchesDatabaseWord(DB::Word clientWord, DB::Word databaseWord, QCA::SecureArray k_i);

    /**
     * @brief Xors the two arrays
     * @param a First array to xor
     * @param b Second array to xor
     *
     * Xors a and b together and returns the result. a and b must be the same length; if they are not, an empty
     * array is returned.
     *
     * @return a ^ b
     */
    static QCA::SecureArray arrayXor(QCA::SecureArray a, QCA::SecureArray b);

    const static quint32 N_BYTES;
    const static quint32 M_BYTES;
};

#endif // CRYPTO_H
