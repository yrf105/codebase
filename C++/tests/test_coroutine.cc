#include <coroutine>
#include <iostream>
#include <stdexcept>
#include <thread>
 
auto switch_to_new_thread(std::jthread& out) {
  struct awaitable {
    std::jthread* p_out;
    bool await_ready() { return false; }
    void await_suspend(std::coroutine_handle<> h) {
      std::jthread& out = *p_out;
      if (out.joinable())
        throw std::runtime_error("Output jthread parameter not empty");
      out = std::jthread([h] { h.resume(); });
      // 潜在的未定义行为：访问潜在被销毁的 *this
      // std::cout << "New thread ID: " << p_out->get_id() << "\n";
      std::cout << "New thread ID: " << out.get_id() << "\n"; // 这 OK
    }
    void await_resume() {}
  };
  return awaitable{&out};
}
 
task<> resuming_on_new_thread(std::jthread& out) {
  std::cout << "Coroutine started on thread: " << std::this_thread::get_id() << "\n";
  co_await switch_to_new_thread(out);
  // 等待器销毁于此
  std::cout << "Coroutine resumed on thread: " << std::this_thread::get_id() << "\n";
}
 
int main() {
  std::jthread out;
  resuming_on_new_thread(out);
}