#pragma once

#include <functional>
#include <type_traits>
#include <utility>
#include <vector>
class Callback {
  std::vector<std::function<void()>> cbs;

 public:
  template <class F, class... Args>
  void Add(F &&f, Args &&...args);

  void Run() {
    for (auto f : cbs) {
      f();
    }
  }
};

template <class F, class... Args>
void Callback::Add(F &&f, Args &&...args) {
  using return_type = typename std::invoke_result_t<F, Args...>;
  auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
  cbs.push_back([task]() { task(); });
}
