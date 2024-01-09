from PIL import Image
import numpy as np

import mlx.core as mx
import mlxin as mxi

def compare(img: str, cmp: mx.array):
    x = np.array(Image.open(img))
    np.testing.assert_allclose(x, cmp)

compare("./dog.png", mxi.load_png("./dog.png"))
compare("./dog.jpg", mxi.load_jpeg("./dog.jpg"))