#ifndef MP4TAG_H
#define MP4TAG_H

#include <QObject>

class Mp4Tag : public QObject
{
    Q_OBJECT
public:
    explicit Mp4Tag(QObject *parent = 0);
    ~Mp4Tag();
    void get(QString path);
signals:

public slots:
};

#endif // MP4TAG_H
