#pragma once

#include <mlx/array.h>
#include <mlx/ops.h>

mlx::core::array load_png(const std::string &path);
mlx::core::array load_jpeg(const std::string& path);
