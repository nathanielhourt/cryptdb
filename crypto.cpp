#include "crypto.hpp"

QCA::SecureArray Crypto::generateS(QCA::SecureArray ks, DB::Index i) {
    QCA::Hash hasher("sha1");
    hasher.update(ks);
    hasher.update((char*)&i, sizeof(i));
    QCA::SecureArray result = hasher.final();

    return result.toByteArray().right(N_BYTES - M_BYTES);
}

QCA::SecureArray Crypto::generateKi(QCA::SecureArray kk, QCA::SecureArray Li)
{
    QCA::Hash hasher("sha1");
    hasher.update(kk);
    hasher.update(Li);
    QCA::SecureArray result = hasher.final();

    return result;
}

QCA::SecureArray Crypto::arrayXor(QCA::SecureArray a, QCA::SecureArray b)
{
    if (a.size() != b.size())
        return QCA::SecureArray();

    QCA::SecureArray c(a.size());
    for (int i = 0; i < a.size(); ++i)
        c[i] = a[i] ^ b[i];

    return c;
}

const quint32 Crypto::N_BYTES = 4;
const quint32 Crypto::M_BYTES = 1;
