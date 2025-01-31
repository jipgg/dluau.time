cmake_minimum_required(VERSION 3.22.2)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(qol LANGUAGES CXX C)
find_package(Boost REQUIRED COMPONENTS container)
set(lib_EXPORT "DLUAU_TIME_EXPORT")

set(lib_DIR src)
set(require_DIR ${CMAKE_SOURCE_DIR}/require)
file(GLOB lib_SRC ${lib_DIR}/*.cpp)

add_subdirectory(extern/dluaulib_stub)

add_library(time SHARED ${lib_SRC})

target_compile_definitions(time PRIVATE ${lib_EXPORT})
target_include_directories(time PRIVATE include)
set_target_properties(time PROPERTIES OUTPUT_NAME time)
target_link_libraries(time PRIVATE dluaulib-stub Boost::container)
add_custom_command(TARGET time POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${require_DIR}/init.luau"
    "$<TARGET_FILE_DIR:time>"
)
