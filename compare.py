from PIL import Image
import numpy as np

def compare(img: str, cmp: str):
    x1 = np.load(cmp)
    x = np.array(Image.open(img))
    print(x.shape)
    print(x1.shape)
    np.testing.assert_allclose(x, x1)

compare("./dog.png", "./png.npy")
# compare("./dog.jpg", "./jpeg.npy")