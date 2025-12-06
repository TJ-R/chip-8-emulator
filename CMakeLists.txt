cmake_minimum_required(VERSION 3.16)
project(chip-8-emulator LANGUAGES C)

find_package(SDL3 REQUIRED)

set(CMAKE_PREFIX_PATH "/usr")

file(GLOB C_SOURCES "src/*.c")
file(GLOB H_SOURCES "src/*.h")


add_executable(${PROJECT_NAME}
  ${C_SOURCES}
  ${H_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
