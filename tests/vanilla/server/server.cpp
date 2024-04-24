#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <array>
#include <cerrno>
#include <cstring>

#include "leptlog.h"
#include "util.h"

constexpr int MAX_EVENTS = 1000;
constexpr int BUFFER_SIZE = 1024;

void setnonblocking(int fd) { fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK); }

int main() {
  leptlog::set_log_level(leptlog::log_level::trace);

  auto servSock = socket(AF_INET, SOCK_STREAM, 0);
  errif(servSock == -1, "create socket error");

  sockaddr_in servAddr;
  bzero(&servAddr, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servAddr.sin_port = ntohs(8888);

  errif(bind(servSock, reinterpret_cast<sockaddr *>(&servAddr), sizeof(servAddr)) == -1, "bind error");
  errif(listen(servSock, SOMAXCONN) == -1, "listen error");

  auto epfd = epoll_create1(0);
  errif(epfd == -1, "epoll create error");

  epoll_event events[MAX_EVENTS], ev;
  bzero(events, sizeof(events));
  bzero(&ev, sizeof(ev));

  ev.data.fd = servSock;
  ev.events = EPOLLIN | EPOLLET;

  setnonblocking(servSock);
  errif(epoll_ctl(epfd, EPOLL_CTL_ADD, servSock, &ev) == -1, "epoll add error");

  while (true) {
    auto nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
    errif(nfds == -1, "epoll wait error");
    for (auto i = 0; i < nfds; ++i) {
      if (events[i].data.fd == servSock) {  // new connection
        sockaddr_in clntAddr;
        bzero(&clntAddr, sizeof(clntAddr));
        socklen_t socklen = sizeof(clntAddr);

        int clntSock = accept(servSock, reinterpret_cast<sockaddr *>(&clntAddr), &socklen);
        errif(clntSock == -1, "accept error");
        leptlog::log_info("new client fd {}! IP: {} Port: {}", clntSock, inet_ntoa(clntAddr.sin_addr),
                          ntohs(clntAddr.sin_port));
        bzero(&ev, sizeof(ev));
        ev.data.fd = clntSock;
        ev.events = EPOLLIN | EPOLLET;
        setnonblocking(clntSock);
        epoll_ctl(epfd, EPOLL_CTL_ADD, clntSock, &ev);
      } else if (events[i].events & EPOLLIN) {  // reading events
        auto clntFd = events[i].data.fd;
        std::array<char, BUFFER_SIZE> buf;
        while (true) {
          auto readBytes = read(clntFd, buf.data(), sizeof(buf));
          if (readBytes > 0) {
            leptlog::log_info("message for client {} {}", clntFd, buf.data());
            write(clntFd, buf.data(), sizeof(buf));
          } else if (readBytes == -1 && errno == EINTR) {
            leptlog::log_debug("continue reading");
            continue;
          } else if (readBytes == -1 && ((errno == EAGAIN || (errno == EWOULDBLOCK)))) {
            leptlog::log_debug("finish read once , errno {}", errno);
            break;
          } else if (readBytes == 0) {
            leptlog::log_info("EOF client {} disconnected", clntFd);
            close(clntFd);
            break;
          }
        }
      } else {
        leptlog::log_debug("somethin else happend");
      }
    }
  }
  close(servSock);
  return 0;
}
