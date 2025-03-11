cmake_minimum_required(VERSION 3.15.0)

# For MSVC RUNTIME LIBRARY, need CMP0091=NEW and cmake 3.15+
cmake_policy(SET CMP0091 NEW)

set(AP_VERSION_MAJOR 1)
set(AP_VERSION_MINOR 0)
set(AP_VERSION_PATCH 0)
set(AP_VERSION ${AP_VERSION_MAJOR}.${AP_VERSION_MINOR}.${AP_VERSION_PATCH})

set(PACKAGE_NAME          "Arrow-Practices")
set(PACKAGE_VERSION       "${AP_VERSION}-dev")
set(PACKAGE_VERSION_HEX   "0x01000000")
set(PACKAGE_STRING        "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set(PACKAGE_TARNAME       "${PACKAGE_NAME}-${PACKAGE_VERSION}")
set(PACKAGE_BUGREPORT     "https://github.com/wegamekinglc/arrow-practices/issues/")

project(${PACKAGE_NAME} LANGUAGES CXX DESCRIPTION "The Arrow Practices Library")

message("-- CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
message("-- CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
message("-- CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message("-- VCPKG_TARGET_TRIPLET: ${VCPKG_TARGET_TRIPLET}")

set(CMAKE_CXX_STANDARD 17)
include(Platform)

find_package(ARROW CONFIG REQUIRED)
find_package(ArrowFlight CONFIG REQUIRED)
include_directories("${ARROW_INCLUDE_DIR}")
message("-- Arrow INCLUDE DIR: ${ARROW_INCLUDE_DIR}")

add_subdirectory(chapter1)
add_subdirectory(chapter4)
add_subdirectory(chapter7)