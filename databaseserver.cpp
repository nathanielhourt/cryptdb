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

DB::IndexedRowList DatabaseServer::findRowsContaining(QPair<DB::Word, QCA::SecureArray> search, qint8 column) {
    QCA::SecureArray k_i = search.second;
    DB::Word clientword = search.first;
    DB::IndexedRowList retlist;

    if(column == -1) {
        for(int i=0;i < crypticDatabase.size();i++) {
            for(int j = 0; j < crypticDatabase.at(i).size();j++) {
                if (Crypto::clientWordMatchesDatabaseWord(clientword,crypticDatabase.at(i).at(j),k_i)) {
                    //Add current row to retlist.
                    DB::Index index = (i * crypticDatabase[0].size()) + j;
                    retlist.append(QPair<DB::Index,QList<DB::Word> >(index,crypticDatabase[i]));
                }
            }
        }
    }
    else if ((column != -1) && (column < crypticDatabase.size())) {
        for(int i = 0; i < crypticDatabase[column].size();i++) {
            if(Crypto::clientWordMatchesDatabaseWord(clientword,crypticDatabase[column][i],k_i)) {
                DB::Index index = (column * crypticDatabase[0].size()) + i;
                retlist.append(QPair<DB::Index,QList<DB::Word> >(index,crypticDatabase[column]));
            }
        }
    }

    return retlist;
}
