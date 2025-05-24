# Getting Started

This guide helps you build and install Flox on your machine.

## Prerequisites

- C++20 or later
- CMake 3.22+
- Git
- Linux (recommended)

## Clone and Build

```bash
git clone https://github.com/eeiaao/flox.git
cd flox
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## Run Tests

From the `build` directory:

```bash
ctest --output-on-failure
```

## Run Benchmarks

From the `build` directory:

```bash
./benchmarks/full_order_book_benchmark
```

Or run any other benchmark binary available in the `benchmarks/` directory.

## Install

To install the library system-wide:

```bash
sudo make install
```

## Use in Your Project

Flox is a collection of reusable low-latency building blocks.  
You can use it as a library for developing your own HFT engine or trading infrastructure.