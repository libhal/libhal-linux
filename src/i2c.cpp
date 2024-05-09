#include <array>
#include <cstring>
#include <fcntl.h>
#include <libhal/i2c.hpp>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <libhal-linux/i2c.hpp>

namespace {
inline auto linux_read(int fd, void* buf, size_t nbytes)
{
  return read(fd, buf, nbytes);
}

inline auto linux_write(int fd, const void* buf, size_t nbytes)
{
  return write(fd, buf, nbytes);
}
}  // namespace

namespace hal::gnu_linux {
i2c::i2c(const std::string& p_file_path)
{
  m_fd = open(p_file_path.c_str(), O_RDWR);
  if (m_fd < 0) {
    throw hal::io_error(this);
  }
}

i2c::~i2c()
{
  close(m_fd);
}

void i2c::driver_configure([[maybe_unused]] const settings& p_settings)
{
  return;
}

void i2c::driver_transaction(
  hal::byte p_address,
  std::span<const hal::byte> p_data_out,
  std::span<hal::byte> p_data_in,
  [[maybe_unused]] hal::function_ref<hal::timeout_function> p_timeout)
{
  // Check if we're a 10 bit address
  // The first 5 bytes MUST be this pattern to be considered a 10 bit address
  constexpr hal::byte ten_bit_mask = 0b1111'0 << 2;
  const auto is_ten_bit = (p_address & ten_bit_mask) == ten_bit_mask;
  std::uint16_t real_address = 0;

  if (is_ten_bit) {
    real_address = p_address << 8 | p_data_out[0];
  } else {
    real_address = p_address;
  }
  // Enable 10 bit mode if set
  if (ioctl(m_fd, I2C_TENBIT, is_ten_bit) < 0) {

    throw hal::operation_not_supported(this);
  }

  // Set peripheral address
  if (ioctl(m_fd, I2C_SLAVE, real_address) < 0) {

    throw hal::no_such_device(real_address, this);
  }

  const bool is_reading = p_data_out.empty();
  const bool write_then_read =
    &p_data_out.data()[0] != nullptr && &p_data_in.data()[0] != nullptr;

  if (write_then_read) {
    i2c_rdwr_ioctl_data data_queue;
    std::array<i2c_msg, 2> msgs;
    // First, the message thats to be written
    msgs[0].addr = real_address;
    msgs[0].buf = (__u8*)(&p_data_out.data()[0]);
    msgs[0].flags = 0;
    msgs[0].len = p_data_out.size();

    // Next, the message thats to be read
    msgs[1].addr = real_address;
    msgs[1].buf = (__u8*)(&p_data_in.data()[0]);
    msgs[1].flags = I2C_M_RD;
    msgs[1].len = p_data_in.size();

    data_queue.nmsgs = 2;
    data_queue.msgs = &msgs[0];
    if (ioctl(m_fd, I2C_RDWR, &data_queue) < 0) {

      throw hal::operation_not_permitted(this);
    }
    return;
  }

  if (is_reading) {
    if (linux_read(m_fd, &p_data_in.data()[0], p_data_in.size()) == -1) {

      throw hal::operation_not_permitted(this);
    }
  } else {
    if (linux_write(m_fd, &p_data_out.data()[0], p_data_out.size()) == -1) {

      throw hal::operation_not_permitted(this);
    }
  }
}

}  // namespace hal::gnu_linux