cmake_minimum_required(VERSION 3.8)
project(mlx_in LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mlx_in main.cpp)

target_sources(mlx_in PRIVATE 
  image.cpp
)


find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
find_package(MLX REQUIRED)
# Set up the mlx_in library
target_include_directories(mlx_in PUBLIC ${JPEG_INCLUDE_DIR})
target_include_directories(mlx_in PUBLIC ${PNG_INCLUDE_DIR})

target_link_libraries(mlx_in PUBLIC ${JPEG_LIBRARIES})
target_link_libraries(mlx_in PUBLIC ${PNG_LIBRARIES})

target_link_libraries(mlx_in PUBLIC ${MLX_LIBRARIES})

# Include src directory