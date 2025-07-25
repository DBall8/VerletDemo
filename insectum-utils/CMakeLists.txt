set(CMAKE_CXX_STANDARD 20)
add_library(insectum-utils
    "src/timing.cpp"
    "src/timing.hpp"
    "src/thread_pool.cpp"
    "src/thread_pool.hpp"
)

target_include_directories(insectum-utils PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")