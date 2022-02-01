#include "midi_to_cv.h"

void MidiToCV::init() {
    
}

void MidiToCV::attach(MCP48X2 *dac) {
    m_dac = dac;
}

void MidiToCV::attach(UI *ui) {
    m_ui = ui;
}

void MidiToCV::attach(MidiHandler *midi_handler) {
    m_midi_handler = midi_handler;
}


/**
 * The plan
 * 1. midi_handler->read_midi
 * 2. get the note stack from midi_handler
 * 3. calculate CV for midi note on top of the stack
 * 3. dac->write CV
 * 
 */
void MidiToCV::process() {
    m_midi_handler->read_midi();
    // ...
}

uint16_t MidiToCV::get_cv_for_note(uint8_t note) {

    // Return calibrated CV value
    uint8_t t_note = note;
    uint8_t notes = OCTAVES * 12;
    if (t_note > LOWEST_MIDI_NOTE + notes) {
        t_note = LOWEST_MIDI_NOTE + notes;
    }
    uint8_t octave = floor(t_note / 12) + 1;
    uint16_t volt_per_octave = (uint16_t) (MAX_NOTE_VOLTAGE / OCTAVES);
    uint16_t raw_cv_lo = volt_per_octave * (octave - 1);
    uint16_t raw_cv_hi = volt_per_octave * octave;

    uint16_t raw_cv = (uint16_t)Utils::map(t_note, LOWEST_MIDI_NOTE, (LOWEST_MIDI_NOTE + notes), 0, MAX_NOTE_VOLTAGE);
    uint16_t cv = (uint16_t)Utils::map(raw_cv, raw_cv_lo, raw_cv_hi, (raw_cv_lo + settings.calibration[octave - 1]), (raw_cv_hi + settings.calibration[octave]));

    // return cv + _pitchBendCV;
    return cv;
}