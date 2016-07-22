#include "oggtag.h"
#include <QFile>
#include <QDebug>


enum HeaderType
{
    Same,
    First,
    Last
};


typedef struct PageHeader
{
    char CapturePattern[4];//页标识，"OggS"的ASCII字符 4F 67 67 53
    char StructureVersion;//版本ID，当前版本默认＝0
    char HeaderTypeFlag;//页头部类型
    uchar GranulePosition[8];//区段位置
    uchar SerialNumber[4];//逻辑流的序列号
    uchar PageSeguenceNumber[4];//本页在逻辑流的序号，OGG解码器据此识别有无页丢失。
    uchar CRCChecksum[4];//循环冗余校验码校验和
    uchar NumberPageSegments;//本页的区段数量，指明区段表中有多少个区段长度，≤255
    //Segment_table ≤255
    //区段长度表，每个字节表示一个区段的长度
}PageHeader;

typedef struct IdentificationHeader
{
    //uchar HeaderTypeFlag;// 　1　＝1：包头类型为标识包
    //char PacketPattern[6];//　　 6　＝76 6F 72 62 69 73，包头标识，vorbis的Ascii码
    uchar VorbisVersion[4];//　　 4　版本
    uchar AudioChannels;//　　 1　声道数目，必须＞0
    uchar AudioSampleRate[4];//　4　音频采样率，必须＞0
    uchar BitrateMaximum[4];//　　4　最大比特率
    uchar BitrateNominal[4];//　　4　标称比特率
    uchar BitrateMinimum[4];//　　4　最小比特率
    uchar blocksize_0:4;//
    uchar blocksize_1:4;//
    uchar FramingFlag;//　　　 1　＝1，边界标志，表示标识头结束
}IdentificationHeader;

typedef struct Header
{
    uchar HeaderTypeFlag;// 3包头类型为标识包
    char PacketPattern[6];//
}Header;



OggTag::OggTag(QObject *parent) : QObject(parent)
{

}

OggTag::~OggTag()
{

}

void OggTag::get(QString path)
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


    for(int k=0;k<2;k++)
    {
        PageHeader header;
        file.read((char *)&header,sizeof(PageHeader));

        qDebug("Type:%d,num:%d",(uchar)header.HeaderTypeFlag,header.NumberPageSegments);

        QByteArray segments=file.read(header.NumberPageSegments);
        uint size=0;
        QList<uint> Sizes;
        uint tmp=(uchar)segments.at(0);
        for(int i=0;i<header.NumberPageSegments;i++)
        {
            size+=(uchar)segments[i];

            if((uchar)segments[i] !=0xFF)
            {
                //qDebug("size:%d %d",Sizes.length(),tmp);
                Sizes.append(tmp);
                tmp=0;
            }
            else
            {
                tmp+=(uchar)segments[i];
            }
        }

        QList<QByteArray> list;

        for(int b=0;b<Sizes.length();b++)
        {
           Header header;
            file.read((char *)&header,sizeof(Header));
            uint sizeReaded=sizeof(Header);

            if(header.HeaderTypeFlag == 0x03)
            {


                quint32 size;
                file.read((char *)&size,4);
                QString vender=QString::fromUtf8(file.read(size));
                sizeReaded+=size+4;
                qDebug()<<vender;

                quint32 count;
                file.read((char *)&count,4);
                sizeReaded+=4;

                for(uint l=0;l<count;l++)
                {
                    file.read((char *)&size,4);
                    qDebug()<<QString::fromUtf8(file.read(size));
                    sizeReaded+=size+4;
                }
                qDebug()<<Sizes.at(b)-sizeReaded;
                //return;
            }

            file.read(Sizes.at(b)-sizeReaded);

           /* QByteArray arr=file.read(Sizes.at(b));

            for(int j=0;j<arr.length();j++)
            {
                printf("%02X ",(uchar)arr.at(j));
            }
            printf("\n---------\n");*/
        }


    }


}
