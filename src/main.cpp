/*
 * pico-midi2cv for Raspberry Pi Pico
 *
 * @version     1.0.0
 * @author      Zimo
 * @copyright   2022
 * @licence     MIT
 *
 */
#include "main.h"
#include <mcp48x2.h>

MCP48X2 dac;

int main() {
    // Use for debugging
    stdio_init_all();

    // Test DAC
    dac.init(DAC_SPI_PORT, PIN_DAC_CS, PIN_DAC_SCK, PIN_DAC_MOSI);
    dac.write(2000); // 2V on the output of dacA (default)

    while (true) {
        sleep_ms(1000);
    }

    return 0;
}
