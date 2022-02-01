#include "midi_handler.h"

void MidiHandler::init() {

    m_input_buffer.init(m_buffer_var, MIDI_BUFFER_SIZE);
    m_gate = 0;
    m_cv = 0;

    // Initialize + set GPIO pin for UART
    uart_init(MIDI_UART_INSTANCE, MIDI_BAUDRATE);
    gpio_set_function(GP_MIDI_RX, GPIO_FUNC_UART);
}

void MidiHandler::read_midi() {
    // Read MIDI input and write it to ringbuffer
    if (!uart_is_readable(MIDI_UART_INSTANCE)) {
        uint8_t data = uart_getc(MIDI_UART_INSTANCE);
        m_input_buffer.write_byte(data);
    }

    // Read the buffer and call appropriate MIDI messages
    while (!m_input_buffer.is_empty()) {
        uint8_t byte = 0;
        m_input_buffer.read_byte(byte);
        
        // Parent class call that eventually calls this instance's note_on,
        // note_off etc. functions
        this->parse_byte(byte);  
    }
}

void MidiHandler::note_on(uint8_t channel, uint8_t note, uint8_t velocity) {
    // Add to note stack
}

void MidiHandler::note_off(uint8_t channel, uint8_t note, uint8_t velocity) {
    // Remove from note stack
}

void MidiHandler::pitch_bend(uint8_t channel, uint16_t bend) {
    // Unused ATM
}