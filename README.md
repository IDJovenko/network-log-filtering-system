# network-log-filtering-system

## 1) Build static library

Build the static library with Make:

`make lib`

This creates: `ip_log_filtering.a`

The library can also be built with CMake:

```bash
cmake -S . -B build
cmake --build build
```

This project exports a CMake package, so it can be consumed from other CMake-based projects via `find_package()`.

**Public API header is in include/ip_log_filter.hpp**

## 2) Build and run demo

Build demo binary:

`make build_demo`

Build and run demo in one command:

`make demo`

## 3) Run tests

Build tests:

`make build_test`

Build and run tests:

`make test`
