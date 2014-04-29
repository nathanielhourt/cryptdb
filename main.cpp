#include <QCoreApplication>
#include <QDebug>
#include <QtCrypto>
#include <QTextStream>
#include <cstdio>
#include <gmpxx.h>

#include "database.hpp"
#include "databaseclient.hpp"
#include "databaseserver.hpp"
#include "crypto.hpp"
#include "bigintmath.hpp"
#include "paillierprivatekey.hpp"

void DB::dumpDB(RowList db) {
    foreach (DB::Row r, db) {
        foreach (DB::Word w, r)
            printf("%s ", w.toHex().data());
        printf ("\n");
    }
}

DB::IndexedRowList DB::RowListToIndexedRowList(DB::RowList rows, quint8 startingIndex) {
    DB::IndexedRowList db;

    foreach (DB::Row row, rows) {
        db.append(DB::IndexedRow(startingIndex, row));
        startingIndex += row.size();
    }

    return db;
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

    QTextStream qin(stdin);
    QTextStream qout(stdout);
    QCA::BigInteger sourceIP;
    QCA::BigInteger destIP;

    //Valid IPs
    const static QList<QCA::BigInteger> IPs = {0x6FDD4D9E, 0x81A14B33,0x81A14B9E, 0xEFFFFFFA,0xAE892A4B, 0x81A14B33,
                                      0x6FDD4D9E, 0x81A14BFF, 0x8D657494, 0x81A14B33,0x6FDD4D9E, 0x81A14BFF,
                                       0x8D657494, 0x81A14B33,0x6FDD4D9E,0x81A14B33, 0xC0A80167,
                                        0x81A14B33, 0xC0A80167,0x81A14BFF, 0xAE892A4B, 0x81A14B33,0xAE892A4B,
                                       0x81A14B33,0xA29FF2A5, 0x81A14B33, 0xAE892A4B, 0xC0A80167,0x6FDD4D9E,
                                       0xC0A80167,0x81A14B33, 0xFFFFFFFF,0x81A14B33,0x81A14B9E,0xAE892A4B,
                                       0xEFFFFFFA,0xA29FF2A5, 0x81A14B33, 0x8D657494, 0x81A14B33
                                      };

    //Read in the database
    DB::RowList rows;
    foreach(QList<quint32> row, DB::database) {
        rows.append(DB::Row());
        foreach (quint32 entry, row) {
            mpz_class n(entry);
            rows.last().append(QCA::BigInteger(n.get_str().c_str()).toArray().toByteArray());
        } //end for each entry in the row
    } //end for each row in the database

    //Alice encrypts the information and sends it to the server
    DB::RowList enc_database = alice.encryptNewRows(rows, 0);
    foreach(DB::Row row, enc_database){
        bob.appendRow(row);
    } //end for each row in encrypted database


    int selection;
    bool inputflag = 1;
    while (inputflag != 0){
        qout<< "Enter selection:"<<endl;
        qout<< "1: SourceIP_Average_MSG_Len"<<endl;
        qout<< "2: DestinationIP_Average_MSG_Len"<<endl;
        qout<< "3: From_To_Total#ofMsgs"<<endl;
        qout<< "4: Total_#of_Outbound_Msgs"<<endl;
        qout<< "5: Total_#of_Inbound_Msgs"<<endl;
        qout<< "6: Pearson_Corr_Coeff??"<<endl;
        qout<< "0: Exit"<<endl;

        qin >> selection;

        if(selection == 0){
            inputflag = 0;
        }
        if(selection == 1){
            qout<<"Enter SourceIP Address in hex"<<endl;
            qin << sourceIP;
            while(!(IPs.contains(sourceIP))) { //check validity of source IP
                qout<<"Invalid hex input. Please try again"<<endl;
                qin << sourceIP;
            }

            //Call Average_Msg_Len function
            DatabaseServer::SearchWord search_pair = alice.encryptWordForSearch(QCA::BigInteger(sourceIP.get_str().c_str()).toArray().toByteArray());
            QList<DatabaseServer::SearchTerm> searchTerms;
            searchTerms.append(DatabaseServer::SearchTerm(search_pair, DB::SourceIP));

            QPair<QCA::BigInteger, QCA::BigInteger> result = bob.sumAndCountOfColumnInRowsContainingMultiple(searchTerms, DB::Length, alice.getPublicKey());
            result.first = alice.decryptNumber(result.first);
            result.second = alice.decryptNumber(result.second);
            qout<<"Source IP average message length:"<<endl;
            qDebug() << (result.first / result.second).toString();
        }
        if(selection == 2){
            qout<<"Enter DestinationIP Address in hex"<<endl;
            qin << destIP;
            while(!(IPs.contains(destIP))) { //check validity of destination IP
                qout<<"Invalid hex input. Please try again"<<endl;
                qin << destIP;
            }
            //Call Average_Msg_Len function
            DatabaseServer::SearchWord search_pair = alice.encryptWordForSearch(QCA::BigInteger(destIP.get_str().cstr()).toArray().toByteArray());
            QList<DatabaseServer::SearchTerm> searchTerms;
            searchTerms.append(DatabaseServer::SearchTerm(search_pair,DB::DestinationIP));

            QPair<QCA::BigInteger, QCA::BigInteger> result = bob.sumAndCountOfColumnInRowsContainingMultiple(searchTerms, DB::Length, alice.getPublicKey());
            result.first = alice.decryptNumber(result.first);
            result.second = alice.decryptNumber(result.second);
            qout<<"Destination IP average message length:"<<endl;
            qDebug() << (result.first / result.second).toString();
        }
        if(selection == 3){
            qout<<"Enter SourceIP Addressi in hex"<<endl;
            qin << sourceIP;
            while(!(IPs.contains(sourceIP))) { //check validity of source IP
                qout<<"Invalid hex input. Please try again"<<endl;
                qin << sourceIP;
            }
            qout<<"Enter DestinationIP Address in hex"<<endl;
            qin << destIP;
            while(!(IPs.contains(destIP))) { //check validity of destination IP
                qout<<"Invalid hex input. Please try again"<<endl;
                qin << destIP;
            }

            //Call From_to_Total_Msgs function
            DatabaseServer::SearchWord search_pair1 = alice.encryptWordForSearch(QCA::BigInteger(sourceIP.get_str().cstr()).toArray().toByteArray());
            QList<DatabaseServer::SearchTerm> searchterms;
            searchTerms.append(DatabaseServer::SearchTerm(search_pair1,DB::SourceIP));

            DatabaseServer::SearchWord search_pair2 = alice.encryptWordForSearch(QCA::BigInteger(destIP.get_str().cstr()).toArray().toByteArray());
            searchTerms.append(DatabaseServer::SearchTerm(search_pair2,DB::DestinationIP));

            QCA::BigInteger result = bob.findRowsContainingMultiple(searchTerms);
            result=alice.decryptNumber(result);
            qout<<"Total number of messages from source IP to destination IP"<<endl;
            qout<<result;

        }
        if(selection == 4){
            qout<<"Enter SourceIP Address in hex"<<endl;
            qin << sourceIP;
            while(!(IPs.contains(sourceIP))) { //check validity of destination IP
                qout<<"Invalid hex input. Please try again"<<endl;
                qin << sourceIP;
            }
            //Call Total_Outbound_Msgs function
            DatabaseServer::SearchWord search_pair = alice.encryptWordForSearch(QCA::BigInteger(sourceIP.get_str().cstr()).toArray().toByteArray());
            QList<DatabaseServer::SearchTerm> searchTerms;
            searchTerms.append(DatabaseServer::SearchTerm(search_pair,DB::SourceIP));

            QCA::BigInteger result = bob.findRowsContainingMultiple(searchTerms);
            result=alice.decryptNumber(result);
            qout<<"Total number of outbound messages from given source IP:"<<endl;
            qout<<result;
        }
        if(selection == 5){
            qout<<"Enter DestinationIP Address in hex"<<endl;
            qin << destIP;
            while(!(IPs.contains(destIP))) { //check validity of destination IP
                qout<<"Invalid hex input. Please try again"<<endl;
                qin << destIP;
            }
            //Call Total_Inbound_Msgs function
            DatabaseServer::SearchWord search_pair = alice.encryptWordForSearch(QCA::BigInteger(destIP.get_str().cstr()).toArray().toByteArray());
            QList<DatabaseServer::SearchTerm> searchTerms;
            searchTerms.append(DatabaseServer::SearchTerm(search_pair,DB::DestinationIP));

            QCA::BigInteger result = bob.findRowsContainingMultiple(searchTerms);
            result=alice.decryptNumber(result);
            qout<<"Total number of inbound messages from given destination IP:"<<endl;
            qout<<result;
        }
        if(selection == 6){
            qout<<"Enter 1st IP address in hex"<<endl;
            qin << sourceIP;
            while(!(IPs.contains(sourceIP))) { //check validity of destination IP
                qout<<"Invalid hex input. Please try again"<<endl;
                qin << sourceIP;
            }
            qout<<"Enter 2nd IP address in hex"<<endl;
            qin << destIP;
            while(!(IPs.contains(destIP))) { //check validity of destination IP
                qout<<"Invalid hex input. Please try again"<<endl;
                qin << destIP;
            }
            //Call Pearson_Corr_Coeff function?
        }

    }


    return 0;
} //end main
