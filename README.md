# Set vcpkg local environment variable
echo 'export VCPKG_ROOT="$HOME/vcpkg"' >> ~/.bashrc

# Install new packages with
./vcpkg install ./vcpkg install [pkg_name:system_name]

# 

# Make sure you're in your project root
mkdir build
cd build
cmake ..
cmake --build .

# Run the executable (from build directory)
./cpp_prototype_template