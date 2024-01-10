from PIL import Image
import numpy as np

import mlx.core as mx
import mlxin as mxi

def compare(x: np.ndarray, cmp: mx.array):
    np.testing.assert_allclose(x, cmp)

# wget -O dog.png https://static.vecteezy.com/system/resources/previews/018/871/732/original/cute-and-happy-dog-png.png
compare(np.array(Image.open("./dog.png")), mxi.load_png("./dog.png"))

# wget -O dog.jpg https://upload.wikimedia.org/wikipedia/commons/4/43/Cute_dog.jpg
compare(np.array(Image.open("./dog.jpg")), mxi.load_jpeg("./dog.jpg"))

x = mxi.load_png("./dog.png")
mxi.save_png("./dog2.png", x)
compare(mxi.load_png("./dog2.png"), mxi.load_png("./dog.png"))