# Timeout function in C++

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

## Install

Maybe just copy the `src/timeout.hpp` file directly?

## Test

```
$ make
$ ./tests/test
```