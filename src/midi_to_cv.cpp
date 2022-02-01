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
}

uint16_t MidiToCV::cv_for_note(uint8_t note, int voice) {

    // Return calibrated CV value
    uint8_t tNote = note;
    uint8_t notes = OCTAVES * 12;
    if (tNote > LOWEST_MIDI_NOTE + notes) {
        tNote = LOWEST_MIDI_NOTE + notes;
    }
    uint8_t octave = floor(tNote / 12) + 1;
    uint16_t voltPerOctave = (uint16_t) (MAX_NOTE_VOLTAGE / OCTAVES);
    uint16_t rawCVlo = voltPerOctave * (octave - 1);
    uint16_t rawCVhi = voltPerOctave * octave;

    uint16_t rawCV = (uint16_t)Utils::map(tNote, LOWEST_MIDI_NOTE, (LOWEST_MIDI_NOTE + notes), 0, MAX_NOTE_VOLTAGE);
    uint16_t cv = (uint16_t)Utils::map(rawCV, rawCVlo, rawCVhi, (rawCVlo + settings.calibration[octave - 1]), (rawCVhi + settings.calibration[octave]));

    // return cv + _pitchBendCV;
    return cv;
}