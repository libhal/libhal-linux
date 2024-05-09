#pragma once

#include <libhal/i2c.hpp>
#include <string>

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