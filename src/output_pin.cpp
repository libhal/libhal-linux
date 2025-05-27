// Copyright 2024 - 2025 Khalil Estell and the libhal contributors
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

#include <libhal-linux/errors.hpp>
#include <libhal-linux/output_pin.hpp>
#include <libhal/error.hpp>

namespace hal::gnu_linux {

output_pin::output_pin(const std::string& p_chip_name,
                       const std::uint16_t p_pin)
{
  m_chip_fd = open(p_chip_name.c_str(), O_RDONLY);
  if (m_chip_fd < 0) {
    throw invalid_character_device(p_chip_name, errno, this);
  }
  memset(&m_line_request, 0, sizeof(gpio_line_request));
  memset(&m_values, 0, sizeof(gpio_values));
  m_line_request.offsets[0] = p_pin;
  m_line_request.num_lines = 1;
  m_line_request.config.flags = GPIO_V2_LINE_FLAG_OUTPUT;
  if (ioctl(m_chip_fd, GPIO_V2_GET_LINE_IOCTL, &m_line_request) < 0) {
    throw errno_exception(errno, std::errc::connection_refused, this);
  }
  m_values.mask = 1;  // only use a single channel
}

output_pin::~output_pin()
{
  close(m_line_request.fd);
  close(m_chip_fd);
}

void output_pin::driver_configure(const settings& p_settings)
{
  // Resistor settings
  switch (p_settings.resistor) {
    default:
    case hal::pin_resistor::none:
      break;
    case hal::pin_resistor::pull_up:
      m_line_request.config.flags |= GPIO_V2_LINE_FLAG_BIAS_PULL_UP;
      break;
    case hal::pin_resistor::pull_down:
      m_line_request.config.flags |= GPIO_V2_LINE_FLAG_BIAS_PULL_DOWN;
      break;
  }

  // Open Drain
  if (p_settings.open_drain) {
    m_line_request.config.flags |= GPIO_V2_LINE_FLAG_OPEN_DRAIN;
  }

  if (ioctl(m_line_request.fd,
            GPIO_V2_LINE_SET_CONFIG_IOCTL,
            &m_line_request.config) < 0) {
    perror("Failed to configured");
    throw hal::operation_not_supported(this);
  }
}

void output_pin::driver_level(bool p_high)
{
  m_values.bits = p_high & m_values.mask;
  if (ioctl(m_line_request.fd, GPIO_V2_LINE_SET_VALUES_IOCTL, &m_values) < 0) {
    perror("driver_level write");
    throw hal::io_error(this);
  }
}

bool output_pin::driver_level()
{
  if (ioctl(m_line_request.fd, GPIO_V2_LINE_GET_VALUES_IOCTL, &m_values) < 0) {
    perror("driver_level read");
    throw hal::io_error(this);
  }
  return static_cast<bool>(m_values.bits & m_values.mask);
}
}  // namespace hal::gnu_linux
