#ifndef _MIDI_HANDLER_H
#define _MIDI_HANDLER_H

#include "settings.h"
#include "hardware/uart.h"
#include <midi_parser.h>
#include <ringbuffer.h>

#define MIDI_BUFFER_SIZE 32

class MidiHandler: public MidiParser {
public:
    void init(void);
    void read_midi();
    void note_on(uint8_t channel, uint8_t note, uint8_t velocity);
    void note_off(uint8_t channel, uint8_t note, uint8_t velocity);
    void pitch_bend(uint8_t channel, uint16_t bend);

private:
    uint8_t m_buffer_var[MIDI_BUFFER_SIZE];
    RingBuffer m_input_buffer;
    int m_gate;
    uint16_t m_cv;
};

#endif