#include <QDebug>

#include "databaseclient.hpp"

#include "crypto.hpp"

QCA::SecureArray DatabaseClient::generateTi(DB::Word word, DB::Index index)
{
    qDebug() << "kk" << kk.toByteArray().toHex();
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
    qDebug() << "Encrypting new rows";

    foreach (DB::Row row, newRows) {
        crypticRows.append(DB::Row());
        foreach (DB::Word word, row) {
            word = Crypto::preEncrypt(word, kPrimePrime, preEncryptIV);
            qDebug() << "Index" << nextAvailableIndex << "Pre-Encrypted word:" << word.toHex();
            qDebug() << "T_i:" << generateTi(word, nextAvailableIndex).toByteArray().toHex();
            crypticRows.last().append(Crypto::arrayXor(word, generateTi(word, nextAvailableIndex++)).toByteArray());
            qDebug() << "Final ciphertext:" << crypticRows.last().last().toHex();
        }
    }

    return crypticRows;
}

DB::RowList DatabaseClient::decryptRows(DB::IndexedRowList crypticRows)
{
    DB::RowList plaintextRows;
    qDebug() << "Decrypting rows";

    foreach(DB::IndexedRow row, crypticRows) {
        quint32 index = row.first;
        qDebug() << "Decrypting row starting at index" << index;
        plaintextRows.append(DB::Row());
        foreach (DB::Word crypticWord, row.second) {
            qDebug() << "Index" << index << "ciphertext:" << crypticWord.toHex();
            qDebug() << "XORing" << crypticWord.left(Crypto::N_BYTES - Crypto::M_BYTES).toHex() << "with" << Crypto::generateS(ks, index).toByteArray().toHex();
            QCA::SecureArray word = Crypto::arrayXor(crypticWord.left(Crypto::N_BYTES - Crypto::M_BYTES), Crypto::generateS(ks, index));
            qDebug() << "Got" << word.toByteArray().toHex();
            qDebug() << "XORing" << Crypto::generateFki(Crypto::generateKi(kk, word), Crypto::generateS(ks, index)).toByteArray().toHex() << "with" << crypticWord.right(Crypto::M_BYTES).toHex();
            word.append(Crypto::arrayXor(Crypto::generateFki(Crypto::generateKi(kk, word), Crypto::generateS(ks, index++)), crypticWord.right(Crypto::M_BYTES)));
            qDebug() << "Got" << word.toByteArray().right(Crypto::M_BYTES).toHex();
            qDebug() << "Pre-encrypted word:" << word.toByteArray().toHex();
            plaintextRows.last().append(Crypto::postDecrypt(word.toByteArray(), kPrimePrime, preEncryptIV));
            qDebug() << "Final plaintext:" << plaintextRows.last().last().toHex();
        }
    }

    return plaintextRows;
}

QPair<DB::Word, QCA::SecureArray> DatabaseClient::encryptWordForSearch(DB::Word plainText)
{
    DB::Word ctxt = Crypto::preEncrypt(plainText, kPrimePrime, preEncryptIV);
    QCA::SecureArray cipherText(ctxt);
    QCA::SecureArray k = Crypto::generateKi(kk, cipherText.toByteArray().left(Crypto::N_BYTES-Crypto::M_BYTES));

    return QPair<DB::Word, QCA::SecureArray>(ctxt, k);
} //end encryptWordForSearch function
