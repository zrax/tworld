#include "TWTextCoder.h"

#include <iostream>

TWTextCoder::TWTextCoder() {
    m_decoder = QStringDecoder{"windows-1252"};
    if (m_decoder.isValid()) {
        m_encoder = QStringEncoder{"windows-1252"};
    } else {
        std::cerr << "Unable to construct a windows-1252 encoder and decoder, falling back to Latin-1, some characters may not display correctly" << std::endl;
        m_decoder = QStringDecoder(QStringDecoder::Latin1);
        m_encoder = QStringEncoder(QStringEncoder::Latin1);
        if (!m_decoder.isValid() || !m_encoder.isValid()) {
            throw std::runtime_error{"Unable to construct a latin-1 encoder and decoder, this shouldn't be possible"};
        }
    }
}
