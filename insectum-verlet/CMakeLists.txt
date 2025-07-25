set(CMAKE_CXX_STANDARD 20)
add_library(insectum-verlet
    "src/cell_grid.cpp"
    "src/container.cpp"
    "src/object.cpp"
    "src/utils.cpp"
    "src/InsectumVerlet.hpp"
)

target_include_directories(insectum-verlet PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_link_libraries(insectum-verlet PRIVATE SFML::Graphics
                                      PRIVATE insectum-utils)
