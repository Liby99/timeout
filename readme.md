# Timeout function in C++

Provide a task (in the form of a function) and a time. It either completes
the task within the budget or throw a timeout error. The timeout function itself
will return whatever the task function returns, and you should provide whatever
the task function requires as function arguments.

Internally, it creates another thread and uses mutex + conditional variables.
Refer to [this stackoverflow question](https://stackoverflow.com/questions/40550730/how-to-implement-timeout-for-function-in-c) for more information.

## How to use

You can run a simple lambda function with a timeout.

``` C++
timeout(1s, []() {
  std::this_thread::sleep_for(0.8s);
});
```

You can run a function pointer with arguments and return value, with a timeout:

``` C++
int my_sum(int a, int b) {
  std::this_thread::sleep_for(0.5s);
  return a + b;
}

// main
int j = timeout(1s, my_sum, 5, 6);
```

You can run a functor with or without argument or return value:

``` C++
struct Bar {
  int c;

  Bar(int c) : c(c) {}

  int operator()(int b) {
    return b + c + 3;
  }
};

// main
Bar b(10);
auto k = timeout(1s, b, 10);
```

> Note: to use chrono literals like `0.8s` please do `using namespace std::literals::chrono_literals;`
> beforehand.

## Install

Maybe just copy the `src/timeout.hpp` file directly?

## Test

```
$ make
$ ./tests/test
```