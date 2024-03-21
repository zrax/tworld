#ifndef TWSFX_H
#define TWSFX_H

#include <QObject>
#include <QAudio>
#include <QAudioFormat>
#include <QAudioDecoder>
#include <QVector>

class QAudioSink;
class QBuffer;
class LoopyBuffer;

/* Some generic default settings for the audio output.
 */
#define DEFAULT_SND_FREQ	22050
#define DEFAULT_SND_CHAN	1
#define DEFAULT_SND_FORM	QAudioFormat::Int16

class TWSfx : public QObject
{
    Q_OBJECT
public:
    TWSfx(QString filename, bool repeating, QObject* parent=nullptr);
    void play();
    void stop();
    void forceStop();
    void pause();
    void resume();
    void setVolume(qreal volume);
public slots:
    void handleStateChanged(QAudio::State state);
    void consumeConversionBuffer();
    void finishConvertingSound();
    void handleConvertionError(QAudioDecoder::Error err);
private:
    QBuffer* decodeBuf;
    LoopyBuffer* buf;
    QAudioDecoder* decoder;
    QAudioSink* sink;
    bool repeating;
    static constexpr QAudioFormat defaultFormat() {
        QAudioFormat format = QAudioFormat();
        format.setSampleRate(DEFAULT_SND_FREQ);
        format.setChannelCount(DEFAULT_SND_CHAN);
        format.setSampleFormat(DEFAULT_SND_FORM);
        return format;
    }
};

class TWSfxManager: public QObject {
    Q_OBJECT
public:
    TWSfxManager(QObject* parent = nullptr);
    ~TWSfxManager();
private:
    bool enableAudio;
    QVector<TWSfx*> sounds;

public slots:
    void EnableAudio(bool bEnabled);
    void LoadSoundEffect(int index, QString szFilename);
    void SetSoundEffects(int sfx);
    void StopSoundEffects();
    void SetAudioVolume(qreal fVolume);
    void PauseSoundEffects(bool pause);

};

#endif // TWSFX_H
