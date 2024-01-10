#pragma once

#include <mlx/array.h>
#include <mlx/ops.h>

using namespace mlx::core;

// load png file into an mlx array, shape will be [height, width, 4]
array load_png(const std::string& path);

void save_png(const std::string& path, array img);

// load jpeg file into an mlx array, shape will be [height, width, 3]
array load_jpeg(const std::string& path);
