#include <midi_parser.h>

class midi_handler: public midi_parser {
public:
    void init(void);
    void note_off(uint8_t channel, uint8_t note, uint8_t velocity);
    void note_on(uint8_t channel, uint8_t note, uint8_t velocity);
    void pitch_bend(uint8_t channel, uint16_t bend);

    // void attach(DAC *dac);
    // void attach(Gate *gate);

private:
    /* data */
};