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

    QCA::SymmetricKey key(16);
    datastream << key.toByteArray();

    QByteArray holder;
    QDataStream str(data);
    str >> holder;
    QCA::SymmetricKey key2(holder);

    if (key == key2)
        printf("Good\n");
    else
        printf("Bad\n");

    Crypto crypt;
    DatabaseClient alice;
    DB::Word text("test");
    QPair<DB::Word, QCA::SecureArray> send = alice.encryptWordForSearch(text);
    qDebug() << DB::database;

    DB::RowList rows;
    foreach(QList<quint32> row, DB::database) {
        rows.append(DB::Row());
        foreach (quint32 entry, row) {
            rows.last().append(QByteArray((char*)&entry, sizeof(entry)));
        }
    }
    DB::dumpDB(rows);
    DB::RowList crypticRows = alice.encryptNewRows(rows, 0);
    DB::dumpDB(crypticRows);

    DB::IndexedRowList decryptableRows;
    DB::Index i = 0; i-=4;
    foreach (DB::Row row, crypticRows)
        decryptableRows.append(QPair<DB::Index, DB::Row>(i += 4, row));
    DB::RowList newRows = alice.decryptRows(decryptableRows);

    if (rows == newRows)
        printf("Good\n");
    else
        printf("Bad\n");

    return 0;
}
