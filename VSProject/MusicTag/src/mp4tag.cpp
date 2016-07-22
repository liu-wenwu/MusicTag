#include "mp4tag.h"
#include <QFile>
#include <QDebug>


Mp4Tag::Mp4Tag(QObject *parent) : QObject(parent)
{

}

Mp4Tag::~Mp4Tag()
{

}



quint32 getUINT32(quint8 *data)
{
    quint8 tmp[4];
    tmp[0]=data[3];
    tmp[1]=data[2];
    tmp[2]=data[1];
    tmp[3]=data[0];
    return *(quint32*)tmp;
}

quint16 getUINT16(quint8 *data)
{
    quint8 tmp[2];
    tmp[0]=data[1];
    tmp[1]=data[0];
    return *(quint16*)tmp;
}

uint ceng=0;

void parseMOOV(QByteArray array)
{
    qDebug()<<"------------------------------";
    //qDebug()<<array.length();
    int index=0;

    quint8 *data=(quint8*)array.data();

    while(index<array.length())
    {

        quint32 size=getUINT32(data+index);
        index+=4;
        //qDebug()<<size;



        QByteArray type=array.mid(index,4);
        index+=4;


        QString str="";
        for(uint k=0;k<ceng;k++)
            str+="  ";
        str+=QString::fromLatin1(type)+"    "+QString("%1").arg(size);
        qDebug("%s",str.toLatin1().data());

        if(//type == QByteArray::fromRawData("trak",4) ||
                //    type == QByteArray::fromRawData("mdia",4) ||
                //     type == QByteArray::fromRawData("minf",4) ||
                //    type == QByteArray::fromRawData("stbl",4) ||
                type == QByteArray::fromRawData("udta",4) ||
                type == QByteArray::fromRawData("tags",4) ||
                type == QByteArray::fromRawData("ilst",4)
                )
        {
            //if(array.left(4) ==)
            //qDebug()<<"parse begin:"<<type;


            ceng++;
            QByteArray content=array.mid(index);
            parseMOOV(content);
            ceng--;
            //qDebug()<<"parse end:"<<type;
            index+=size-8;
        }
        else if(type == QByteArray::fromRawData("meta",4))
        {
            //
            //MP4Stream.Read(Version, 1);
            //MP4Stream.Read(Flags, 3);


            // index+=4;
            quint32 count=getUINT32(data+index);
            index+=4;

            qDebug("count:%d",count);

            for(uint k=0;k<count;k++)
            {
                //quint32 flag=*(data+index);
                index+=1;
                quint16 size=getUINT16(data+index);
                index+=2;
                qDebug()<<QString::fromUtf8(array.mid(index,size));
                index+=size;

                quint16 itemcount=getUINT16(data+index);
                index+=2;

                for(int w=0;w<itemcount;w++)
                {
                    quint32 itemsize=getUINT32(data+index);
                    index+=4;
                    qDebug()<<QString::fromUtf8(array.mid(index,itemsize));
                    index+=itemsize;

                }

            }

            //QByteArray content=array.mid(index);
            //parseMOOV(content);
            //index+=size-8-4;

        }
        else
        {

            //qDebug()<<type;
            //QByteArray content=array.mid(index);
            //qDebug()<<content;
            index+=size-8;
        }
    }
}



void Mp4Tag::get(QString path)
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
    qDebug()<<path;



    for(uint i=0;i<3;i++)
    {
        quint8 tmp[4];
        file.read((char *)&tmp,4);
        quint32 size=getUINT32(tmp);
        if(size==1)
        {
            qDebug()<<"size=1";
        }

        //char boxType[4];
        QByteArray arr= file.read(4);

        qDebug()<<arr<<size;
        if(arr == QByteArray::fromRawData("ftyp",4))
        {
            //qDebug()<<"ftyp";
            QByteArray array=file.read(size-8);

            //break;
        }
        else if(arr == QByteArray::fromRawData("moov",4))
        {

            QByteArray array=file.read(size-8);
            parseMOOV(array);
            break;
        }
        else
        {
            file.seek(file.pos()+size-4);

        }
    }
    //qDebug("%X",getUINT32(tmp));


}
