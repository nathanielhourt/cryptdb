#include "crypto.hpp"

QCA::SecureArray Crypto::generateS(QCA::SecureArray ks, DB::Index i) {
    QCA::Hash hasher("sha1");
    hasher.update(ks);
    hasher.update((char*)&i, sizeof(i));
    QCA::SecureArray result = hasher.final();

    return result.toByteArray().right(N_BYTES - M_BYTES);
}

const quint32 Crypto::N_BYTES = 4;
const quint32 Crypto::M_BYTES = 1;
