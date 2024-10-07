#include <unistd.h>

#include <iostream>
#include <span>
#include <string_view>

#include <libhal-linux/serial.hpp>
#include <libhal-util/serial.hpp>

void application()
{
  std::cout << "UART test\n";
  constexpr auto serial_file_path = "/dev/serial0";
  auto serial_bus = hal::gnu_linux::serial(serial_file_path);
  std::string_view const test_str = "Hello from libhal\n";
  std::array<hal::byte, 255> input_buffer = { 0 };

  while (true) {
    hal::print(serial_bus, test_str);

    sleep(1);

    auto const received_data = serial_bus.read(input_buffer);

    if (received_data.data.empty()) {
      std::cout << "Nothing to read\n";
      continue;
    }

    std::cout << "Len of res buffer: " << received_data.data.size()
              << " len of input buffer: " << input_buffer.size() << "\n";
    auto const read_string =
      std::string_view(reinterpret_cast<char const*>(received_data.data.data()),
                       received_data.data.size());

    sleep(1);

    std::cout << "Read from serial: " << read_string << "\n";
  }
}