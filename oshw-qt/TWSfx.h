#ifndef TWSFX_H
#define TWSFX_H

#include <QObject>
#include <QAudio>
#include <QAudioFormat>

class QFile;
class QAudioSink;

/* Some generic default settings for the audio output.
 */
#define DEFAULT_SND_FREQ	22050
#define DEFAULT_SND_CHAN	1
#define DEFAULT_SND_FORM	QAudioFormat::Int16

class TWSfx : public QObject
{
    Q_OBJECT
public:
    TWSfx(const char* filename, bool repeating, QObject* parent=nullptr);
    void play();
    void stop();
    void pause();
    void resume();
    void setVolume(qreal volume);
public slots:
    void handleStateChanged(QAudio::State state);
private:
    QFile* file;
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

#endif // TWSFX_H
