#ifndef TWORLD_TWTEXTCODER_H
#define TWORLD_TWTEXTCODER_H

#include <QString>
#include <QByteArray>
#include <QStringDecoder>
#include <QStringEncoder>

class TWTextCoder {
private:
    QStringDecoder m_decoder;
    QStringEncoder m_encoder;
public:
    TWTextCoder();
    QString decode(const QByteArray& arr) { return m_decoder(arr); }
    QByteArray encode(const QString& str) { return m_encoder(str); };
};

static TWTextCoder s_textCoder = TWTextCoder();

#endif //TWORLD_TWTEXTCODER_H
