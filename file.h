#pragma once

#include <mlx/array.h>

using namespace mlx::core;

void save_csv(const std::string &path, std::vector<std::string> header, array& data);
// load csv file into an mlx array, shape will be [rows, cols]
// NOTE: currently this will only parse float32 values
array load_csv(const std::string& path);