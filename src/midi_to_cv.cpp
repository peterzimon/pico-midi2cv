#include "midi_to_cv.h"
#include "settings.h"

void MidiToCV::init() {
    m_cv = 0;
    m_gate = false;

    gpio_init(GP_GATE);
    gpio_set_dir(GP_GATE, GPIO_OUT);
    gpio_pull_down(GP_GATE);
    gpio_put(GP_GATE, 0);
}

void MidiToCV::attach(MCP48X2 *dac) {
    m_dac = dac;
}

void MidiToCV::attach(UI *ui) {
    m_ui = ui;
}

void MidiToCV::attach(MidiHandler *midi) {
    m_midi = midi;
}

/**
 * Main process. Put this in the main program loop for continuous midi to CV
 * conversion.
 */
void MidiToCV::process() {
    
    // Read MIDI messages. This builds a note stack in m_midi.note_stack
    m_midi->read();

    // If there's a note playing
    if (m_midi->note_stack[0] != -1) {
        m_cv = get_note_cv(m_midi->note_stack[0]);
        m_gate = true;
    } else {
        m_gate = false;
    }

    // Get pitch bend
    m_pitch_bend_cv = get_pitch_bend_cv(m_midi->bend);

    // Set CV voltage
    int cv = m_cv; // + m_pitch_bend_cv;
    if (cv >= 0) {
        m_dac->write((uint16_t)cv);
    } else {
        m_dac->write(0);
    }

    // Set gate
    set_gate(m_gate);
}

uint16_t MidiToCV::get_note_cv(uint8_t note) {

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

    return cv;
}

// @TODO: pitch to be calculated. There's something wrong ATM. it always returns 
// some negative value instead of 0.
int16_t MidiToCV::get_pitch_bend_cv(uint16_t bend) {
    int16_t shiftedBend = bend - PITCH_BEND_CENTER;
    uint8_t maxBendCV = (uint8_t) (MAX_NOTE_VOLTAGE / (OCTAVES * 12) * 2);
    return shiftedBend * maxBendCV / PITCH_BEND_CENTER; // BEND / MAX_BEND_VALUE[8192]] = BENDCV / MAX_BEND_CV
}

void MidiToCV::set_gate(bool on) {
    gpio_put(GP_GATE, on);
}