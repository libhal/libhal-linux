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
#pragma once

#include <span>

#include <libhal/serial.hpp>

namespace hal::gnu_linux {
class serial : public hal::serial
{
public:
  serial(const std::string& p_file_path, settings p_settings = {});
  virtual ~serial();

private:
  void driver_configure(const settings& p_settings) override;
  write_t driver_write(std::span<const hal::byte> p_data) override;
  read_t driver_read(std::span<hal::byte> p_data) override;
  void driver_flush() override;

  int m_fd = 0;
};
}  // namespace hal::gnu_linux
