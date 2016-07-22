#include "flactag.h"
#include <QFile>
#include <QDebug>


enum PicType
{
    Other,
    FileIcon32X32PNG,
    OtherFileIcon,
    FrontCover,
    BackCover,
    LeafletPage,
    Media,
    LeadArtist,
    Artist,
    Conductor,
    Band,
    Composer,
    Lyricist,
    RecordingLocation,
    DuringRecording,
    DuringPerformance,
    MovieOrvideoScreenCapture,
    ABrightColouredFish,
    Illustration,
    BandOrArtistLogotype,
    PublisherOrStudioLogotype
};

QString PicTypeStrings[]={
    "Other",
    "32x32 pixels 'file icon' (PNG only)",
    "Other file icon",
    "Cover (front)",
    "Cover (back)",
    "Leaflet page",
    "Media (e.g. label side of CD)",
    "Lead artist/lead performer/soloist",
    "Artist/performer",
    "Conductor",
    "Band/Orchestra",
    "Composer",
    "Lyricist/text writer",
    "Recording Location",
    "During recording",
    "During performance",
    "Movie/video screen capture",
    "A bright coloured fish",
    "Illustration",
    "Band/artist logotype",
    "Publisher/Studio logotype"
};


enum Metadatatype{
    STREAMINFO,
    PADDING,
    APPLICATION,
    SEEKTABLE,
    VORBIS_COMMENT,
    CUESHEET,
    PICTURE,
    Reserved
};


typedef struct MetadataHeader
{
    uchar Type;
    uchar Size[3];
}MetadataHeader;


quint32 getSize24(uchar *x)
{

    char tmp[4];
    tmp[0]=x[2];
    tmp[1]=x[1];
    tmp[2]=x[0];
    tmp[3]=0;

    return *(quint32*)tmp;
}
quint32 getSize24(char *x)
{
    return getSize24((uchar *)x);
}


quint32 getSize32(uchar *x)
{
    char tmp[4];
    tmp[0]=x[3];
    tmp[1]=x[2];
    tmp[2]=x[1];
    tmp[3]=x[0];

    return *(quint32*)tmp;
}
quint32 getSize32(char *x)
{
    return getSize32((uchar *)x);
}


FlacTag::FlacTag(QObject *parent) : QObject(parent)
{

}

FlacTag::~FlacTag()
{

}

void parseVorbisComment(QByteArray array)
{
    char *data=array.data();
    quint32 index=0;
    quint32 size=*(quint32 *)data;
    index+=4;
    QString reference=QString::fromUtf8(array.mid(index,size));
    //qDebug("--size:%d--",size);
    qDebug()<<reference;

    index+=size;

    quint32 count=*(quint32 *)(data+index);
    index+=4;

    for(uint i=0;i<count;i++)
    {
        size=*(quint32 *)(data+index);
        index+=4;
        QString str=QString::fromUtf8(array.mid(index,size));
        index+=size;
        qDebug()<<str;
    }
    //qDebug("--size:%d--",size);
}




void parsePicture(QByteArray array)
{
    char *data=array.data();
    quint32 index=0;
    PicType type=(PicType)getSize32(data);
    qDebug("Type:%d",type);
    index+=4;
    quint32 mimeSize=getSize32(data+index);
    qDebug("mimeSize:%d",mimeSize);
    index+=4;
    QString mimeStr=QString::fromUtf8(array.mid(index,mimeSize));
    index+=mimeSize;
    qDebug()<<mimeStr;

    quint32 disSize=getSize32(data+index);
    qDebug("disSize:%d",disSize);
    index+=4;

    QString disStr=QString::fromUtf8(array.mid(index,disSize));
    index+=disSize;
    qDebug()<<disStr;


    quint32 imgWidth,imgHeight,colorDepth,colorIndex,imgLength;

    imgWidth=getSize32(data+index);
    qDebug("imgWidth:%d",imgWidth);
    index+=4;

    imgHeight=getSize32(data+index);
    qDebug("imgHeight:%d",imgHeight);
    index+=4;

    colorDepth=getSize32(data+index);
    qDebug("colorDepth:%d",colorDepth);
    index+=4;

    colorIndex=getSize32(data+index);
    qDebug("colorIndex:%d",colorIndex);
    index+=4;


    //char *x=(char *)(data+index);
    //uint xx=((((quint32)(x)[0])<<24)+(((quint32)(x)[1])<<16)+(((quint32)(x)[2])<<8)+((quint32)(x)[3]));

    imgLength=getSize32(data+index);
    qDebug("imgLength:%d",imgLength);
    index+=4;

    QByteArray pic=array.mid(index,imgLength);
    qDebug()<<pic.size();

}


void FlacTag::get(QString path)
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
    qDebug()<<"123";

    //可能有id3v2
    char flac[4];
    file.seek(0);
    file.read(flac,4);
    if(flac[0]== 'f'
            && flac[1]== 'L'
            && flac[2]== 'a'
            && flac[3]== 'C')
    {
        qDebug()<<"Is flac";

    }

    while(true)
    {
        MetadataHeader header;
        //qDebug("Pos:%d",file.pos());

        file.read((char *)&header,4);
        bool isLast=header.Type&0x80;
        Metadatatype type=(Metadatatype)(header.Type&0x7F);

        quint32 size=getSize24(header.Size);
        //qDebug("Type:%d Size:%d",type,size);



        if(isLast)
        {
            qDebug()<<"Last";
            break;
        }


        //QByteArray array=file.read(size);
        qDebug("Pos:%d",file.pos());
        switch (type) {
        case STREAMINFO:
            qDebug()<<"STREAMINFO";
            file.seek(file.pos()+size);
            break;
        case PADDING:
            qDebug()<<"PADDING";
            file.seek(file.pos()+size);
            break;
        case VORBIS_COMMENT:
            qDebug()<<"VORBIS_COMMENT";
            parseVorbisComment(file.read(size));
            //file.seek(file.pos()+size);
            break;
        case PICTURE:
            qDebug()<<"PICTURE";
            parsePicture(file.read(size));
            //file.seek(file.pos()+size);
            break;
        case CUESHEET:
            qDebug()<<"CUESHEET";
            file.seek(file.pos()+size);
            break;
        case APPLICATION:
            qDebug()<<"APPLICATION";
            file.seek(file.pos()+size);
            break;
        case SEEKTABLE:
            qDebug()<<"SEEKTABLE";
            file.seek(file.pos()+size);
            break;
        default:
            file.seek(file.pos()+size);
            break;
        }
    }
}
