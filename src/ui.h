#ifndef _UI_H
#define _UI_H

#include <stdio.h>
#include <math.h>
#include <pico/stdio.h>
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "settings.h"

class UI {
public:
    void init();
    void update();
    uint16_t hw_pb_to_midi_pb();

private:
    uint16_t m_pitchbend_reference;
    uint16_t m_pitchbend;
    void m_update_pitchbend();
    uint16_t m_read_adc_samples(size_t sample_size);
};

#endif