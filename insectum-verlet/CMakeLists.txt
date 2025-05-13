add_library(insectum-verlet
    "src/cell_grid.cpp"
    "src/container.cpp"
    "src/object.cpp"
    "src/utils.cpp"
    "src/InsectumVerlet.hpp"
)

target_include_directories(insectum-verlet PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")