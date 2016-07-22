#ifndef APEV2_H
#define APEV2_H

#include <QObject>
#include <QMap>

typedef struct APEV2Header
{
    char ID[8];
    char Version[4];
    uchar Size[4];
    uchar Count[4];
    uchar Flags[4];
    uchar Reserved[8];

}APEV2Header,APEV2Footer;


enum APEV2FrameType
{
    Text,
    Binary,
    Locator,
    Reserved
};

typedef struct APEV2Frame
{
    uint Size;
    bool ReadOnly;
    APEV2FrameType Type;

    QString Key;
    QByteArray Value;
}APEV2Frame;

class APEV2 : public QObject
{
    Q_OBJECT
public:
    explicit APEV2(QObject *parent = 0);
    ~APEV2();
    void get(QString path);
signals:

public slots:

private:
    QMap<QString,QByteArray> frames;
};

#endif // APEV2_H
