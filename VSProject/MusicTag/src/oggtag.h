#ifndef OGGTAG_H
#define OGGTAG_H

#include <QObject>

class OggTag : public QObject
{
    Q_OBJECT
public:
    explicit OggTag(QObject *parent = 0);
    ~OggTag();
    void get(QString path);
signals:

public slots:
};

#endif // OGGTAG_H
