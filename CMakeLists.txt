cmake_minimum_required(VERSION 3.16)
project(chip-8-emulator LANGUAGES C)

find_package(SDL3 REQUIRED)

set(CMAKE_PREFIX_PATH "/usr")

add_executable(${PROJECT_NAME}
  src/main.c
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
