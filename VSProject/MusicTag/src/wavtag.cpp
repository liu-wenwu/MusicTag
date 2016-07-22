#include "wavtag.h"
#include <QFile>
#include <QDebug>


#define getTagSize(x) (((uint)x[0]<<21)+((uint)x[1]<<14)+((uint)x[2]<<7)+x[3])

typedef struct ID3V2Header
{
    char Identifier[3];
    char Version [2];
    uchar Flags;
    uchar Size[4];

}ID3V2Header;

typedef struct WAVEFORMAT；
{
    qint16 wFormatag;
    qint16 nChannls;
    qint32 nSamplesPerSec;
    qint32 nAvgBytesperSec;
    qint16 nBlockAlign;
}WAVEFORMAT;



WavTag::WavTag(QObject *parent) : QObject(parent)
{

}

WavTag::~WavTag()
{

}





void WavTag::get(QString path)
{

    QFile file(path);
    if(!file.exists())
    {
        return;
    }


    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }



    uint id3v2size=0;
    ID3V2Header header;
    file.seek(0);
    file.read((char *)&header,sizeof(ID3V2Header));
    if(QString::fromLocal8Bit(header.Identifier,3) == "ID3")
        id3v2size=getTagSize(header.Size);

    file.seek(id3v2size);
    char RIFF[4];
    file.read(RIFF,4);

    if(QString::fromLocal8Bit(RIFF,4) != "RIFF")
        return;

    qint32 RIFFChunkSize;
    file.read((char *)&RIFFChunkSize,4);
    qDebug()<<RIFFChunkSize;

    char ChunkID[4];
    file.read(ChunkID, 4);
    if(QString::fromLocal8Bit(ChunkID,4) != "WAVE")
        return;

    char FMT[4];
    file.read(FMT,4);
    if(QString::fromLocal8Bit(FMT,4) != "fmt ")
        return;

    qint32 fmtSize;
    file.read((char *)&fmtSize,4);

    if(fmtSize != sizeof(WAVEFORMAT))
        return;

    WAVEFORMAT waveFmt;
    file.read((char *)&waveFmt,sizeof(WAVEFORMAT));


    char DATA[4];
    file.read(DATA, 4);
    if(QString::fromLocal8Bit(DATA,4) != "data")
        return;
    qint32 dataSize;


    file.read((char *)&dataSize,4);
    file.seek(file.pos()+dataSize);


    char LIST[4];
    file.read(LIST, 4);
    if(QString::fromLocal8Bit(LIST,4) != "LIST")
        return;
    qint32 listSize;

    file.read((char *)&listSize,4);

    file.seek(file.pos()+4);

    // QByteArray info= file.read(listSize);

    //qDebug()<<info;

    qint32 count=4;
    while(count < listSize && !file.atEnd())
    {
        char key[4];
        file.read(key,4);
        qDebug()<<QString::fromLocal8Bit(key,4);
        qint32 size=0;
        file.read((char *)&size,4);
        //qDebug()<<size;
        QByteArray value=file.read(size);

        //qDebug()<<(uchar)value.at(value.length()-2);
        while(true)
        {
            char ch;
            bool ret=file.getChar(&ch);
            if(!ret || ch != '\0') break;
            count++;
        }

        file.seek(file.pos()-1);
        count+=8+size;
        qDebug()<<value;
    }
    //qDebug()<<dataSize;
}
