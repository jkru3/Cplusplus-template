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

### Process stock data
```bash
./stock_analyzer --process-stocks --input ./raw_data --output ./processed_data
```

### Generate 100 portfolios
```bash
./stock_analyzer --generate-portfolios --input ./processed_data --output ./portfolios --portfolio-count 100
```

### Run predictions
```bash
./stock_analyzer --stocks ../data/test_stocks.csv --portfolio ../data/test_portfolio.json
```

### Need help?
```bash
./stock_analyzer --help