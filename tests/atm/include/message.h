#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

namespace messaging {

// 1 消息基类。队列中存储的项目
struct message_base {
  virtual ~message_base() {}
};

// 2 每种消息都具有特化类型
template <typename Msg>
struct wrapped_message : message_base {
  Msg contents;
  explicit wrapped_message(Msg const& contents_) : contents(contents_) {}
};

class queue {
  std::mutex m;
  std::condition_variable c;
  std::queue<std::shared_ptr<message_base>> q;

 public:
  template <typename T>
  void push(T const& msg) {
    std::lock_guard lk(m);
    q.push(std::make_shared<wrapped_message<T>>(msg));
    c.notify_all();
  }

  std::shared_ptr<message_base> wait_and_pop() {
    std::unique_lock lk(m);
    c.wait(lk, [&] { return !q.empty(); });
    auto res = q.front();
    q.pop();
    return res;
  }
};

}  // namespace messaging
