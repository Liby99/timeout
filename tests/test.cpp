#include <cstdio>
#include <chrono>
#include <thread>
#include <stdexcept>

#include "../src/timeout.hpp"

using namespace std::literals::chrono_literals;
using namespace to;

int my_sum(int a, int b) {
  std::this_thread::sleep_for(0.5s);
  return a + b;
}

void foo(int c) {
  auto d = c + 10;
}

struct Bar {
  int c;

  Bar(int c) : c(c) {}

  int operator()(int b) {
    return b + c + 3;
  }
};

int main() {

  // Lambda function + return value
  auto i = timeout(1s, []() {
    std::this_thread::sleep_for(0.5s);
    return 1;
  });
  printf("1: %d\n", i);

  // Lambda function without return value
  timeout(1s, []() {
    std::this_thread::sleep_for(0.8s);
  });
  printf("2: \n");

  // Function pointer with arguments and return value
  auto j = timeout(1s, my_sum, 5, 6);
  printf("3: %d\n", j);

  // Function pointer with arguments
  timeout(1s, foo, 3);
  printf("4: \n");

  // Functor with arguments and return value
  Bar b(10);
  auto k = timeout(1s, b, 10);
  printf("5: %d\n", k);
}