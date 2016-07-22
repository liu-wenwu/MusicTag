#ifndef WMATAG_H
#define WMATAG_H

#include <QObject>

class WmaTag : public QObject
{
    Q_OBJECT
public:
    explicit WmaTag(QObject *parent = 0);
    ~WmaTag();
    void get(QString path);
signals:

public slots:
private:
    bool compareFlag(uchar *,uchar *);
};

#endif // WMATAG_H
