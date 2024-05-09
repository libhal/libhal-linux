#pragma once
#include <libhal/error.hpp>
#include <string>
// This is to be internal, will be in the precompiled shared object

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