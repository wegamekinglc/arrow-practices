#!/bin/bash -e

NUM_CORES=$(grep -c processor /proc/cpuinfo)
export WORK_DIR=$PWD
export LD_LIBRARY_PATH=$PWD/lib:$LD_LIBRARY_PATH
export BUILD_TYPE=Release
export USE_COVERAGE=false  # make it `false` when you need a full performance lib
export CMAKE_EXPORT_COMPILE_COMMANDS=on
export BUILD_SHARED_LIBS=off
export CMAKE_TOOLCHAIN_FILE=$PWD/vcpkg/scripts/buildsystems/vcpkg.cmake

echo NUM_CORES: $NUM_CORES
echo BUILD_TYPE: $BUILD_TYPE
echo DAL_DIR: "$WORK_DIR"
echo USE_COVERAGE: $USE_COVERAGE
echo BUILD_SHARED_LIBS: $BUILD_SHARED_LIBS
echo CMAKE_EXPORT_COMPILE_COMMANDS: $CMAKE_EXPORT_COMPILE_COMMANDS
echo CMAKE_TOOLCHAIN_FILE: "$CMAKE_TOOLCHAIN_FILE"

(
cd vcpkg
if [ -f "./vcpkg" ]; then
  echo "vcpkg executable already exists"
else
  bash bootstrap-vcpkg.sh
fi

cd ports/arrow
../../vcpkg install --x-feature=flight
)

if [ $? -ne 0 ]; then
  exit 1
fi

rm -rf build
mkdir -p build
(
cd build || exit
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX="$WORK_DIR" -DUSE_COVERAGE=$USE_COVERAGE -DCMAKE_EXPORT_COMPILE_COMMANDS=$CMAKE_EXPORT_COMPILE_COMMANDS -DBUILD_SHARED_LIBS=$BUILD_SHARED_LIBS -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN_FILE ..
make -j"${NUM_CORES}"
make install
)

if [ $? -ne 0 ]; then
  exit 1
fi

echo "Finished building of Arrow Practices"