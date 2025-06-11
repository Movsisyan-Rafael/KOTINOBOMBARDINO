cmake_minimum_required(VERSION 4.0.2)
project(Kotolet LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(SFML)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(include)

enable_testing()

add_executable(Kotolet src/main.cpp src/flyingcat.cpp)
target_compile_features(Kotolet PRIVATE cxx_std_17)
target_link_libraries(Kotolet PRIVATE SFML::Graphics)
target_link_libraries(Kotolet PRIVATE SFML::Audio)

include(CTest)
include(GoogleTest)
add_executable(MTest src/tests.cpp src/flyingcat.cpp)
target_compile_features(MTest PRIVATE cxx_std_17)
target_link_libraries(MTest PRIVATE GTest::gtest GTest::gtest_main)
target_link_libraries(MTest PRIVATE SFML::Graphics)
target_link_libraries(MTest PRIVATE SFML::Audio)
gtest_discover_tests(MTest)


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})