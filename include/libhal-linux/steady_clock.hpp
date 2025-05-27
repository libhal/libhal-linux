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

#include <cstdint>

#include <chrono>
#include <ratio>
#include <type_traits>

#include <libhal/steady_clock.hpp>
#include <libhal/units.hpp>

namespace hal::gnu_linux {
template<typename T>
concept Clock = requires {
  { T::now() } -> std::convertible_to<std::chrono::time_point<T>>;
  std::is_integral_v<typename T::rep>;
  std::is_same_v<typename T::period,
                 std::ratio<T::period::num, T::period::den>>;
  std::is_same_v<typename T::duration,
                 std::chrono::duration<typename T::rep, typename T::period>>;
  std::is_same_v<typename T::time_point, std::chrono::time_point<T>>;
  { T::is_steady } -> std::convertible_to<const bool>;
};

template<Clock C>
class steady_clock : public hal::steady_clock
{
public:
  steady_clock() = default;

private:
  std::uint64_t driver_uptime() override
  {
    return static_cast<std::uint64_t>(C::now().time_since_epoch().count());
  }
  hertz driver_frequency() override
  {
    using period = C::period;
    double freq = period::den / period::num;
    return static_cast<hertz>(freq);
  }
};

}  // namespace hal::gnu_linux
