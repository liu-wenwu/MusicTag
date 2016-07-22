#ifndef WAVTAG_H
#define WAVTAG_H

#include <QObject>
#include <QMap>

class WavTag : public QObject
{
    Q_OBJECT
public:
    explicit WavTag(QObject *parent = 0);
    ~WavTag();
    void get(QString path);
signals:

public slots:
private:
    QMap<QString,QString> list;
};

#endif // WAVTAG_H
