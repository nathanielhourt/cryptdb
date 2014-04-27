#pragma once

namespace DB {
typedef QByteArray Word;
typedef quint32 Index;
typedef QList<Word> Row;
typedef QList<Row> RowList;
typedef QPair<Index, Row> IndexedRow;
typedef QList<IndexedRow> IndexedRowList;

enum Protocol_Names {TCP, DB_LSP_DISC, SSDP, SNMP};
enum Columns {SourceIP, DestinationIP, Protocol, Length};

const static QList<QList<quint32> > database = {
    {0x6FDD4D9E, 0x81A14B33, TCP, 57},
    {0x81A14B9E, 0xEFFFFFFA, SSDP, 175},
    {0x81A14B9E, 0xEFFFFFFA, SSDP, 175},
    {0xAE892A4B, 0x81A14B33, TCP, 66},
    {0xAE892A4B, 0x81A14B33, TCP, 66},
    {0x6FDD4D9E, 0x81A14BFF, DB_LSP_DISC, 195},
    {0x8D657494, 0x81A14B33, TCP, 54},
    {0x6FDD4D9E, 0x81A14B33, TCP, 57},
    {0x81A14B33, 0xC0A80167, SSDP, 175},
    {0x81A14B33, 0xC0A80167, SNMP, 121},
    {0x81A14B33, 0x81A14BFF, DB_LSP_DISC, 195},
    {0xAE892A4B, 0x81A14B33, TCP, 54},
    {0xAE892A4B, 0x81A14B33, SSDP, 175},
    {0xA29FF2A5, 0x81A14B33, TCP, 54},
    {0xAE892A4B, 0xC0A80167, TCP, 54},
    {0x6FDD4D9E, 0xC0A80167, SNMP, 121},
    {0x81A14B33, 0xFFFFFFFF, DB_LSP_DISC, 195},
    {0x81A14B33, 0xFFFFFFFF, DB_LSP_DISC, 195},
    {0x81A14B9E, 0xEFFFFFFA, SSDP, 175},
    {0xAE892A4B, 0xEFFFFFFA, SSDP, 175},
    {0xA29FF2A5, 0x81A14B33, TCP, 66},
    {0xA29FF2A5, 0x81A14B33, TCP, 66},
    {0x8D657494, 0x81A14B33, TCP, 54},
    {0x8D657494, 0x81A14B33, TCP, 66}
};

void dumpDB(RowList db);
IndexedRowList RowListToIndexedRowList(RowList rows, quint8 startingIndex = 0);
}
