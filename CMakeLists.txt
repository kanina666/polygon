cmake_minimum_required(VERSION 3.21)
project(PolygonLabelEncoder)

add_subdirectory(googletest)

set(CMAKE_CXX_STANDARD 14)


add_subdirectory("googletest")

add_executable(PolygonLabelEncoder main.cpp)


add_executable(PolygonLabelEncoderTests tests.cpp)

add_executable(polygon main.cpp label_encoder.cpp)

target_link_libraries(PolygonLabelEncoderTests gtest_main gtest)

enable_testing()
add_test(NAME PolygonLabelEncoderTests COMMAND PolygonLabelEncoderTests)
