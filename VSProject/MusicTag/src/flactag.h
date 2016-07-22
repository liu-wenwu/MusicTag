#ifndef FLACTAG_H
#define FLACTAG_H

#include <QObject>

class FlacTag : public QObject
{
    Q_OBJECT
public:
    explicit FlacTag(QObject *parent = 0);
    ~FlacTag();
    void get(QString path);
signals:

public slots:
};

#endif // FLACTAG_H
