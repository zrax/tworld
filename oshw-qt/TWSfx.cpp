#include "TWSfx.h"
#include <QFile>
#include <QAudioSink>
#include <QAudioFormat>
#include <QObject>
#include <QMediaDevices>
#include <QAudioDecoder>
#include <QUrl>
#include <QString>
#include <QByteArray>
#include <QBuffer>

TWSfx::TWSfx(const char* filename, bool repeating, QObject* parent): QObject(parent), repeating(repeating) {
    buf = new QBuffer();
    buf->open(QIODevice::ReadWrite);
    decoder = new QAudioDecoder(this);
    connect(decoder, &QAudioDecoder::bufferReady, this, &TWSfx::consumeConversionBuffer);
    connect(decoder, &QAudioDecoder::finished, this, &TWSfx::finishConvertingSound);
    connect(decoder, QOverload<QAudioDecoder::Error>::of(&QAudioDecoder::error), this, &TWSfx::handleConvertionError);

    decoder->setAudioFormat(TWSfx::defaultFormat());
    decoder->setSource(QUrl::fromLocalFile(QString::fromLocal8Bit(filename)));
    decoder->start();

    sink = new QAudioSink(QMediaDevices::defaultAudioOutput(), TWSfx::defaultFormat(), this);
    connect(sink, &QAudioSink::stateChanged, this, &TWSfx::handleStateChanged);
}

void TWSfx::handleConvertionError(QAudioDecoder::Error err) {
    auto a = decoder->errorString().toStdString();
    puts(a.c_str());
}

void TWSfx::consumeConversionBuffer() {
    QAudioBuffer audioBuf = decoder->read();
    audioBuf.detach();
    QByteArray byteArray = QByteArray(audioBuf.constData<char>(), audioBuf.byteCount());
    buf->write(byteArray);
}

void TWSfx::finishConvertingSound() {
    decoder->deleteLater();
    decoder = nullptr;
    buf->seek(0);
}

void TWSfx::play() {
    if (sink->state() == QAudio::ActiveState && repeating) return;
    sink->start(buf);
}

void TWSfx::stop() {
    if (sink->state() != QAudio::ActiveState) return;
    sink->stop();
}

void TWSfx::pause() {
    if (sink->state() != QAudio::ActiveState) return;
    sink->suspend();
}

void TWSfx::resume() {
    if (sink->state() != QAudio::SuspendedState) return
    sink->resume();
}

void TWSfx::setVolume(qreal volume) {
    sink->setVolume(volume);
}

void TWSfx::handleStateChanged(QAudio::State state) {
    switch (state) {
    case QAudio::IdleState:
        if (!repeating) {
            sink->stop();
        } else {
            buf->seek(0);
            sink->start(buf);
        }
        break;
    default:
        break;
    }
}
