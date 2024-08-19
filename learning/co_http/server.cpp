#include <arpa/inet.h>
#include <cstddef>
#include <fmt/core.h>
#include <fmt/format.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

#include <clocale>
#include <cstring>
#include <string>

#define CHECK_ALL(func, ...) check_error(#func, func(__VA_ARGS__))

int check_error(const char *msg, int res) {
  if (res == -1) {
    fmt::println("{}: {}", msg, strerror(errno));
    throw;
  }
  return res;
}

size_t check_error(const char *msg, ssize_t res) {
  if (res == -1) {
    fmt::println("{}: {}", msg, strerror(errno));
    throw;
  }
  return res;
}

struct socket_address_fatptr {
  sockaddr *m_addr;
  socklen_t m_addrlen;
};

struct socket_address_storage {
  union {
    struct sockaddr m_addr;
    struct sockaddr_storage m_addr_storage;
  };
  socklen_t m_addrlen = sizeof(struct sockaddr_storage);
  operator socket_address_fatptr() { return {&m_addr, m_addrlen}; }
};

struct address_resolver_entry {
  addrinfo *m_curr = nullptr;
  socket_address_fatptr get_address() const { return {m_curr->ai_addr, m_curr->ai_addrlen}; }

  int create_socket() const {
    int sockfd = CHECK_ALL(socket, m_curr->ai_family, m_curr->ai_socktype, m_curr->ai_protocol);
    return sockfd;
  }

  int create_socket_and_bind() const {
    int sockfd = create_socket();
    socket_address_fatptr server_addr = get_address();
    CHECK_ALL(bind, sockfd, server_addr.m_addr, server_addr.m_addrlen);
    return sockfd;
  }

  [[nodiscard]] bool next_entry() {
    m_curr = m_curr->ai_next;
    if (m_curr == nullptr) {
      return false;
    }
    return true;
  }
};

struct address_resolver {
  addrinfo *m_head = nullptr;

  address_resolver_entry resolve(std::string const &name, std::string const &service) {
    int err = getaddrinfo(name.c_str(), service.c_str(), nullptr, &m_head);
    if (err != 0) {
      fmt::println("getaddrinfo: {} {}", gai_strerror(err), err);
      throw;
    }
    return {m_head};
  }

  address_resolver_entry get_first_entry() { return {m_head}; }

  address_resolver() = default;

  address_resolver(address_resolver &&that) : m_head(that.m_head) { that.m_head = nullptr; }

  ~address_resolver() {
    if (m_head) {
      freeaddrinfo(m_head);
    }
  }
};

int main() {
  // setlocale(LC_ALL, "zh_CN.UTF-8");
  address_resolver resolver;
  auto entry = resolver.resolve("127.0.0.1", "8080");
  int listenfd = entry.create_socket_and_bind();
  CHECK_ALL(listen, listenfd, SOMAXCONN);
  while (true) {
    socket_address_storage addr;
    int connid = CHECK_ALL(accept, listenfd, &addr.m_addr, &addr.m_addrlen);
    std::thread([connid] {
      char buf[1024];
      size_t n = CHECK_ALL(read, connid, buf, sizeof(buf));
      auto req = std::string(buf, n);
      fmt::println("request: {}", req);
      std::string res = "你好 ! " + req;
      fmt::println("response: {}", res);
      CHECK_ALL(write, connid, res.data(), res.size());
      close(connid);
    }).detach();
  }
  return 0;
}
