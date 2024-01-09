from PIL import Image
import numpy as np

import mlx.core as mx
import mlxin as mxi

def compare(img: str, cmp: mx.array):
    x = np.array(Image.open(img))
    np.testing.assert_allclose(x, cmp)

# wget -O dog.png https://static.vecteezy.com/system/resources/previews/018/871/732/original/cute-and-happy-dog-png.png
compare("./dog.png", mxi.load_png("./dog.png"))
# wget -O dog.jpg https://upload.wikimedia.org/wikipedia/commons/4/43/Cute_dog.jpg
compare("./dog.jpg", mxi.load_jpeg("./dog.jpg"))