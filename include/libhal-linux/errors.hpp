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

#include <libhal/error.hpp>

namespace hal::gnu_linux {
struct errno_exception : public hal::exception
{
  constexpr errno_exception(int p_errno, std::errc p_errc, void* p_instance)
    : exception(p_errc, p_instance)
    , saved_errno(p_errno)
  {
  }

  void print_errno();

  int saved_errno;
};

struct invalid_character_device : public errno_exception
{
  constexpr invalid_character_device(std::string p_file_name,
                                     int p_errno,
                                     void* p_instance)
    : errno_exception(p_errno, std::errc::no_such_device, p_instance)
    , invalid_device_path(std::move(p_file_name))
  {
  }
  std::string invalid_device_path;
};

}  // namespace hal::gnu_linux
