# Copyright 2024 - 2025 Khalil Estell and the libhal contributors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.15)

project(libhal-linux LANGUAGES CXX)

libhal_test_and_make_library(
  LIBRARY_NAME libhal-linux

  SOURCES
  src/output_pin.cpp
  src/input_pin.cpp
  src/i2c.cpp
  src/serial.cpp

  TEST_SOURCES
  tests/output_pin.test.cpp
  tests/main.test.cpp

  PACKAGES
  libhal
  libhal-util

  LINK_LIBRARIES
  libhal::libhal
  libhal::util
)