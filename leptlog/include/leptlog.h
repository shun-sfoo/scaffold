#pragma once
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <source_location>
#include <string>

namespace leptlog {
#define LEPTLOG_FOREACH_FUNCTION(x) x(trace) x(debug) x(info) x(critical) x(warn) x(error) x(fatal)

enum class log_level : uint8_t {
#define _FUNCTION(item) item,
  LEPTLOG_FOREACH_FUNCTION(_FUNCTION)
#undef _FUNCTION
};

namespace details {

#if defined(__linux__) || defined(__APPLE__)
inline constexpr char k_level_ansi_colors[static_cast<std::uint8_t>(log_level::fatal) + 1][8] = {
    "\E[37m", "\E[35m", "\E[32m", "\E[34m", "\E[33m", "\E[31m", "\E[31;1m",
};
inline constexpr char k_reset_ansi_color[4] = "\E[m";
#define LEPTLOG_IF_HAS_ANSI_COLORS(x) x
#else
#define LEPTLOG_IF_HAS_ANSI_COLORS(x)
inline constexpr char k_level_ansi_colors[static_cast<std::uint8_t>(log_level::fatal) + 1][1] = {
    "", "", "", "", "", "", "",
};
inline constexpr char k_reset_ansi_color[4] = "";
#endif

inline std::string log_level_name(log_level lev) {
  switch (lev) {
#define _FUNCTION(item) \
  case log_level::item: \
    return #item;
    LEPTLOG_FOREACH_FUNCTION(_FUNCTION)
#undef _FUNCTION
  }
  return "unkown";
}

inline log_level log_level_from_name(std::string lev) {
#define _FUNCTION(item) \
  if (lev == #item) return log_level::item;
  LEPTLOG_FOREACH_FUNCTION(_FUNCTION)
#undef _FUNCTION
  return log_level::info;
}

template <typename T>
struct with_source_location {
 private:
  T m_inner;
  std::source_location m_loc;

 public:
  template <typename U>
    requires std::constructible_from<T, U>
  consteval with_source_location(U &&inner, std::source_location loc = std::source_location::current())
      : m_inner(std::forward<U>(inner)), m_loc(std::move(loc)) {}
  constexpr T const &format() const { return m_inner; }
  constexpr std::source_location const &location() const { return m_loc; };
};

inline log_level g_max_level = []() -> log_level {
  if (auto *lev = std::getenv("LEPTLOG_LEVEL")) {
    return log_level_from_name(lev);
  }
  return log_level::info;
}();

inline std::ofstream g_log_file = []() -> std::ofstream {
  if (auto *path = std::getenv("LEPTLOG_FILE")) {
    return std::ofstream(path, std::ios::app);
  }
  return std::ofstream();
}();

inline void output_log(log_level lev, std::string msg, std::source_location const &loc) {
  std::chrono::zoned_time now{std::chrono::current_zone(), std::chrono::high_resolution_clock::now()};
  msg = std::format("{} {}:{} [{}] {}", now, loc.file_name(), loc.line(), log_level_name(lev), msg);
  if (g_log_file) {
    g_log_file << msg + '\n';
  }
  if (lev >= g_max_level) {
    std::cout << LEPTLOG_IF_HAS_ANSI_COLORS(k_level_ansi_colors[static_cast<std::uint8_t>(lev)] +)
                         msg LEPTLOG_IF_HAS_ANSI_COLORS(+k_reset_ansi_color) +
                     '\n';
  }
}

template <typename... Args>
void generic_log(log_level lev, with_source_location<std::format_string<Args...>> fmt, Args &&...args) {
  auto const &loc = fmt.location();
  auto msg = std::vformat(fmt.format().get(), std::make_format_args(args...));
  output_log(lev, msg, loc);
}

}  // namespace details

inline void set_log_level(log_level lev) { details::g_max_level = lev; }
inline void set_log_file(std::string path) { details::g_log_file = std::ofstream(path, std::ios::app); }

#define _FUNCTION(name)                                                                             \
  template <typename... Args>                                                                       \
  void log_##name(details::with_source_location<std::format_string<Args...>> fmt, Args &&...args) { \
    generic_log(log_level::name, std::move(fmt), std::forward<Args>(args)...);                      \
  }
LEPTLOG_FOREACH_FUNCTION(_FUNCTION)
#undef _FUNCTION

#define LEPTLOG_P(x) ::leptlog::log_debug(#x "={}", x)
}  // namespace leptlog
