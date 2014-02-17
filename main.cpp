#include <QCoreApplication>

#include <QtCrypto/qca.h>
#include <QTextStream>

#include <cstdio>

#include "database.hpp"
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
    int aa = 5;
    DB::Word test = QByteArray::fromRawData((char*)&aa,sizeof(int));
    crypt.PreEncrypt(test);

    return 0;
}
