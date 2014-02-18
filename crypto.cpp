#include "crypto.hpp"
#include <QDebug>

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

DB::Word Crypto::preEncrypt(DB::Word wi) {
    //Setup AES Key and IV. Need to be deterministic.
    QCA::SymmetricKey akey(QCA::hexToArray("00000000000000000000000000000005"));
    QCA::InitializationVector iv(QCA::hexToArray("00000000000000000000000000000006"));

    QCA::Cipher cipher(QString("aes128"),QCA::Cipher::CBC,
                              // use Default padding, which is equivalent to PKCS7 for CBC
                              QCA::Cipher::DefaultPadding,
                              // this object will encrypt
                              QCA::Encode,
                              akey, iv);
    QCA::SecureArray data(wi);
    qWarning() << "Clear: " << data.toByteArray().toHex();
    QCA::SecureArray encdata = cipher.process(data);


    if(!cipher.ok()) {
        printf("Error\n");
    }
    else {
        printf("Encryption Ok\n");
    }


    wi = encdata.toByteArray();
/*
    qWarning() << "Wi = " << wi.toHex();
    cipher.setup( QCA::Decode, akey, iv );
    QCA::SecureArray ctxt(wi);

    qWarning() << "Ctxt: " << ctxt.toByteArray().toHex();
    qWarning() << "EncData " << encdata.toByteArray().toHex();
    QCA::SecureArray ptxt = cipher.process(encdata);

    if(!cipher.ok()) {
        qWarning() << "Error Decrypting";
    }
    qWarning() << "ptxt = " << ptxt.toByteArray().toHex();
    printf("ptxt = %u\n",ptxt.toByteArray().toUInt());
*/
    return wi;
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
/*
 * Function returns the result of SHA1(k_i + Si)
 */
QCA::SecureArray Crypto::generateFki(QCA::SecureArray k_i, QCA::SecureArray Si)
{
    QCA::Hash hasher("sha1");
    hasher.update(k_i);
    hasher.update(Si);
    QCA::SecureArray result = hasher.final();

    return result.toByteArray().right(M_BYTES);
} //end generateFki function

const quint32 Crypto::N_BYTES = 4;
const quint32 Crypto::M_BYTES = 1;
