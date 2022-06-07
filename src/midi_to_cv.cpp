#include "midi_to_cv.h"
#include "settings.h"

void MidiToCV::init() {
    m_cv = 0;
    m_gate = false;
    m_last_cv = 0;

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

void MidiToCV::attach(MidiHandler *midi_handler) {
    m_midi_handler = midi_handler;
}

/**
 * Main process. Put this in the main program loop for continuous midi to CV
 * conversion.
 */
void MidiToCV::process() {
    
    // Read MIDI messages. This builds a note stack in m_midi.note_stack
    m_midi_handler->read();

    if (m_midi_handler->channel() != settings.midi_channel) {
        return;
    }

    // If there's a note playing
    if (m_midi_handler->note_playing) {
        m_cv = get_note_cv(m_midi_handler->note());
        m_gate = true;
    } else {
        m_gate = false;
    }

    // Get pitch bend
    if (m_midi_handler->pitch_bend_dirty) {
        m_pitch_bend_cv = get_pitch_bend_cv(m_midi_handler->bend);
    } else {
        m_pitch_bend_cv = get_pitch_bend_cv(m_ui->hw_pb_to_midi_pb());
    }

    // Set CV voltage
    int cv = m_cv + m_pitch_bend_cv;
    cv = cv < 0 ? 0 : cv;
    
    if (cv != m_last_cv) {
        m_dac->write(cv);
        m_last_cv = cv;
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

/**
 * Pitch bend value can be between 0 and 0x3fff with 0x2000 meaning no bend. 
 * Pitch bend CV calculation:
 * 1. Shift bend value to -0x2000 and 0x2000
 * 2. Get max bend CV value (2 semitones)
 * 3. Calculate actual bend CV value with the bend vs. max bend ratio
*/
int16_t MidiToCV::get_pitch_bend_cv(uint16_t bend) {
    int16_t shiftedBend = bend - PITCH_BEND_CENTER;
    uint8_t maxBendCV = (uint8_t) (MAX_NOTE_VOLTAGE / (OCTAVES * 12) * MAX_PB_SEMINOTES);
    return shiftedBend * maxBendCV / PITCH_BEND_CENTER; // BEND / MAX_BEND_VALUE[8192]] = BENDCV / MAX_BEND_CV
}

void MidiToCV::set_gate(bool on) {
    gpio_put(GP_GATE, on);
}