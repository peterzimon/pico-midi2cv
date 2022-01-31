/*
 * pico-midi2cv for Raspberry Pi Pico
 *
 * @version     1.0.0
 * @author      Zimo
 * @copyright   2022
 * @licence     MIT
 *
 */

/*
 * C++ headers
 */
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdint>
#include <cstring>

/*
 * Pico headers
 */
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "hardware/adc.h"
#include "hardware/uart.h"

/* 
 * Custom libraries
 */
#include <mcp48x2.h>
#include <midi_parser.h>
#include <ringbuffer.h>

/*
 * Project headers
 */
#include "settings.h"
#include "midi_handler.h"

mcp48x2 dac;
midi_handler midi;

int main() {
    // Use for debugging
    stdio_init_all();

    // Init DAC
    dac.init(DAC_SPI_PORT, PIN_DAC_CS, PIN_DAC_SCK, PIN_DAC_MOSI);

    while (1) {
        
    }

    return 0;
}
