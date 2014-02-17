#ifndef DATABASECLIENT_H
#define DATABASECLIENT_H

#include <QObject>

class DatabaseClient : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseClient(QObject *parent = 0);

signals:

public slots:

};

#endif // DATABASECLIENT_H
