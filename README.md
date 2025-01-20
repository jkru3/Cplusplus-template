# Stock Analyzer

## Prerequisites
- CMake 3.15 or higher
- vcpkg package manager
- C++20 compatible compiler

## vcpkg 

### Install vcpkg if you haven't already:
```bash
git clone https://github.com/Microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh
```

### Set vcpkg local environment variable
```bash
echo 'export VCPKG_ROOT="$HOME/vcpkg"' >> ~/.bashrc
```

### Make sure you're in your project root
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Debug
```bash
cmake -DCMAKE_BUILD_TYPE=Debug
cmake --build .
lldb [usage]
```

## Usage
```bash
./stock_analyzer
```

# TODO:
- We need future stock prediction
- test writing to portfolios