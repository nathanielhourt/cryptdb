#include <QDebug>

#include "databaseclient.hpp"

#include "crypto.hpp"

QCA::SecureArray DatabaseClient::generateTi(DB::Word word, DB::Index index)
{
    QCA::SecureArray ti = Crypto::generateS(ks, index);
    ti.append(Crypto::generateFki(Crypto::generateKi(kk, word.left(Crypto::N_BYTES - Crypto::M_BYTES)), ti));
    return ti;
}

DatabaseClient::DatabaseClient(QObject *parent) :
    QObject(parent),
    kPrime(QCA::Random::randomArray(16)),
    kPrimePrime(QCA::Random::randomArray(16)),
    preEncryptIV(QCA::Random::randomArray(16)),
    ks(QCA::Random::randomArray(16)),
    kk(QCA::Random::randomArray(16))
{
}

DB::RowList DatabaseClient::encryptNewRows(DB::RowList newRows, DB::Index nextAvailableIndex)
{
    DB::RowList crypticRows;

    foreach (DB::Row row, newRows) {
        crypticRows.append(DB::Row());
        foreach (DB::Word word, row) {
            word = Crypto::preEncrypt(word, kPrimePrime, preEncryptIV);
            crypticRows.last().append(Crypto::arrayXor(word, generateTi(word, nextAvailableIndex++)).toByteArray());
        }
    }

    return crypticRows;
}

DB::RowList DatabaseClient::decryptRows(DB::IndexedRowList crypticRows)
{
    DB::RowList plaintextRows;

    foreach(DB::IndexedRow row, crypticRows) {
        quint32 index = row.first;
        plaintextRows.append(DB::Row());
        foreach (DB::Word crypticWord, row.second) {
            QCA::SecureArray word = Crypto::arrayXor(crypticWord.left(Crypto::N_BYTES - Crypto::M_BYTES), Crypto::generateS(ks, index));
            word.append(Crypto::generateFki(Crypto::generateKi(kk, word), Crypto::generateS(ks, index++)));
            plaintextRows.last().append(Crypto::postDecrypt(word.toByteArray(), kPrime, preEncryptIV));
        }
    }

    return plaintextRows;
}

QPair<DB::Word, QCA::SecureArray> DatabaseClient::encryptWordForSearch(DB::Word plainText)
{
    Crypto crypt;
    //Split word into X_i

    DB::Word ctxt = crypt.preEncrypt(plainText, kPrimePrime, preEncryptIV);
    QCA::SecureArray cipherText(ctxt);
    QCA::SecureArray k = crypt.generateKi(kPrime, cipherText);

    return QPair<DB::Word, QCA::SecureArray>(ctxt, k);
} //end encryptWordForSearch function
