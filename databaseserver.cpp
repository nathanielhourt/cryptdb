#include "databaseserver.hpp"
#include "crypto.hpp"
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
                if (Crypto::clientWordMatchesDatabaseWord(clientword,rowsToSearch.at(i).second.at(j),k_i))
                    retlist.append(rowsToSearch[i]);
            }
        }
    }
    else if ((column != -1) && (column < rowsToSearch[0].second.size())) {
        for(int i = 0; i < rowsToSearch.size();i++) {
            if(Crypto::clientWordMatchesDatabaseWord(clientword,rowsToSearch[i].second[column],k_i)) {
                retlist.append(rowsToSearch[i]);
            }
        }
    }

    return retlist;
}
