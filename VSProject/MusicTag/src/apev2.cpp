#include "apev2.h"
#include <QFile>
#include <QDebug>


QString keyList[52]={
    "Artist",
    "Album",
    "Title",
    "Track",
    "Genre",
    "Year",
    "Comment",
    "Album Artist",
    "Band",
    "Disc",
    "Rating",
    "Temo",
    "Chorus",
    "Composer",
    "Conductor",
    "Instrument",
    "Label",
    "Orchestra",
    "Period",
    "Soloists",
    "Copyright",
    "ISRC",
    "Lyrics",
    "Lyricist",
    "Mood",
    "Style",
    "Subtitle",
    "DiscSubtitle",
    "UPC",
    "ArtistSort",
    "AlbumArtistSort",
    "AlbumSort",
    "ComposerSort",
    "ConductorSort",
    "TitleSort",
    "MixArtist",
    "Arranger",
    "Engineer",
    "Producer",
    "DJMixer",
    "Mixer",
    "Grouping",
    "Compilation",
    "BPM",
    "Media",
    "CatalogNumber",
    "Barcode",
    "EncodedBy",
    "Script",
    "Language",
    "LICENSE",
    "RELEASECOUNTRY"
};



APEV2::APEV2(QObject *parent) : QObject(parent)
{

}

APEV2::~APEV2()
{

}



void APEV2::get(QString path)
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


    //查找id3v1
    file.seek(file.size()-128);
    char id3v1[3];
    file.read(id3v1,3);

    int offset=0;
    if(id3v1[0] == 'T' && id3v1[1] == 'A' && id3v1[2] == 'G')
        offset=-128;

    file.seek(file.size()+offset-32);

    APEV2Footer footer;
    file.read((char *)&footer,sizeof(APEV2Footer));
    QString id=QString::fromLocal8Bit(footer.ID,8);
    qDebug()<<id;
    qDebug()<<*(uint *)footer.Size;
    uint size=*(uint *)footer.Size;

    file.seek(file.size()+offset-size);


    //file.read(id3v1,3);
    //printf("%x %x %x",id3v1[0],id3v1[1],id3v1[2]);
    //uint  count=0;
    while(file.pos()<file.size()+offset-32)
    {
        qint32 size;
        file.read((char *)&size,4);
        qint32 flags;
        file.read((char *)&flags,4);


        qDebug()<<size;


        //bool ReadOnly=flags&0x01;


       // APEV2FrameType framesflag=APEV2FrameType((flags&0x06)>>1);


        qint64 index=0;
        qint64 pos=file.pos();
        char pre;
        //find pre
        while(true)
        {
            if(file.getChar(&pre))
            {
                if(pre == '\0')
                {
                    index=file.pos();
                    //qDebug()<<"123";
                    break;
                }
            }
            else break;
        }
        file.seek(pos);
        QByteArray frameid= file.read(index-pos);
        QByteArray value= file.read(size);

        frames.insert(QString::fromUtf8(frameid),value);
        //qDebug()<<(uchar)value.at(0);
        qDebug()<<frameid;

        qDebug()<<QString::fromUtf8(value);
    }

}
