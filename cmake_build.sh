rm -r build
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE:PATH=../cmake/arm-none-eabi-gcc.cmake -DTARGET_CPU="cortex-m55"
make -j8
cd ..