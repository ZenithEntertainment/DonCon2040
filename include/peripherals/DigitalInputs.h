#ifndef PERIPHERALS_DIGITALINPUTS_H_
#define PERIPHERALS_DIGITALINPUTS_H_

#include "utils/InputState.h"

#include <array>
#include <cstdint>

namespace Doncon::Peripherals {

class DigitalInputs {
  public:
    struct Config {
        uint8_t gp0_pin;
        uint8_t gp1_pin;
        uint8_t gp2_pin;
        uint8_t debounce_delay_ms;
    };

  private:
    struct Button {
        uint8_t pin;
        uint32_t last_change{0};
        bool active{false};

        void setState(bool state, uint8_t debounce_delay);
    };

    Config m_config;
    Button m_gp0, m_gp1, m_gp2;

  public:
    DigitalInputs(const Config &config);

    void updateInputState(Utils::InputState &input_state);
};

} // namespace Doncon::Peripherals

#endif // PERIPHERALS_DIGITALINPUTS_H_
