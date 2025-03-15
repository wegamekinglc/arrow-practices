#!/bin/bash -e

NUM_CORES=$(grep -c processor /proc/cpuinfo)
export WORK_DIR=$PWD
export LD_LIBRARY_PATH=$PWD/lib:$LD_LIBRARY_PATH
export BUILD_TYPE=Release
export USE_COVERAGE=false  # make it `false` when you need a full performance lib
export CMAKE_EXPORT_COMPILE_COMMANDS=on
export BUILD_SHARED_LIBS=off
export VCPKG_TARGET_TRIPLET=x64-linux

echo NUM_CORES: $NUM_CORES
echo BUILD_TYPE: $BUILD_TYPE
echo DAL_DIR: "$WORK_DIR"
echo USE_COVERAGE: $USE_COVERAGE
echo BUILD_SHARED_LIBS: $BUILD_SHARED_LIBS
echo CMAKE_EXPORT_COMPILE_COMMANDS: $CMAKE_EXPORT_COMPILE_COMMANDS
echo VCPKG_TARGET_TRIPLET: $VCPKG_TARGET_TRIPLET

(
cd vcpkg
if [ -f "./vcpkg" ]; then
  echo "vcpkg executable already exists"
else
  bash bootstrap-vcpkg.sh
fi
./vcpkg install arrow:$VCPKG_TARGET_TRIPLET
)

if [ $? -ne 0 ]; then
  exit 1
fi

rm -rf build
mkdir -p build
(
cd build || exit
cmake --preset ${BUILD_TYPE}-linux -DUSE_COVERAGE=$USE_COVERAGE -DCMAKE_EXPORT_COMPILE_COMMANDS=$CMAKE_EXPORT_COMPILE_COMMANDS -DBUILD_SHARED_LIBS=$BUILD_SHARED_LIBS -DVCPKG_TARGET_TRIPLET=$VCPKG_TARGET_TRIPLET ..
make -j"${NUM_CORES}"
make install
)

if [ $? -ne 0 ]; then
  exit 1
fi

echo "Finished building of Arrow Practices"