#include "leptlog.h"

int main() {
    leptlog::log_info("hello, the answer is {}", 42);
    leptlog::log_critical("this is right-aligned [{:>+10.04f}]", 3.14);

    leptlog::log_warn("good job, {1:.5s} for making {0}", "leptlog", "username");
    leptlog::set_log_level(leptlog::log_level::trace); // default log level is info
    // leptlog::set_log_file("lept.log"); // uncomment to dump log to a specific file

    int my_variable = 42;
    LEPTLOG_P(my_variable); // shown when log level lower than debug

    leptlog::log_trace("below is the color show :)");
#define _FUNCTION(name) leptlog::log_##name(#name);
    LEPTLOG_FOREACH_FUNCTION(_FUNCTION)
#undef _FUNCTION

    return 0;
}
