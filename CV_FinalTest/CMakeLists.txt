cmake_minimum_required(VERSION 3.10)
project(DocumentScanner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenCV REQUIRED)

add_executable(DocumentScanner
    src/main.cpp
    src/Functions/ImageLoader.cpp
    src/Functions/create3DChessboardCorners.cpp
)

target_link_libraries(DocumentScanner ${OpenCV_LIBS})

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)

