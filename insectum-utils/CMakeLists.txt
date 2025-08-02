set(CMAKE_CXX_STANDARD 20)
add_library(insectum-utils
    "src/timing.cpp"
    "src/timing.hpp"
)

target_include_directories(insectum-utils PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")