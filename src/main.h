/*
 * pico-midi2cv for Raspberry Pi Pico
 *
 * @version     1.0.0
 * @author      Zimo
 * @copyright   2022
 * @licence     MIT
 *
 */
#ifndef _SRC_MAIN_H
#define _SRC_MAIN_H

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

/*
 * Constants
 */
#define DAC_SPI_PORT spi0
#define PIN_DAC_CS 5
#define PIN_DAC_SCK 6
#define PIN_DAC_MOSI 7

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Usual header code here
 */

#ifdef __cplusplus
}
#endif

#endif // _PICO-MIDI2CV_MAIN_HEADER_
