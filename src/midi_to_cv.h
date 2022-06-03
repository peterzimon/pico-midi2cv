#ifndef _MIDI_TO_CV_H
#define _MIDI_TO_CV_H

/**
 * Converts MIDI note to CV and gate
 * 
 * Pitch CV
 * --------
 * Pitch CV is generated through a DAC (MCP4822) which can output 0-4095mV. To
 * cover the 10V range the DAC output value is amplified with an op-amp (TL07x).
 * 
 * Pitch bend value can be between 0 and 0x3fff with 0x2000 meaning no bend. 
 * Pitch bend CV calculation:
 * 1. shift bend value to -0x2000 and 0x2000
 * 2. get max bend CV value (2 semitones)
 * 3. calculate actual bend CV value with the bend vs. max bend ratio
 * 
 * Gate
 * ----
 * Gate is output on a standard GPIO pin on the PICO (can be set in settings.h)
 * which is amplified with a standard op-amp (e.g. TL07x) or a level shifter 
 * (e.g. CD4504).
 */

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

    static uint16_t get_note_cv(uint8_t note);
    static int16_t get_pitch_bend_cv(uint16_t bend);
    static void set_gate(bool on);

private:
    bool m_gate;
    int m_last_cv;
    uint16_t m_cv;
    int16_t m_pitch_bend_cv;

    MCP48X2 *m_dac;
    UI *m_ui;
    MidiHandler *m_midi_handler;
};

#endif