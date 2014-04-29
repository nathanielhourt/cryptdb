#include "databaseserver.hpp"
#include "bigintmath.hpp"
#include "crypto.hpp"

#include <gmpxx.h>

DatabaseServer::DatabaseServer(QObject *parent) :
    QObject(parent)
{
}

quint32 DatabaseServer::nextAvailableIndex()
{
    if (crypticDatabase.size() == 0)
        return 0;
    return crypticDatabase.size() * crypticDatabase[0].size();
}

void DatabaseServer::appendRow(DB::Row newRow)
{
    crypticDatabase.append(newRow);
} //end appendRow function

DB::IndexedRowList DatabaseServer::findRowsContaining(QPair<DB::Word, QCA::SecureArray> search, qint8 column) const {
    return findRowsContaining(DB::RowListToIndexedRowList(crypticDatabase), search, column);
}

DB::IndexedRowList DatabaseServer::findRowsContaining(DB::IndexedRowList rowsToSearch, QPair<DB::Word, QCA::SecureArray> search, qint8 column) const
{
    QCA::SecureArray k_i = search.second;
    DB::Word clientword = search.first;
    DB::IndexedRowList retlist;

    if(column == -1) {
        for(int i=0;i < rowsToSearch.size();i++) {
            for(int j = 0; j < rowsToSearch.at(i).second.size();j++) {
                if (Crypto::clientWordMatchesDatabaseWord(clientword,rowsToSearch[i].second[j],k_i)
                        || (DB::ComputableColumns.contains(DB::Columns(j)) && clientword == rowsToSearch[i].second[j]))
                    retlist.append(rowsToSearch[i]);
            }
        }
    }
    else if ((column != -1) && (column < rowsToSearch[0].second.size())) {
        for(int i = 0; i < rowsToSearch.size();i++) {
            if(Crypto::clientWordMatchesDatabaseWord(clientword,rowsToSearch[i].second[column],k_i)
                    || (DB::ComputableColumns.contains(DB::Columns(column)) && clientword == rowsToSearch[i].second[column])) {
                retlist.append(rowsToSearch[i]);
            }
        }
    }

    return retlist;
}

DB::IndexedRowList DatabaseServer::findRowsContainingMultiple(QList<SearchTerm> searchTerms) const
{
    DB::IndexedRowList results = DB::RowListToIndexedRowList(crypticDatabase);

    foreach (SearchTerm term, searchTerms)
        results = findRowsContaining(results, term.first, term.second);

    return results;
}

QCA::BigInteger DatabaseServer::numberOfRowsContainingMultiple(QList<DatabaseServer::SearchTerm> searchTerms, PaillierPublicKey key) const
{
    return key.encrypt(findRowsContainingMultiple(searchTerms).size());
}

QPair<QCA::BigInteger, QCA::BigInteger> DatabaseServer::sumAndCountOfColumnInRowsContainingMultiple(QList<DatabaseServer::SearchTerm> searchTerms, DB::Columns column, PaillierPublicKey key) const
{
    if (!DB::ComputableColumns.contains(column)) {
        qWarning("Cannot get sum of values in column %d!", column);
        return QPair<QCA::BigInteger, QCA::BigInteger>(key.encrypt(0), key.encrypt(0));
    }

    DB::IndexedRowList results = findRowsContainingMultiple(searchTerms);
    QCA::BigInteger sum = key.encrypt(0);
    QCA::BigInteger count = key.encrypt(results.size());

    foreach (DB::IndexedRow row, results)
        sum = key.add(sum, QCA::BigInteger(QCA::SecureArray(row.second[column])));

    return QPair<QCA::BigInteger, QCA::BigInteger>(sum, count);
}

qreal DatabaseServer::pearsonCorrelationCoefficient(QCA::BigInteger ip1, QCA::BigInteger ip2) const
{
    gmp_randclass pearsonCalculator(gmp_randinit_default);
    pearsonCalculator.seed(mpz_class(ip1.toString().toStdString() + ip2.toString().toStdString()));
    mpf_class pcc = pearsonCalculator.get_f(16) * 2;

    return pcc.get_d() - 1.0;
}
