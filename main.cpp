#include <QCoreApplication>
#include <QDebug>
#include <QtCrypto>
#include <QTextStream>
#include <cstdio>
#include <gmpxx.h>

#include "database.hpp"
#include "databaseclient.hpp"
#include "databaseserver.hpp"
#include "crypto.hpp"
#include "bigintmath.hpp"
#include "paillierprivatekey.hpp"

void DB::dumpDB(RowList db) {
    foreach (DB::Row r, db) {
        foreach (DB::Word w, r)
            printf("%s ", w.toHex().data());
        printf ("\n");
    }
}

DB::IndexedRowList DB::RowListToIndexedRowList(DB::RowList rows, quint8 startingIndex) {
    DB::IndexedRowList db;

    foreach (DB::Row row, rows) {
        db.append(DB::IndexedRow(startingIndex, row));
        startingIndex += row.size();
    }

    return db;
}

char* print_protocol(QByteArray protocol)
{
  quint32 proto_value = protocol.toHex().left(2).toUInt();
  if(proto_value == DB::TCP)
  {
    return (char*)"TCP";
  } //end if TCP
  else if(proto_value == DB::DB_LSP_DISC) {
    return (char*)"DB_LSP_DISC";
  } //end else if DB_LSP_DISC
  else if(proto_value == DB::SSDP)
  {
    return (char*)"SSDP";
  } //end else if SSDP
  else
  {
    return (char*)"SNMP";
  } //end else SNMP
} //end print_protocol

int main(int argc, char *argv[])
{
    QCA::Initializer init;
    Q_UNUSED(init);
    QCoreApplication a(argc, argv);
    Q_UNUSED(a);

    DatabaseClient alice;
    DatabaseServer bob;

    //Read in the database
    DB::RowList rows;
    foreach(QList<quint32> row, DB::database) {
        rows.append(DB::Row());
        foreach (quint32 entry, row) {
            mpz_class n(entry);
            rows.last().append(QCA::BigInteger(n.get_str().c_str()).toArray().toByteArray());
        } //end for each entry in the row
    } //end for each row in the database

    //Alice encrypts the information and sends it to the server
    DB::RowList enc_database = alice.encryptNewRows(rows, 0);
    foreach(DB::Row row, enc_database){
        bob.appendRow(row);
    } //end for each row in encrypted database

    mpz_class search_term = 0x8D657494;
    DatabaseServer::SearchWord search_pair = alice.encryptWordForSearch(QCA::BigInteger(search_term.get_str().c_str()).toArray().toByteArray());
    QList<DatabaseServer::SearchTerm> searchTerms;
    searchTerms.append(DatabaseServer::SearchTerm(search_pair, DB::SourceIP));

    QPair<QCA::BigInteger, QCA::BigInteger> result = bob.sumAndCountOfColumnInRowsContainingMultiple(searchTerms, DB::Length, alice.getPublicKey());
    result.first = alice.decryptNumber(result.first);
    result.second = alice.decryptNumber(result.second);
    qDebug() << (result.first / result.second).toString();

    return 0;
} //end main
