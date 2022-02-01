#ifndef _MIDI_TO_CV_H
#define _MIDI_TO_CV_H

#include "settings.h"
#include <mcp48x2.h>
#include "ui.h"
#include "midi_handler.h"
#include "math.h"
#include <utils.h>

class MidiToCV {
public:
    void init();
    void attach(MCP48X2 *dac);
    void attach(UI *ui);
    void attach(MidiHandler *midi_handler);
    void process();
    static uint16_t get_cv_for_note(uint8_t note);

private:
    MCP48X2 *m_dac;
    UI *m_ui;
    MidiHandler *m_midi_handler;
};

#endif