#include "databaseclient.hpp"

DatabaseClient::DatabaseClient(QObject *parent) :
    QObject(parent)
{
    //A sixteen byte random kPrime
    kPrime = QCA::Random::randomArray(16);
}

DB::Word DatabaseClient::encryptWordForSearch(DB::Word plainText)
{
    Crypto crypt;
    //Split word into X_i
    DB::Word ctxt = crypt.preEncrypt(plainText);
    QCA::SecureArray cipherText(ctxt);
    QCA::SecureArray k = crypt.generateKi(kPrime, cipherText);

    cipherText.append(k);
    return cipherText.toByteArray();
} //end encryptWordForSearch function
