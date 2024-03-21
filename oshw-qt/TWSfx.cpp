#include "TWSfx.h"
#include "defs.h"
#include "err.h"
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
#include <QSoundEffect>

class LoopyBuffer: public QIODevice {
public:
    LoopyBuffer(QByteArray* byteArray, QObject *parent = nullptr);
    bool seek(qint64 pos);
    bool loopyEnabled;
    bool paused;
protected:
    qint64 readData(char* data, qint64 len);
    qint64 writeData(const char* data, qint64 len);
private:
    QByteArray byteArr;
    qint64 offset;
    qsizetype len;
    char* bytePtr;
    void updatePos();
};

LoopyBuffer::LoopyBuffer(QByteArray* byteArray, QObject *parent): QIODevice(parent), loopyEnabled(false),paused(false) {
    byteArr = QByteArray(*byteArray);
    bytePtr = byteArr.data();
    len = byteArr.length();
}

bool LoopyBuffer::seek(qint64 pos) {
    if (pos < 0) return false;
    if (!loopyEnabled && pos > len) return false;
    offset = pos % len;
    return true;
}

qint64 LoopyBuffer::readData(char *data, qint64 reqLen) {
    if (paused) return 0;
    qint64 bytesWritten = 0;
    qint64 bytesAvalThisLoop = len - offset;
    if (!loopyEnabled && reqLen > bytesAvalThisLoop) {
        reqLen = bytesAvalThisLoop;
    } else while (reqLen > bytesAvalThisLoop) {
        memcpy(data, bytePtr + offset, bytesAvalThisLoop);
        data += bytesAvalThisLoop;
        bytesWritten += bytesAvalThisLoop;
        reqLen -= bytesAvalThisLoop;
        offset = 0;
        bytesAvalThisLoop = len;
    }
    memcpy(data, bytePtr + offset, reqLen);
    offset += reqLen;
    bytesWritten += reqLen;
    return bytesWritten;
};

qint64 LoopyBuffer::writeData(const char* data, qint64 len) {
    return 0;
}

TWSfx::TWSfx(QString filename, bool repeating, QObject* parent): QObject(parent), repeating(repeating) {
    decodeBuf = new QBuffer(this);
    decodeBuf->open(QIODevice::ReadWrite);
    decoder = new QAudioDecoder(this);
    connect(decoder, &QAudioDecoder::bufferReady, this, &TWSfx::consumeConversionBuffer);
    connect(decoder, &QAudioDecoder::finished, this, &TWSfx::finishConvertingSound);
    connect(decoder, QOverload<QAudioDecoder::Error>::of(&QAudioDecoder::error), this, &TWSfx::handleConvertionError);

    decoder->setAudioFormat(TWSfx::defaultFormat());
    decoder->setSource(QUrl::fromLocalFile(filename));
    decoder->start();

    sink = new QAudioSink(QMediaDevices::defaultAudioOutput(), TWSfx::defaultFormat(), this);
    connect(sink, &QAudioSink::stateChanged, this, &TWSfx::handleStateChanged);
}

void TWSfx::handleConvertionError(QAudioDecoder::Error err) {
    QString errorMessage = decoder->errorString();
    warn("cannot initialize sfx: %s", errorMessage.toStdString().c_str());
}

void TWSfx::consumeConversionBuffer() {
    QAudioBuffer audioBuf = decoder->read();
    audioBuf.detach();
    QByteArray byteArray = QByteArray(audioBuf.constData<char>(), audioBuf.byteCount());
    decodeBuf->write(byteArray);
}

void TWSfx::finishConvertingSound() {
    decoder->deleteLater();
    decoder = nullptr;
    decodeBuf->seek(0);
    QByteArray* data = new QByteArray(decodeBuf->data());
    buf = new LoopyBuffer(data, this);
    buf->loopyEnabled = repeating;
    buf->open(QIODevice::ReadOnly);
}

void TWSfx::play() {
    if (sink->state() == QAudio::ActiveState && repeating) return;
    buf->seek(0);
    buf->paused = false;
    sink->start(buf);
}

void TWSfx::stop() {
    if (sink->state() != QAudio::ActiveState && sink->state() != QAudio::SuspendedState) return;
    sink->stop();
}

void TWSfx::forceStop() {
    if (sink->state() != QAudio::ActiveState && sink->state() != QAudio::SuspendedState) return;
    sink->stop();
}

void TWSfx::pause() {
    if (sink->state() != QAudio::ActiveState) return;
    buf->paused = true;
    sink->suspend();
}

void TWSfx::resume() {
    if (sink->state() != QAudio::SuspendedState) return;
    buf->paused = false;
    sink->resume();
}

void TWSfx::setVolume(qreal volume) {
    sink->setVolume(volume);
}

void TWSfx::handleStateChanged(QAudio::State state) {
    switch (state) {
    case QAudio::IdleState:
        if (repeating) {
         //   sink->start(buf);
        } else {
         //   sink->stop();
        }
        break;
    default:
        break;
    }
}

TWSfxManager::TWSfxManager(QObject* parent) :
    QObject(parent),
    enableAudio(false),
    sounds() {
    sounds.resize(SND_COUNT);
}

void TWSfxManager::EnableAudio(bool bEnabled) {
    enableAudio = bEnabled;
}

void TWSfxManager::LoadSoundEffect(int index, QString szFilename)
{
    sounds[index] = new TWSfx(szFilename, index >= SND_ONESHOT_COUNT, this);
}

TWSfxManager::~TWSfxManager() {
    for (int index = 0; index < SND_COUNT; index++) {
        if (!sounds[index]) continue;
        // Defer deletion in case the effect is still playing.
        sounds[index]->deleteLater();
        sounds[index] = nullptr;
    }
}

void TWSfxManager::SetAudioVolume(qreal fVolume) {
    for (TWSfx* pSoundEffect : sounds)
    {
        if (pSoundEffect)
            pSoundEffect->setVolume(fVolume);
    }
}

void TWSfxManager::SetSoundEffects(int sfx) {
    int i;
    unsigned long flag;
    for (i = 0, flag = 1u; i < SND_COUNT; ++i, flag <<= 1)
    {
        TWSfx* sound = sounds[i];
        if (!sound) continue;
        if (sfx & flag) {
            sound->play();
        } else if (i >= SND_ONESHOT_COUNT) {
            sound->stop();
        }
    }
}

void TWSfxManager::StopSoundEffects() {
    for (int i = 0; i < SND_COUNT;i++)
    {
        TWSfx* sound = sounds[i];
        if (!sound) continue;
        sound->forceStop();
    }
}

void TWSfxManager::PauseSoundEffects(bool pause) {
    for (TWSfx* sfx: sounds) {
        if (!sfx) continue;
        if (pause) {
            sfx->pause();
        } else {
            sfx->resume();
        }
    }
}
