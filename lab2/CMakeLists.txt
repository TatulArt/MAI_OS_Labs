cmake_minimum_required(VERSION 4.2)

project(lab2
        LANGUAGES C)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(lab2
        src/main.c
        src/geometry.c
        src/metrics.c
)

target_include_directories(lab2
        PRIVATE
        ${INCLUDE_DIR}
)

target_link_libraries(lab2
        PRIVATE
        m pthread
)
