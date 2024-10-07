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

#include <unistd.h>

#include <iostream>

#include <libhal-linux/input_pin.hpp>
#include <libhal-linux/output_pin.hpp>
#include <libhal/error.hpp>

void application()
{
  auto output_gpio = hal::gnu_linux::output_pin("/dev/gpiochip0", 2);
  auto input_gpio = hal::gnu_linux::input_pin("/dev/gpiochip0", 3);

  std::cout << "blinking gpio 2 on gpiochip0\n";

  bool state = output_gpio.level();
  bool saved_state = false;

  while (true) {
    output_gpio.level(state);
    saved_state = output_gpio.level();
    std::cout << "current state: " << saved_state << "\n";
    sleep(1);
    state ^= 1;
    if (not input_gpio.level()) {
      std::cout << "quitting, bye bye\n";
      break;
    }
  }
}
