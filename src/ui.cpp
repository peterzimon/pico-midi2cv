#include "ui.h"

void UI::init() {
    // Read MIDI channel. Only do this once during bootup
    gpio_init(GP_CHANNEL_DIP_0); gpio_set_dir(GP_CHANNEL_DIP_0, GPIO_IN); gpio_pull_down(GP_CHANNEL_DIP_0);
    gpio_init(GP_CHANNEL_DIP_1); gpio_set_dir(GP_CHANNEL_DIP_1, GPIO_IN); gpio_pull_down(GP_CHANNEL_DIP_1);
    gpio_init(GP_CHANNEL_DIP_2); gpio_set_dir(GP_CHANNEL_DIP_2, GPIO_IN); gpio_pull_down(GP_CHANNEL_DIP_2);
    gpio_init(GP_CHANNEL_DIP_3); gpio_set_dir(GP_CHANNEL_DIP_3, GPIO_IN); gpio_pull_down(GP_CHANNEL_DIP_3);

    int midi_ch_0 = gpio_get(GP_CHANNEL_DIP_0);
    int midi_ch_1 = gpio_get(GP_CHANNEL_DIP_1);
    int midi_ch_2 = gpio_get(GP_CHANNEL_DIP_2);
    int midi_ch_3 = gpio_get(GP_CHANNEL_DIP_3);

    uint8_t midi_ch = midi_ch_0 | midi_ch_1 << 1 | midi_ch_2 << 2 | midi_ch_3;
    settings.midi_channel = midi_ch;

    printf("Midi channel: %d\r\n", midi_ch);

    // Pitchbend initialisation
    // Init ADC
    adc_init();
    adc_gpio_init(ADC_GP_PITCHBEND);

    // Select a GPIO port for input. 0 represents GPIO26, 1: GPIO27 etc. Subtract
    // 26 from the GPIO port number to get the mapping.
    adc_select_input(ADC_GP_PITCHBEND - 26);

    // Store initial value of pitchbend as a reference point
    m_pitchbend_reference = m_read_adc_samples(10);

    // printf("Pitchbend reference: %d", m_pitchbend_reference);
}

// Read all values on the UI
void UI::update() {
    m_update_pitchbend();
}

/**
 * Return the pitch bend value converted to MIDI value
 * 1. m_pitchbend_reference should be mapped to PITCH_BEND_CENTER (0x2000)
 * 2. m_pitchbend_reference +/- HW_PB_MAX_DIFF should be mapped to
 *    MAX_PITCH_BEND (0x3fff) and 0x0000 respectively
 * 3. Returned value should not be between 0 and 0x3fff
 */
uint16_t UI::hw_pb_to_midi_pb() {
    uint16_t midi_pitchbend = 0x0;
    uint16_t diff = 0;
    uint16_t ratio = 0;

    if (m_pitchbend > m_pitchbend_reference) {
        diff = m_pitchbend - m_pitchbend_reference;
        ratio = round(diff * 100 / HW_PB_MAX_DIFF);
        midi_pitchbend = uint16_t(PITCH_BEND_CENTER + (PITCH_BEND_CENTER * ratio / 100));
    } else if (m_pitchbend < m_pitchbend_reference) {
        diff = m_pitchbend_reference - m_pitchbend;
        ratio = round(diff * 100 / HW_PB_MAX_DIFF);
        midi_pitchbend = uint16_t(PITCH_BEND_CENTER - (PITCH_BEND_CENTER * ratio / 100));
    }

    return midi_pitchbend;
}

void UI::m_update_pitchbend() {
    adc_select_input(ADC_GP_PITCHBEND - 26);
    m_pitchbend = m_read_adc_samples(10);
}

uint16_t UI::m_read_adc_samples(size_t sample_size) {
    uint16_t value = 0;
    for (size_t i = 0; i < sample_size; i++) {
        value += adc_read();
    }
    return round(value / sample_size);
}