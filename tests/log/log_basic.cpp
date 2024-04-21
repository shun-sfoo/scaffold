#include <iostream>

#include "leptlog.h"

#define TEST_FOREACH_FUNCTION(x) x(trace) x(debug) x(info) x(critical) x(warn) x(error) x(fatal)

void print_log_value() {
#define _FUNCTION(item) std::cout << static_cast<int>(leptlog::log_level::item) << '\n';
  TEST_FOREACH_FUNCTION(_FUNCTION)
#undef _FUNCTION
}

void print_log_name() {
#define _FUNCTION(item) std::cout << leptlog::details::log_level_name(leptlog::log_level::item) << '\n';
  TEST_FOREACH_FUNCTION(_FUNCTION)
#undef _FUNCTION
}

int main() {
  print_log_value();
  print_log_name();
}
