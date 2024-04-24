#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>

#include "leptlog.h"
#include "util.h"

constexpr int BUFFER_SIZE = 1024;

int main() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  errif(sockfd == -1, "socket create error");

  struct sockaddr_in serv_addr;
  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(8888);

  errif(connect(sockfd, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1, "socket connect error");

  while (true) {
    char buf[BUFFER_SIZE];  // 在这个版本，buf大小必须大于或等于服务器端buf大小，不然会出错，想想为什么？
    bzero(&buf, sizeof(buf));
    scanf("%s", buf);
    ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
    if (write_bytes == -1) {
      leptlog::log_warn("socket already disconnected, can't write any more!");
      break;
    }
    bzero(&buf, sizeof(buf));
    ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
    if (read_bytes > 0) {
      leptlog::log_info("message from server: {}", buf);
    } else if (read_bytes == 0) {
      leptlog::log_warn("server socket disconnected!");
      break;
    } else if (read_bytes == -1) {
      close(sockfd);
      errif(true, "socket read error");
    }
  }
  close(sockfd);
  return 0;
}
