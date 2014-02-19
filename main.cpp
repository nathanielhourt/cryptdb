#include <QCoreApplication>
#include <QDebug>

#include <QtCrypto/qca.h>
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
        }
    }

    //Alice encrypts the database and sends it to Bob
    DB::RowList enc_database = alice.encryptNewRows(rows, 0);
    foreach(DB::Row row, enc_database)
    {
        bob.appendRow(row);
    } //end for each row in encrypted database

    //Search for IP add 0x6FDD4D99E
    quint32 search_term = 0x81A14B33;
    QPair<DB::Word, QCA::SecureArray> search_pair = alice.encryptWordForSearch(QByteArray((char*) &search_term, sizeof(quint32)));
    DB::IndexedRowList found_pairs = bob.findRowsContaining(search_pair, -1);
    qDebug() << "Searched database for" << search_pair.first.toHex() << "and got" << found_pairs.size() << "matching rows.";

    //Nothing is being returned here
    DB::RowList decrypted_found_pairs = alice.decryptRows(found_pairs);

    foreach(DB::Row row, decrypted_found_pairs) {
        foreach(DB::Word w, row) {
            qDebug() << w.toHex();
        } //end for print each entry in the row
        printf("\n");
    } //end for each in which search_term was found
    return 0;
}
