cmake_minimum_required(VERSION 3.27)
project(Raycaster)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

set(SDL2_PATH ${CMAKE_SOURCE_DIR}/vendor/SDL2)
set(SDL2_IMAGE_PATH ${CMAKE_SOURCE_DIR}/vendor/SDL2_image)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIRS})

add_executable(
        Raycaster
        main.cpp
        core/texture.cpp core/texture.h
)

target_link_libraries(Raycaster ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES})

file(COPY ${CMAKE_SOURCE_DIR}/vendor/SDL2/bin/SDL2.dll DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/vendor/SDL2_image/bin/SDL2_image.dll DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/textures DESTINATION ${CMAKE_BINARY_DIR})