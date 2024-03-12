#include "TWSfx.h"
#include <QFile>
#include <QAudioSink>
#include <QAudioFormat>
#include <QObject>


TWSfx::TWSfx(const char* filename, bool repeating, QObject* parent): QObject(parent), repeating(repeating) {
    file = new QFile(filename, this);
    sink = new QAudioSink(TWSfx::defaultFormat(), this);
    QObject::connect(sink, &QAudioSink::stateChanged, this, &TWSfx::handleStateChanged);
}

void TWSfx::play() {
    if (sink->state() == QAudio::ActiveState) return;
    sink->start(file);
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
        if (repeating) {
            sink->reset();
        } else {
            sink->stop();
        }
        break;
    default:
        break;
    }
}
