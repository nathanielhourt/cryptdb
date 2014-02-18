#include "databaseserver.hpp"

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
