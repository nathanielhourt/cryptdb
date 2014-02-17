#include <QCoreApplication>

#include <QtCrypto>
#include <QTextStream>

#include <cstdio>

#include "raw_plaintext.hpp"

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

    return 0;
}
