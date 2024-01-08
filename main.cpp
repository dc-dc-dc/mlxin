#include <stdio.h>

#include <mlx/ops.h>

#include "image.h"

int main(int argv, char** argc) {
    mlx::core::save("png.npy", load_png("./dog.png"));
    mlx::core::save("jpeg.npy", load_jpeg("./dog.jpg"));
    return 0;
}