#include <QCoreApplication>

#include <QtCrypto/qca.h>
#include <QTextStream>

#include <cstdio>

#include "database.hpp"
#include "databaseclient.hpp"
#include "databaseserver.hpp"
#include "crypto.hpp"
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

    DB::RowList rows;
    foreach(QList<quint32> row, DB::database) {
        rows.append(DB::Row());
        foreach (quint32 entry, row)
            rows.last().append(QByteArray::fromRawData((char*)&entry, sizeof(entry)));
    }

    return 0;
}
