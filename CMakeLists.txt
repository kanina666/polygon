cmake_minimum_required(VERSION 3.21)
project(PolygonLabelEncoder)

set(CMAKE_CXX_STANDARD 14)

# Убедимся, что CMake найдет заголовочные файлы в корневой директории
include_directories(${CMAKE_SOURCE_DIR})

# Основной исполняемый файл, который использует label_encoder
add_executable(PolygonLabelEncoder main.cpp label_encoder.cpp)

# Тестовый исполняемый файл, который использует label_encoder и тесты
add_executable(PolygonLabelEncoderTests tests.cpp label_encoder.cpp)

# Линковка GoogleTest библиотек для тестов
target_link_libraries(PolygonLabelEncoderTests gtest_main gtest)

# Включаем тестирование
enable_testing()

# Добавляем тесты
add_test(NAME PolygonLabelEncoderTests COMMAND PolygonLabelEncoderTests)
