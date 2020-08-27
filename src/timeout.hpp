#pragma once

#include <mutex>
#include <condition_variable>
#include <thread>

namespace to {
  template<
    class T,
    class F,
    class... Args,
    std::enable_if_t<
      !std::is_same<decltype(std::declval<F>()(std::declval<Args>()...)), void>::value,
      int> = 0
  >
  auto timeout(
    const T &time,
    F &&f,
    Args &&... args
  ) -> decltype(std::declval<F>()(std::declval<Args>()...)) {
    using ResultType = decltype(std::declval<F>()(std::declval<Args>()...));
    std::mutex m;
    std::condition_variable cv;
    ResultType result;
    std::thread t([&cv, &f, &result, &args...]() {
      result = f(args...);
      cv.notify_one();
    });
    t.detach();
    {
      std::unique_lock<std::mutex> l(m);
      if (cv.wait_for(l, time) == std::cv_status::timeout) {
        throw std::runtime_error("Timeout");
      }
    }
    return result;
  }

  template<
    class T,
    class F,
    class... Args,
    std::enable_if_t<
      std::is_same<decltype(std::declval<F>()(std::declval<Args>()...)), void>::value,
      int> = 0
  >
  void timeout(
    const T &time,
    F &&f,
    Args &&... args
  ) {
    std::mutex m;
    std::condition_variable cv;
    std::thread t([&cv, &f, &args...]() {
      f(args...);
      cv.notify_one();
    });
    t.detach();
    {
      std::unique_lock<std::mutex> l(m);
      if (cv.wait_for(l, time) == std::cv_status::timeout) {
        throw std::runtime_error("Timeout");
      }
    }
  }
}
