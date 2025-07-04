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

#include <cerrno>
#include <cstring>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <string>

#include <libhal-linux/errors.hpp>
#include <libhal-linux/serial.hpp>
#include <libhal-util/math.hpp>
#include <libhal/error.hpp>

namespace {
auto linux_read(int fd, void* buf, size_t nbytes)
{
  return read(fd, buf, nbytes);
}

auto linux_write(int fd, const void* buf, size_t nbytes)
{
  return write(fd, buf, nbytes);
}
}  // namespace

namespace hal::gnu_linux {
serial::serial(const std::string& p_file_path, serial::settings p_settings)
{
  m_fd = open(p_file_path.c_str(), O_RDWR | O_NDELAY | O_NOCTTY);
  if (m_fd < 0) {
    perror("Error opening serial connection");
    throw hal::gnu_linux::invalid_character_device(p_file_path, errno, this);
  }
  configure(p_settings);
}

serial::~serial()
{
  close(m_fd);
}

void serial::driver_configure(const settings& p_settings)
{
  tcflag_t control_flags = CS8 | CLOCAL | CREAD;
  tcflag_t input_flags = 0;

  // Stop Bit
  if (p_settings.stop == settings::stop_bits::two) {
    control_flags |= CSTOPB;
  }

  // Parity Settings
  switch (p_settings.parity) {
    default:  // shouldn't be needed, fail safe
    case settings::parity::none:
      input_flags |= IGNPAR;
      break;

    case settings::parity::odd:
      control_flags |= PARODD;
    case settings::parity::even:
      control_flags |= PARENB;
      input_flags |= INPCK;
      break;
    case settings::parity::forced0:
    case settings::parity::forced1:
      throw hal::operation_not_supported(this);
  }

  // Baudrate settings
  uint64_t baud = static_cast<int>(p_settings.baud_rate);
  int internal_baud = 0;

  switch (baud) {
    case 0:
      internal_baud = B0;
      break;
    case 50:
      internal_baud = B50;
      break;
    case 75:
      internal_baud = B75;
      break;
    case 110:
      internal_baud = B110;
      break;
    case 134:
      internal_baud = B134;
      break;
    case 150:
      internal_baud = B150;
      break;
    case 200:
      internal_baud = B200;
      break;
    case 300:
      internal_baud = B300;
      break;
    case 600:
      internal_baud = B600;
      break;
    case 1200:
      internal_baud = B1200;
      break;
    case 1800:
      internal_baud = B1800;
      break;
    case 2400:
      internal_baud = B2400;
      break;
    case 4800:
      internal_baud = B4800;
      break;
    case 9600:
      internal_baud = B9600;
      break;
    case 19200:
      internal_baud = B19200;
      break;
    case 38400:
      internal_baud = B38400;
      break;
    case 57600:
      internal_baud = B57600;
      break;
    case 115200:
      internal_baud = B115200;
      break;
    case 230400:
      internal_baud = B230400;
      break;
    case 460800:
      internal_baud = B460800;
      break;
    case 500000:
      internal_baud = B500000;
      break;
    case 576000:
      internal_baud = B576000;
      break;
    case 921600:
      internal_baud = B921600;
      break;
    case 1000000:
      internal_baud = B1000000;
      break;
    case 1152000:
      internal_baud = B1152000;
      break;
    case 1500000:
      internal_baud = B1500000;
      break;
    case 2000000:
      internal_baud = B2000000;
      break;
    default:
      throw hal::operation_not_supported(this);
  }

  control_flags |= internal_baud;

  termios options{};
  options.c_cflag = control_flags;
  options.c_iflag = input_flags;
  options.c_oflag = 0;
  options.c_lflag = 0;

  flush();
  int res = tcsetattr(m_fd, TCSANOW, &options);
  if (res < 0) {
    perror("Unable to configure serial device");
  }
}

hal::serial::write_t serial::driver_write(std::span<const hal::byte> p_data)
{
  uint32_t write_res = linux_write(m_fd, &p_data.data()[0], p_data.size());
  if (write_res < 0) {
    perror("Failed to write\n");
    throw hal::io_error(this);
  }
  return write_t{ .data = p_data.subspan(0, write_res) };
}

hal::serial::read_t serial::driver_read(std::span<hal::byte> p_data)
{
  uint32_t read_res = linux_read(m_fd, &p_data.data()[0], p_data.size());
  if (read_res < 0) {
    perror("Failed to read\n");
    throw hal::io_error(this);
  }
  return read_t{ .data = p_data.subspan(0, read_res),
                 .available = read_res,
                 .capacity = p_data.size() };
}

void serial::driver_flush()
{
  if (m_fd < 0) {
    throw hal::operation_not_permitted(this);
  }
  tcflush(m_fd, TCIFLUSH);  // Flushes both TX and RX
}
}  // namespace hal::gnu_linux
