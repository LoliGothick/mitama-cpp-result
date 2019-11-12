#ifndef MITAMA_PANIC_HPP
#define MITAMA_PANIC_HPP

#include <stdexcept>
#include <variant>
#include <utility>
#include <string>
#include <string_view>
#include <fmt/core.h>
#include <mitama/concepts/display.hpp>
#if defined(MITAMA_PANIC_WITH_STACKTRACE) 
  #ifdef _WIN32
  #else
    #define BOOST_STACKTRACE_USE_ADDR2LINE
  #endif
  #include <boost/stacktrace.hpp>
#endif

namespace mitama {

class macro_use_tag_t{};
inline static constexpr macro_use_tag_t macro_use{};

class stacktarce_use_tag_t{};
inline static constexpr stacktarce_use_tag_t stacktarce_use{};

class runtime_panic : public std::runtime_error
{
public:
  template <class... Args>
  runtime_panic(std::string fmt, Args &&... args) noexcept
      : std::runtime_error(fmt::format(fmt, std::forward<Args>(args)...)) {}

  template <class... Args>
  explicit runtime_panic(macro_use_tag_t, const char *func, int line, std::string fmt, Args &&... args) noexcept
      : std::runtime_error(
           std::string{"runtime panicked at '"}
         + fmt::format(fmt, as_display(std::forward<Args>(args)).as_str()...)
         + fmt::format("', {}:{}", func, line)
      )
  {}

#if defined(MITAMA_PANIC_WITH_STACKTRACE)
  template <class StackTrace, class... Args>
  explicit runtime_panic(stacktarce_use_tag_t, const char *func, int line, StackTrace&& st, std::string fmt, Args &&... args) noexcept
      : std::runtime_error(
           std::string{"runtime panicked at '"}
         + fmt::format(fmt, as_display(std::forward<Args>(args)).as_str()...)
         + fmt::format("', {}:{}\n\nstacktrace:\n{}", func, line, as_display(std::forward<StackTrace>(st)).as_str())
      )
    {}
#endif
};
}

#if !defined(MITAMA_PANIC_WITH_STACKTRACE) 
#define PANIC(...) \
  throw ::mitama::runtime_panic { ::mitama::macro_use, __FILE__, __LINE__, __VA_ARGS__ }
#else
#define PANIC(...) \
  throw ::mitama::runtime_panic { ::mitama::stacktarce_use, __FILE__, __LINE__, boost::stacktrace::stacktrace(), __VA_ARGS__ }
#endif

#endif
