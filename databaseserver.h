#ifndef DATABASESERVER_H
#define DATABASESERVER_H

#include <QObject>

class DatabaseServer : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseServer(QObject *parent = 0);

    /**
     * @brief Return the next available index within the database
     *
     * This calculates the next available index, which is the same as the number
     * of items currently stored. If there are two rows in the database, four
     * items per row, then the next available index is 8. If the database is
     * empty, the next available index is 0.
     *
     * This information is needed by the client, in order to add new rows to the
     * database, as the encryption of a field is based on its index within the
     * database.
     *
     * @return The next available index
     */
    quint32 nextAvailableIndex();


signals:

public slots:

private:
    QList<QList<quint32> > crypticDatabase;
};

#endif // DATABASESERVER_H
