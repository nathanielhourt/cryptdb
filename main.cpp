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
    foreach (Row r, db) {
        foreach (Word w, r)
            printf("%s ", w.toHex().data());
        printf ("\n");
    }
}

int main(int argc, char *argv[])
{
    QCA::Initializer init;
    Q_UNUSED(init);
    QCoreApplication a(argc, argv);

    QByteArray data;
    QDataStream datastream(&data, QIODevice::ReadWrite);

    Crypto crypt;
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

    foreach(QList<quint32> row, DB::database) {
        DB::Row tmp_row;
        foreach (quint32 entry, row) {
            tmp_row.append(QByteArray((char*)&entry, sizeof(entry)));
        } //end for each byte array
        bob.appendRow(tmp_row);
    }
    return 0;
}
