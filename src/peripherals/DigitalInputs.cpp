#include "peripherals/DigitalInputs.h"

#include "hardware/gpio.h"
#include "pico/time.h"

namespace Doncon::Peripherals {

void DigitalInputs::Button::setState(bool state, uint8_t debounce_delay) {
    if (active == state) {
        return;
    }

    const uint32_t now = to_ms_since_boot(get_absolute_time());
    if (last_change + debounce_delay <= now) {
        active = state;
        last_change = now;
    }
}

DigitalInputs::DigitalInputs(const Config &config) : m_config(config) {
    m_gp0.pin = config.gp0_pin;
    m_gp1.pin = config.gp1_pin;
    m_gp2.pin = config.gp2_pin;

    for (uint8_t pin : {config.gp0_pin, config.gp1_pin, config.gp2_pin}) {
        gpio_init(pin);
        gpio_set_dir(pin, GPIO_IN);
        gpio_pull_up(pin);
    }
}

void DigitalInputs::updateInputState(Utils::InputState &input_state) {
    const uint32_t gpio_state = ~gpio_get_all();

    m_gp0.setState((gpio_state & (1u << m_gp0.pin)) != 0, m_config.debounce_delay_ms);
    m_gp1.setState((gpio_state & (1u << m_gp1.pin)) != 0, m_config.debounce_delay_ms);
    m_gp2.setState((gpio_state & (1u << m_gp2.pin)) != 0, m_config.debounce_delay_ms);

    input_state.digital_inputs.gp0 = m_gp0.active;
    input_state.digital_inputs.gp1 = m_gp1.active;
    input_state.digital_inputs.gp2 = m_gp2.active;
}

} // namespace Doncon::Peripherals
