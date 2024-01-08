#include <stdio.h>

#include <mlx/ops.h>

#include "image.h"

int main(int argv, char** argc) {
    auto x = load_png("./dog.png");


    printf("shape: \n");
    for(int i = 0; i < x.ndim(); i++) {
        printf("%d ", x.shape()[i]);
    }
    printf("\n");
    if(x.dtype() == mlx::core::uint8) {
        printf("dtype: uint8\n");
    }
    mlx::core::save("png.npy", x);
    return 0;
}