#pragma once
#include <libhal/serial.hpp>
#include <span>

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