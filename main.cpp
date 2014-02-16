#include <QCoreApplication>

#include <QtCrypto>
#include <QTextStream>

#include <cstdio>

int main(int argc, char *argv[])
{
    QCA::Initializer init;
    Q_UNUSED(init);
    QCoreApplication a(argc, argv);

    QTextStream qin(stdin);
    QTextStream qout(stdout);

    QCA::SymmetricKey key(16);
    QCA::InitializationVector iv(16);

    QCA::Cipher cipher(QString("aes128"),QCA::Cipher::CBC,
                       QCA::Cipher::DefaultPadding,
                       QCA::Encode,
                       key, iv);

    QCA::SecureArray msg = "Hello, QCA!";
    msg = cipher.process(msg);
    qout << "ctxt: " << qPrintable(msg.toByteArray().toBase64()) << endl;

    cipher.setup(QCA::Decode, key, iv);

    msg = cipher.process(msg);
    qout << "ptxt: " << msg.toByteArray() << endl;

    qout << "Enter a message: " << flush;
    msg = qin.readLine().toLocal8Bit();

    cipher.setup(QCA::Encode, key, iv);

    msg = cipher.process(msg);
    qout << "ctxt: " << qPrintable(msg.toByteArray().toBase64()) << endl;

    cipher.setup(QCA::Decode, key, iv);

    msg = cipher.process(msg);
    qout << "ptxt: " << msg.toByteArray() << endl;

    return 0;
}
