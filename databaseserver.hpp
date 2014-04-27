#ifndef DATABASESERVER_H
#define DATABASESERVER_H

#include <QObject>
#include <QtCrypto>
#include "database.hpp"
#include "paillierpublickey.hpp"

class DatabaseServer : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseServer(QObject *parent = 0);

    typedef QPair<DB::Word, QCA::SecureArray> SearchWord;
    typedef QPair<SearchWord, DB::Index> SearchTerm;

    /**
     * @brief Return the next available index within the database
     *
     * This calculates the next available index, which is the same as the number
     * of items currently stored. If there are two rows in the database, four
     * items per row, then the next available index is 8. If the database is
     * empty, the next available index is 0.
     *
     * This information is needed by the client, in order to add new rows to the
     * database, as the encryption of a field is based on its index within the
     * database.
     *
     * @return The next available index
     */
    DB::Index nextAvailableIndex();

    /**
     * @brief Adds a new row to the database
     * @param newRow The new row to add
     *
     * This method adds a new row to the end of the database. The row should be
     * properly encrypted already, and must have taken nextAvailableIndex() into
     * account.
     */
    void appendRow(DB::Row newRow);

    /**
     * @brief Return all rows containing the specified word
     * @param word The word to search for
     * @param column The column to search for the word in; if -1, search all
     *
     * This method searches the encrypted database for word. If column is not
     * -1, then only the specified column is searched, but the entire matching
     * row will be returned. If column is -1, then all columns are searched.
     *
     * If column is less than -1 or greater than the highest column index, the
     * return value is QList<QList<quint32> >()
     *
     * @return Indexed lsit of all rows containing the specified word
     */
    DB::IndexedRowList findRowsContaining(QPair<DB::Word, QCA::SecureArray> search, qint8 column = -1) const;
    /**
     * @brief This is an overloaded function
     * @param rowsToSearch A list of rows to search, instead of the entire database
     * @return Rows in rowsToSearch which match the search word
     */
    DB::IndexedRowList findRowsContaining(DB::IndexedRowList rowsToSearch, QPair<DB::Word, QCA::SecureArray> search, qint8 column = -1) const;

    /**
     * @brief Similar to findRowsContaining, except it performs multiple searches
     * @param searchTerms A list of <searchTerm, column> pairs. Any columns which are -1 mean match in any column.
     * @return Rows in database which match all search terms
     */
    DB::IndexedRowList findRowsContainingMultiple(QList<SearchTerm> searchTerms) const;

    /**
     * @brief Same as findRowsContainingMultiple, except it returns the number of matches
     * @param searchTerms A list of <searchTerm, column> pairs. Any columns which are -1 mean match in any column.
     * @param key Public key to encrypt result with
     * @return Paillier-encrypted count of rows that match all terms
     */
    QCA::BigInteger numberOfRowsContainingMultiple(QList<SearchTerm> searchTerms, PaillierPublicKey key) const;
signals:

public slots:

private:
    DB::RowList crypticDatabase;
};

#endif // DATABASESERVER_H
