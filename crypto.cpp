#include "crypto.hpp"
#include "paillierprivatekey.hpp"
#include "bigintmath.hpp"

#include <gmpxx.h>
#include <QDebug>

QCA::SecureArray Crypto::generateS(QCA::SecureArray ks, DB::Index i) {
    QCA::Hash hasher("sha1");
    hasher.update(ks);
    hasher.update((char*)&i, sizeof(i));
    QCA::SecureArray result = hasher.final();

    //We need a lot more bits than any hashing algorithm gives. Use the hash to seed an RNG
    return randomBytes(N_BYTES-M_BYTES, result);
}

QCA::SecureArray Crypto::generateKi(QCA::SecureArray kk, QCA::SecureArray Li)
{
    QCA::Hash hasher("sha1");
    hasher.update(kk);
    hasher.update(Li);
    QCA::SecureArray result = hasher.final();

    return result;
}

DB::Word Crypto::preEncrypt(DB::Word wi, PaillierPublicKey key) {
    DB::Word ctxt = key.encrypt(QCA::SecureArray(wi)).toArray().toByteArray();
    if (unsigned(ctxt.size()) < N_BYTES)
        ctxt.prepend(DB::Word(N_BYTES - ctxt.size(), '\0'));
    return ctxt;
}

DB::Word Crypto::postDecrypt(DB::Word ctxt, const PaillierPrivateKey &key)
{
    return key.decrypt(QCA::SecureArray(ctxt)).toArray().toByteArray();
}

/*
 * Function returns the result of SHA1(k_i + Si)
 */
QCA::SecureArray Crypto::generateFki(QCA::SecureArray k_i, QCA::SecureArray Si)
{
    QCA::Hash hasher("sha1");
    hasher.update(k_i);
    hasher.update(Si);
    QCA::SecureArray result = hasher.final();

    return randomBytes(M_BYTES, result);
}

bool Crypto::clientWordMatchesDatabaseWord(DB::Word clientWord, DB::Word databaseWord, QCA::SecureArray k_i)
{
    QByteArray ti = arrayXor(clientWord, databaseWord).toByteArray();
    return generateFki(k_i, ti.left(N_BYTES - M_BYTES)) == ti.right(M_BYTES);
}

QCA::SecureArray Crypto::arrayXor(QCA::SecureArray a, QCA::SecureArray b)
{
    if (a.size() != b.size()) {
        qDebug() << "Cannot XOR unequally sized arrays! Sizes:" << a.size() << b.size();
        return QCA::SecureArray();
    }

    QCA::SecureArray c(a.size());
    for (int i = 0; i < a.size(); ++i)
        c[i] = a[i] ^ b[i];

    return c;
}


const quint32 Crypto::N_BYTES = 257;
const quint32 Crypto::M_BYTES = 51;
