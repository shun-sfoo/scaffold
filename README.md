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

3. log with the parameter name

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

1. Demostration video: <https://www.bilibili.com/video/BV1t94y1r72E>
2. Github link : <https://github.com/archibate/minilog>

Leak compare spdlog

1. limit the file size
2. async output

## co-http

c++ 17 标准的实现的http服务器， 手动实现协程。

### http 新式函数

以前初始化http协议需要通过如下的形式

```cpp
#include <apra/inet.h>
struct sockaddr_in serr_addr;
bzero(&serv_addr, sizeof(serv_addr));
serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
serv_addr.sin_port = htons(8888);
```

通过新api 只需要如下的方式

```cpp
#include <netdb.h>
addrinfo *m_head = nullptr;
int err = getaddrinfo("127.0.0.1", "8080", nullptr, &m_head);
```

其中 err 如果不为0 可以通过 gai_strerror(err) 获取错误信息。
gai 是 getaddrinfo 的缩写。该库中重新定义了一套错误库。


### 关于 thread

thread 捕获一般都是使用按值捕获。

按引用捕获一般用于 std::visit 它是就地调用。

