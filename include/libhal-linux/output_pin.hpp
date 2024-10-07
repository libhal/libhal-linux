// Copyright 2024 Khalil Estell
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <linux/gpio.h>

#include <libhal/error.hpp>
#include <libhal/output_pin.hpp>
#include <libhal/units.hpp>

namespace hal::gnu_linux {
/**
 * @brief Output pin for the linux kernel. Wraps libgpiod 2.1 at the earlist.
 * Assumes a GPIO driver exists and is properly written for the specific
 * hardware to interface with the linux kernel.
 */
class output_pin : public hal::output_pin
{
public:
  /**
   * @brief Takes a *full path* to the GPIO character device and a number that
   * is known to said device.
   *
   * @param p_chip_name - Full path to GPIO character device.
   * @param p_pin - Pin number for said device
   *
   * @throws std::invalid_argument if an invalid chip path was given, an invalid
   * pin number was given, or if a request to said line failed.
   */
  output_pin(const std::string& p_chip_name, const std::uint16_t p_pin);

  virtual ~output_pin();

private:
  void driver_configure(const settings& p_settings) override;
  void driver_level(bool p_high) override;
  bool driver_level() override;

  int m_chip_fd = -1;
  gpio_v2_line_request m_line_request;
  gpio_v2_line_values m_values;
};
}  // namespace hal::gnu_linux
