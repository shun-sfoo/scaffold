# scaffold

Linux c++ scaffold project without third library

## leptlog

Usage:

1. basic

```cpp

    leptlog::log_info("hello, the answer is {}", 42);
    leptlog::log_warn("good job, {1:.5s} for making {0}", "leptlog", "username");
```

2. set log level

```cpp
    leptlog::set_log_level(leptlog::log_level::trace); // default log level is info
```

3.  log with the parameter name

```cpp
    LEPTLOG_P(my_variable); // shown when log level lower than debug
```

4. enable log file

```cpp
     leptlog::set_log_file("lept.log"); // uncomment to dump log to a specific file
```

Knowledge:

1. `c++ 20 format`
2. `X-macro` for enum class reflect

Study by:

1. Demostration video: https://www.bilibili.com/video/BV1t94y1r72E
2. Github link : https://github.com/archibate/minilog

Leak compare spdlog

1. limit the file size
2. async output
