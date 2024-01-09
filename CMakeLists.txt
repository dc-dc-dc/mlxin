cmake_minimum_required(VERSION 3.24)
project(mlxin LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
find_package(MLX CONFIG REQUIRED)
find_package(Python COMPONENTS Interpreter Development)
find_package(pybind11 CONFIG REQUIRED)

add_library(mlxin_ext)

target_sources(mlxin_ext PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/file.cpp
  ${CMAKE_CURRENT_LIST_DIR}/image.cpp
)

target_include_directories(mlxin_ext PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
)

# Set up the mlxin library
target_include_directories(mlxin_ext PUBLIC ${JPEG_INCLUDE_DIR})
target_include_directories(mlxin_ext PUBLIC ${PNG_INCLUDE_DIR})

target_link_libraries(mlxin_ext PUBLIC ${JPEG_LIBRARIES})
target_link_libraries(mlxin_ext PUBLIC ${PNG_LIBRARIES})



target_link_libraries(mlxin_ext PUBLIC ${MLX_LIBRARIES})

pybind11_add_module(mlxin
  ${CMAKE_CURRENT_LIST_DIR}/bindings.cpp
)

target_link_libraries(mlxin PRIVATE mlxin_ext)

if(BUILD_SHARED_LIBS)
  target_link_options(mlxin PRIVATE -Wl,-rpath,@loader_path)
endif()