#ifndef TWORLD_TWTEXTCODER_H
#define TWORLD_TWTEXTCODER_H

#include <QString>
#include <QByteArray>

class TWTextCoder {
public:
    static QString decode(const QByteArray& arr);
    static QByteArray encode(const QString& str);
};

#endif //TWORLD_TWTEXTCODER_H
