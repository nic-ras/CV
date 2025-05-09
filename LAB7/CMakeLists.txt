cmake_minimum_required(VERSION 3.10)
project(ImageStitchingProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenCV REQUIRED)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(ImageStitchingProject
    src/main.cpp
    src/ImageLoader.cpp
)

target_link_libraries(ImageStitchingProject ${OpenCV_LIBS})