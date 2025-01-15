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

## Usage

### Lookup stocks for a specific date
```bash
./stock_analyzer lookup ../data/sample_stock_data.csv 2020-01-02
```

### Project stocks with custom parameters
```bash
./stock_analyzer project ../data/sample_stock_data.csv 2020-01-02 7 random
```

### Rebalance
```bash
./stock_analyzer rebalance ../data/sample_stock_data.csv ../data/portfolio.json 5 3 random
```

### Need help?
```bash
./stock_analyzer --help
```