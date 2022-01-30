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

mcp48x2 dac;

int main() {
    // Use for debugging
    stdio_init_all();

    // Test DAC
    dac.init(DAC_SPI_PORT, PIN_DAC_CS, PIN_DAC_SCK, PIN_DAC_MOSI);

    while (true) {
        dac.write(2000); // 2V on the output of dacA
        sleep_ms(2000);
        dac.write(0); // 0V on the output of dacA
        sleep_ms(2000);
    }

    return 0;
}
