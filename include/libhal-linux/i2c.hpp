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

#include <libhal/i2c.hpp>

namespace hal::gnu_linux {
class i2c : public hal::i2c
{
public:
  /**
   * @brief Constructs a new i2c linux device
   * @param p_file_path The absolute path the to i2c udev device.
   *
   * @throws hal::io_error if the device was not found or a file descriptor
   * could not be opened.
   */
  i2c(const std::string& p_file_path);

  virtual ~i2c();

private:
  void driver_configure(const settings& p_settings) override;
  void driver_transaction(
    hal::byte p_address,
    std::span<const hal::byte> p_data_out,
    std::span<hal::byte> p_data_in,
    hal::function_ref<hal::timeout_function> p_timeout) override;

  int m_fd = 0;
};
}  // namespace hal::gnu_linux
