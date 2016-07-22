#include "wmatag.h"
#include <QFile>
#include <QDebug>
#include <QTextCodec>
#include <QImage>


#define getUINT64(x) (*(quint64 *)(x))
#define getUINT32(x) (*(quint32 *)(x))
#define getUINT16(x) (*(quint16 *)(x))


typedef struct WMAHeader
{
    uchar Flag[16];
    uchar Size[8];
    uchar Unknown[6];
}WMAHeader;


typedef struct FrameHeader
{
    uchar Flag[16];
    uchar Size[8];
}FrameHeader;

typedef struct StdFrameSize
{
    uchar Title[2];
    uchar Artist[2];
    uchar Copyight[2];
    uchar Comment[2];
    uchar Unknown[2];
}StdFrameSize;


enum DescriptorValueDataType{
    UnicodeString,//varies
    ByteArray ,//varies
    BOOL,//32
    DWORD,//32
    QWORD,//64
    WORD,//16
    GUID
};



enum GUIDType
{
    ///Top-level ASF object GUIDS
    ASF_Header_Object,// 75B22630-668E-11CF-A6D9-00AA0062CE6C
    ASF_Data_Object,//75B22636-668E-11CF-A6D9-00AA0062CE6C
    ASF_Simple_Index_Object,//33000890-E5B1-11CF-89F4-00A0C90349CB
    ASF_Index_Object,//D6E229D3-35DA-11D1-9034-00A0C90349BE
    ASF_Media_Object_Index_Object,//FEB103F8-12AD-4C64-840F-2A1D2F7AD48C
    ASF_Timecode_Index_Object,//3CB73FD0-0C4A-4803-953D-EDF7B6228F0C

    /// Header Object GUIDs
    ASF_File_Properties_Object,//8CABDCA1-A947-11CF-8EE4-00C00C205365
    ASF_Stream_Properties_Object,//B7DC0791-A9B7-11CF-8EE6-00C00C205365
    ASF_Header_Extension_Object,//5FBF03B5-A92E-11CF-8EE3-00C00C205365
    ASF_Codec_List_Object,//86D15240-311D-11D0-A3A4-00A0C90348F6
    ASF_Script_Command_Object,//1EFB1A30-0B62-11D0-A39B-00A0C90348F6
    ASF_Marker_Object,//F487CD01-A951-11CF-8EE6-00C00C205365
    ASF_Bitrate_Mutual_Exclusion_Object,//D6E229DC-35DA-11D1-9034-00A0C90349BE
    ASF_Error_Correction_Object,//75B22635-668E-11CF-A6D9-00AA0062CE6C
    ASF_Content_Description_Object,//75B22633-668E-11CF-A6D9-00AA0062CE6C
    ASF_Extended_Content_Description_Object,//D2D0A440-E307-11D2-97F0-00A0C95EA850
    ASF_Content_Branding_Object,//2211B3FA-BD23-11D2-B4B7-00A0C955FC6E
    ASF_Stream_Bitrate_Properties_Object,//7BF875CE-468D-11D1-8D82-006097C9A2B2
    ASF_Content_Encryption_Object,//2211B3FB-BD23-11D2-B4B7-00A0C955FC6E
    ASF_Extended_Content_Encryption_Object,//298AE614-2622-4C17-B935-DAE07EE9289C
    ASF_Digital_Signature_Object,//2211B3FC-BD23-11D2-B4B7-00A0C955FC6E
    ASF_Padding_Object,//1806D474-CADF-4509-A4BA-9AABCB96AAE8

    ///Header Extension Object GUIDs
    ASF_Extended_Stream_Properties_Object,//14E6A5CB-C672-4332-8399-A96952065B5A
    ASF_Advanced_Mutual_Exclusion_Object,//A08649CF-4775-4670-8A16-6E35357566CD
    ASF_Group_Mutual_Exclusion_Object,//D1465A40-5A79-4338-B71B-E36B8FD6C249
    ASF_Stream_Prioritization_Object,//D4FED15B-88D3-454F-81F0-ED5C45999E24
    ASF_Bandwidth_Sharing_Object,//A69609E6-517B-11D2-B6AF-00C04FD908E9
    ASF_Language_List_Object,//7C4346A9-EFE0-4BFC-B229-393EDE415C85
    ASF_Metadata_Object,//C5F8CBEA-5BAF-4877-8467-AA8C44FA4CCA
    ASF_Metadata_Library_Object,//44231C94-9498-49D1-A141-1D134E457054
    ASF_Index_Parameters_Object,//D6E229DF-35DA-11D1-9034-00A0C90349BE
    ASF_Media_Object_Index_Parameters_Object,//6B203BAD-3F11-48E4-ACA8-D7613DE2CFA7
    ASF_Timecode_Index_Parameters_Object,//F55E496D-9797-4B5D-8C8B-604DFE9BFB24
    ASF_Compatibility_Object,//26F18B5D-4584-47EC-9F5F-0E651F0452C9
    ASF_Advanced_Content_Encryption_Object,//43058533-6981-49E6-9B74-AD12CB86D58C ,

    ///
    ASF_Reserved_1,//ABD3D211-A9BA-11cf-8EE6-00C00C205365
    ASF_NONE
};


QString GUIDS[36]={
    ///
    "75B22630-668E-11CF-A6D9-00AA0062CE6C",
    "75B22636-668E-11CF-A6D9-00AA0062CE6C",
    "33000890-E5B1-11CF-89F4-00A0C90349CB",
    "D6E229D3-35DA-11D1-9034-00A0C90349BE",
    "FEB103F8-12AD-4C64-840F-2A1D2F7AD48C",
    "3CB73FD0-0C4A-4803-953D-EDF7B6228F0C",
    ///
    "8CABDCA1-A947-11CF-8EE4-00C00C205365",
    "B7DC0791-A9B7-11CF-8EE6-00C00C205365",
    "5FBF03B5-A92E-11CF-8EE3-00C00C205365",
    "86D15240-311D-11D0-A3A4-00A0C90348F6",
    "1EFB1A30-0B62-11D0-A39B-00A0C90348F6",
    "F487CD01-A951-11CF-8EE6-00C00C205365",
    "D6E229DC-35DA-11D1-9034-00A0C90349BE",
    "75B22635-668E-11CF-A6D9-00AA0062CE6C",
    "75B22633-668E-11CF-A6D9-00AA0062CE6C",
    "D2D0A440-E307-11D2-97F0-00A0C95EA850",
    "2211B3FA-BD23-11D2-B4B7-00A0C955FC6E",
    "7BF875CE-468D-11D1-8D82-006097C9A2B2",
    "2211B3FB-BD23-11D2-B4B7-00A0C955FC6E",
    "298AE614-2622-4C17-B935-DAE07EE9289C",
    "2211B3FC-BD23-11D2-B4B7-00A0C955FC6E",
    "1806D474-CADF-4509-A4BA-9AABCB96AAE8",
    ///
    "14E6A5CB-C672-4332-8399-A96952065B5A",
    "A08649CF-4775-4670-8A16-6E35357566CD",
    "D1465A40-5A79-4338-B71B-E36B8FD6C249",
    "D4FED15B-88D3-454F-81F0-ED5C45999E24",
    "A69609E6-517B-11D2-B6AF-00C04FD908E9",
    "7C4346A9-EFE0-4BFC-B229-393EDE415C85",
    "C5F8CBEA-5BAF-4877-8467-AA8C44FA4CCA",
    "44231C94-9498-49D1-A141-1D134E457054",
    "D6E229DF-35DA-11D1-9034-00A0C90349BE",
    "6B203BAD-3F11-48E4-ACA8-D7613DE2CFA7",
    "F55E496D-9797-4B5D-8C8B-604DFE9BFB24",
    "26F18B5D-4584-47EC-9F5F-0E651F0452C9",
    "43058533-6981-49E6-9B74-AD12CB86D58C",
    ///
    "ABD3D211-A9BA-11cf-8EE6-00C00C205365"
};






WmaTag::WmaTag(QObject *parent) : QObject(parent)
{

}




WmaTag::~WmaTag()
{

}

bool WmaTag::compareFlag(uchar *a,uchar *b)
{
    bool ret=true;
    for(int i=0;i<16;i++)
    {
        if(a[i] != b[i])
        {
            ret=false;
            break;
        }
    }
    return ret;

}

GUIDType getGUIDType(uchar *flag)
{

    GUIDType type=ASF_NONE;

    QString str;
    str.sprintf("%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
                (uchar)flag[3],(uchar)flag[2],(uchar)flag[1],(uchar)flag[0],
            (uchar)flag[5],(uchar)flag[4],
            (uchar)flag[7],(uchar)flag[6],
            (uchar)flag[8],(uchar)flag[9],
            (uchar)flag[10],(uchar)flag[11],(uchar)flag[12],(uchar)flag[13],(uchar)flag[14],(uchar)flag[15]
            );

    //qDebug()<<str;
    for(int i=0;i<36;i++)
    {
        if(GUIDS[i] == str)
        {
            type=(GUIDType)i;
            break;
        }

    }
    return type;
}



void printGUID(uchar *flag)
{
    int i=0;
    for(;i<4;i++)
    {
        printf("%02X", (uchar)flag[3-i]);
    }

    printf("-");
    for(;i<6;i++)
    {
        if(i==4)
            printf("%02X", (uchar)flag[5]);
        else
            printf("%02X", (uchar)flag[4]);
    }

    printf("-");

    for(;i<8;i++)
    {
        if(i==6)
            printf("%02X", (uchar)flag[7]);
        else
            printf("%02X", (uchar)flag[6]);
    }

    printf("-");
    for(;i<10;i++)
    {
        printf("%02X", (uchar)flag[i]);
    }

    printf("-");

    for(;i<16;i++)
    {
        printf("%02X", (uchar)flag[i]);
    }
    printf("\n");

}

void parseContentDescription(QByteArray array)
{
    StdFrameSize sizes;
    QString title="";
    QString artist="";
    QString copyright="";
    QString comment="";
    QString unknown="";
    sizes=*(StdFrameSize*)array.data();
    //qDebug()<<getUINT16(sizes.Artist);

    QTextCodec *codec=QTextCodec::codecForName("UTF-16");
    uint index=sizeof(StdFrameSize);
    uint size=getUINT16(sizes.Title);
    qDebug("TitleSize:%d",size);
    if(size != 0)
        title=codec->toUnicode(array.mid(index,size));
    index+=size;
    size=getUINT16(sizes.Artist);
    if(size != 0)
        artist=codec->toUnicode(array.mid(index,size));
    size=getUINT16(sizes.Comment);
    if(size != 0)
        comment=codec->toUnicode(array.mid(index,size));
    size=getUINT16(sizes.Copyight);
    if(size != 0)
        copyright=codec->toUnicode(array.mid(index,size));
    size=getUINT16(sizes.Unknown);
    if(size != 0)
        unknown=codec->toUnicode(array.mid(index,size));

    qDebug("Title:%s,Artist:%s,Comment:%s,Copyight:%s",
           title.toUtf8().data(),
           artist.toUtf8().data(),
           comment.toUtf8().data(),
           copyright.toUtf8().data()
           );
}

void parseExtendedContentDescription(QByteArray array)
{

    char *data=array.data();
    uint count=getUINT16(data);

    qDebug()<<count;
    uint index=2;
    QTextCodec *codec=QTextCodec::codecForName("UTF-16");

    for(uint i=0;i<count;i++)
    {

        uint size=getUINT16(data+index);
        index+=2;

        QString key=codec->toUnicode(array.mid(index,size));
        qDebug()<<key;

        index+=size;

        //char FLAG[2];
        //file.read(FLAG,2);
        quint16 valueType=getUINT16(data+index);
        DescriptorValueDataType type=(DescriptorValueDataType)valueType;
        qDebug()<<type;
        index+=2;

        size=getUINT16(data+index);
        index+=2;

        QString value=codec->toUnicode(array.mid(index,size));
        qDebug()<<value;
        index+=size;
        //qDebug("%x %x",FLAG[0],FLAG[1]);

    }
}


int findUnicodeEnd(QByteArray array,uint start)
{
    int index=start;
    bool find=false;
    while(true)
    {
        if(index<array.length() && array.at(index)==0 && array.at(index+1)==0)
        {
            if((index-start)%2 == 0)
            {
                find=true;
                break;
            }
        }
        index++;
    }

    if(find) return index;
    else return -1;
}



typedef struct DescriptionRecordHeader{
    quint16 LanguageListIndex;//16
    quint16 StreamNumber;//16
    quint16 NameLength;//16
    quint16 DataType;//16
    quint32 DataLength;//32
    // QString Name;//varies
    // QByteArray Data;//varies
}DescriptionRecordHeader;


void parseHeaderExtension(QByteArray array)
{
    QTextCodec *codec=QTextCodec::codecForName("UTF-16");
    char *data=array.data();
    printf("-----\n");
    printGUID((uchar *)data);

    //quint16 ReservedField2=getUINT16(data+16);

    quint32 DataSize=getUINT32(data+18);
    //qDebug()<<DataSize;
    //printGUID((uchar *)(data+22));
    uint index=22;

    GUIDType type = getGUIDType((uchar *)(data+index));
    qDebug("Header Extension DataSize:%d",DataSize);



    quint64 objSize;
    quint16 objCount;

    switch (type) {
    case ASF_Metadata_Library_Object:
        qDebug()<<"ASF_Metadata_Library_Object";
        index+=16;
        objSize=getUINT64(data+index);
        index+=8;
        objCount=getUINT16(data+index);

        qDebug("Metadata Library objSize:%d",objSize);
        index+=2;
        for(int i=0;i<objCount;i++)
        {

            DescriptionRecordHeader record;
            record=*(DescriptionRecordHeader*)(data+index);
            //qDebug()<<record.NameLength;
            //qDebug()<<record.DataLength;
            index+=12;
            ///qDebug()<<array.at(index);
            QString name=codec->toUnicode(array.mid(index,record.NameLength));//::fromUtf8(data,record.NameLength);
            index+=record.NameLength;
            DescriptorValueDataType type=(DescriptorValueDataType)record.DataType;
            qDebug("Record:%d dataSize:%d",i,record.DataLength);

            qDebug()<<name;
            //qDebug()<<record.DataLength;
            //qDebug()<<"---";
            //qDebug()<<index;
            if(type== UnicodeString)
            {
                QString dataa=codec->toUnicode(array.mid(index,record.DataLength));
                qDebug()<<dataa;
            }
            else if(type==ByteArray)
            {
                uint strSize=index;
                //QByteArray arr=array.mid(index,record.DataLength);
                //qDebug()<<"ByteArray:";
                //qDebug()<<*(uchar*)(data+index);
                if(name.startsWith("WM/Picture\0\0"))
                {
                    qDebug()<<"hghaha";
                    //quint32 size=getUINT32(data+index);
                    index+=5;
                    //qDebug()<<(uchar)array.at(index);
                    int end=findUnicodeEnd(array,index);
                    QString mime=codec->toUnicode(array.mid(index,end-index));
                    qDebug()<<mime;
                    index=end+2;

                    end=findUnicodeEnd(array,index);
                    QString des=codec->toUnicode(array.mid(index,end-index));
                    qDebug()<<des;
                    index=end+2;

                    qDebug()<<(uchar)array.at(index);


                    strSize=index-strSize;
                    qDebug("strSize:%d",strSize);

                    QByteArray pic=array.mid(index,record.DataLength-strSize);
                    QImage img=QImage::fromData(pic);
                    qDebug()<<img;

                }
            }
            index+=record.DataLength;


        }
        break;
    default:
        break;
    }

}


void WmaTag::get(QString path)
{
    WMAHeader header;

    QFile file(path);
    if(!file.exists())
    {
        return;
    }


    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }


    file.read((char *)&header,sizeof(WMAHeader));



    /* for(int i=0;i<6;i++)
    {
        printf("%X ", (uchar)header.Unknown[i]);
    }
    */

    qint64 size=getUINT64(header.Size);
    //printf("\nsize:%d\n",header.Size);

    //qint64 frameDataSize = header.Size - sizeof(WMAHeader);

    for(int k=0;k<10;k++)
    {

        qDebug()<<file.pos();
        FrameHeader frameheader;
        file.read((char *)&frameheader,sizeof(FrameHeader));

        printGUID(frameheader.Flag);

        /*for(int i=0;i<16;i++)
        {
            printf("%X ", (uchar)frameheader.Flag[i]);
        }
        printf("----\n\n");*/

        GUIDType type= getGUIDType(frameheader.Flag);

        size=getUINT64(frameheader.Size);

        switch (type) {
        case ASF_Content_Description_Object:
            qDebug()<<QStringLiteral("内容描述帧");

            parseContentDescription( file.read(size-sizeof(FrameHeader)));


            break;


        case ASF_Extended_Content_Description_Object:
            qDebug()<<QStringLiteral("扩展内容描述帧");
            parseExtendedContentDescription(file.read(size-sizeof(FrameHeader)));

            break;

        case ASF_Header_Extension_Object:
            qDebug()<<QStringLiteral("扩展头部帧");
            parseHeaderExtension(file.read(size-sizeof(FrameHeader)));
            break;
        default:
            file.seek(file.pos()+size-sizeof(FrameHeader));
            break;
        }






    }
}
