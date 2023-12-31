cmake_minimum_required(VERSION 3.0)
project(TUV4L2)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif()

enable_language(CXX)
set(CMAKE_CXX_STANDARD		17)
set(CMAKE_CXX_STANDARD_REQUIRED	ON)
set(CMAKE_CXX_EXTENSIONS	OFF)
set(CMAKE_CXX_FLAGS_RELEASE	"-Wall -O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG	"-g")
add_definitions(-DNDEBUG -DTUV4L2PP_CONF_DIR="${CMAKE_INSTALL_PREFIX}/etc")

find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED)
pkg_check_modules(YAML-CPP REQUIRED yaml-cpp)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${CMAKE_INSTALL_PREFIX}/include
    ${Boost_INCLUDE_DIRS}
    ${YAML-CPP_INCLUDE_DIRS}
)

link_directories(${CMAKE_INSTALL_PREFIX}/lib)

file(GLOB sources *.cc)
add_library(${PROJECT_NAME} SHARED ${sources})
target_link_libraries(${PROJECT_NAME} TUTools ${YAML-CPP_LIBRARIES})

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)

add_subdirectory(TU)
add_subdirectory(testv4l2camera)

option(BUILD_TUV4L2PP_TEST_PROGRAMS "Building test programs for TUV4L2" OFF)

if(BUILD_TUV4L2PP_TEST_PROGRAMS)
  add_subdirectory(testcam)
  add_subdirectory(testcam2)
endif()
