#ifndef DATABASECLIENT_H
#define DATABASECLIENT_H

#include <QObject>

#include "database.hpp"
#include "crypto.hpp"
class DatabaseClient : public QObject
{
    Q_OBJECT
private:
    QCA::SecureArray kPrime;
public:
    explicit DatabaseClient(QObject *parent = 0);

    /**
     * @brief Encrypts the plaintext newRows to be stored in the database
     * @param newRows The plaintext rows to be encrypted
     * @param nextAvailableIndex The next available index on the server
     *
     * This method takes rows of plaintext data that need to be stored on the server, and encrypts them
     * based on their index in the server, starting with nextAvailableIndex.
     *
     * @return The encrypted rows, which can be appended to the database
     */
    DB::RowList encryptNewRows(DB::RowList newRows, DB::Index nextAvailableIndex);

    /**
     * @brief Decrypt the encrypted crypticRows from the database server for reading
     * @param crypticRows The encrypted and indexed rows from the database server, which are to be
     * decrypted
     *
     * After making a query to the server, it will return an indexed list of encrypted rows. This method
     * will decrypt those rows and return them as plaintext.
     *
     * @return A decrypted version of crypticRows
     */
    DB::RowList decryptRows(DB::IndexedRowList crypticRows);

    /**
     * @brief Encrypts the word plainText for searching within the database server
     * @param plainText The plaintext word which is to be searched for in the database
     *
     * In order for the database server to search for a word, that word must be encrypted. This method
     * will take a plaintext word and encrypt it so that the database server can search for it.
     *
     * @return An encrypted version of plainText which the server can search the database for
     */
    DB::Word encryptWordForSearch(DB::Word plainText);

signals:

public slots:

};

#endif // DATABASECLIENT_H
