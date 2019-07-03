# Copy the file from the build directory
cp build/CMakeLists.txt CMakeLists.txt

# Now change into that directory
cd build

# Build
cmake ../

# Make
make

# And clean up
cd ..
rm CMakeLists.txt
mv -f build/collision collision