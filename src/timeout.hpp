#pragma once

#include <mutex>
#include <condition_variable>
#include <thread>

namespace timeout {
  namespace utils {
    template<typename F, typename... Args>
    struct ResultOf {
      using Type = decltype(std::declval<F>()(std::declval<Args>()...));
    };

    template<typename T>
    void wait(const T &time, std::condition_variable &cv, std::mutex &m) {
      std::unique_lock<std::mutex> l(m);
      if (cv.wait_for(l, time) == std::cv_status::timeout) {
        throw std::runtime_error("Timeout");
      }
    }
  }

  template<class T, class F, class... Args, std::enable_if_t<!std::is_same<typename utils::ResultOf<F, Args...>::Type, void>::value, int> = 0>
  typename utils::ResultOf<F, Args...>::Type timeout(const T &time, F &&f, Args &&... args) {
    std::mutex m;
    std::condition_variable cv;
    typename utils::ResultOf<F, Args...>::Type result;
    std::thread t([&cv, &f, &result, &args...]() { result = f(args...); cv.notify_one(); });
    t.detach();
    utils::wait(time, cv, m);
    return result;
  }

  template<class T, class F, class... Args, std::enable_if_t<std::is_same<typename utils::ResultOf<F, Args...>::Type, void>::value, int> = 0>
  void timeout(const T &time, F &&f, Args &&... args) {
    std::mutex m;
    std::condition_variable cv;
    std::thread t([&cv, &f, &args...]() { f(args...); cv.notify_one(); });
    t.detach();
    utils::wait(time, cv, m);
  }
}
