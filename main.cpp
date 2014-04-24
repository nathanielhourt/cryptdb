#include <QCoreApplication>
#include <QDebug>

#include <QtCrypto>
#include <QTextStream>

#include <cstdio>

#include "database.hpp"
#include "databaseclient.hpp"
#include "databaseserver.hpp"
#include "crypto.hpp"

void DB::dumpDB(RowList db) {
    foreach (DB::Row r, db) {
        foreach (DB::Word w, r)
            printf("%s ", w.toHex().data());
        printf ("\n");
    }
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
    qDebug() << DB::database;

    //Read in the database
    DB::RowList rows;
    foreach(QList<quint32> row, DB::database) {
        rows.append(DB::Row());
        foreach (quint32 entry, row) {
            rows.last().append(QByteArray((char*)&entry, sizeof(entry)));
        } //end for each entry in the row
    } //end for each row in the database

    //Alice encrypts the information and sends it to the server
    DB::RowList enc_database = alice.encryptNewRows(rows, 0);
    foreach(DB::Row row, enc_database){
        bob.appendRow(row);
    } //end for each row in encrypted database

    //Search for IP add 0x6FDD4D99E
    quint32 search_term = 0x81A14B33;
    QPair<DB::Word, QCA::SecureArray> search_pair = alice.encryptWordForSearch(QByteArray((char*) &search_term, sizeof(quint32)));
    DB::IndexedRowList found_pairs = bob.findRowsContaining(search_pair, -1);
    qDebug() << "Searched database for" << search_pair.first.toHex() << "and got" << found_pairs.size() << "matching rows.";

    DB::RowList decrypted_found_pairs = alice.decryptRows(found_pairs);
    qDebug() << "Searched column 1 for" << search_pair.first.toHex() << "and got" << found_pairs.size() << "matching rows.";
    foreach(DB::Row row, decrypted_found_pairs) {
      qDebug() << row[0].toHex() << ", " << row[1].toHex() << ", " << print_protocol(row[2]) << ", " << row[3].toHex();
    } //end for each decrypted row
    
    //Decrypt results from found_pairs
    return 0;
} //end main
