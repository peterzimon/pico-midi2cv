#ifndef _MIDI_HANDLER_H
#define _MIDI_HANDLER_H

#include "hardware/uart.h"
#include <midi_parser.h>

class MidiHandler: public MidiParser {
public:
    void init(void);
    void read_midi();
    void note_on(uint8_t channel, uint8_t note, uint8_t velocity);
    void note_off(uint8_t channel, uint8_t note, uint8_t velocity);
    void pitch_bend(uint8_t channel, uint16_t bend);

private:
    
};

#endif