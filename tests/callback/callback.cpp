#include "callback.h"

#include <iostream>

int main() {
  Callback cb;
  cb.Add([]() { std::cout << "hello, callback" << '\n'; });
  auto f = [](int i) { std::cout << i << '\n'; };
  cb.Add(f, 5);
  cb.Run();
}
