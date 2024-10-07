#pragma once

#include <linux/gpio.h>

#include <libhal/error.hpp>
#include <libhal/input_pin.hpp>

namespace {
typedef struct gpio_v2_line_request gpio_line_request;
typedef struct gpio_v2_line_values gpio_values;
}  // namespace

namespace hal::gnu_linux {

/**
 * @brief Input pin for the linux kernel. Wraps libgpiod 2.1 at the earlist.
 * Assumes a GPIO driver exists and is properly written for the specific
 * hardware to interface with the linux kernel.
 */
class input_pin : public hal::input_pin
{
public:
  /**
   * @brief Constructor. Takes a *full path* to the GPIO character device and a
   * numeber that is known to said device.
   * @param p_chip_name Full path to GPIO character device.
   * @param p_pin Pin number for said device
   *
   * @throws std::invalid_argument if an invalid chip path was given, an invalid
   * pin number was given, or if a request to said line failed.
   */
  input_pin(const std::string& p_chip_name, const std::uint16_t p_pin);

  virtual ~input_pin();

private:
  bool driver_level() override;
  void driver_configure(const settings& p_settings) override;

  int m_chip_fd = -1;
  gpio_line_request m_line_request;
  gpio_values m_values;
};
}  // namespace hal::gnu_linux
