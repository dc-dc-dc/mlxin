import mlx.core as mx
import mlxin
y = mx.ones([100, 100, 3], dtype=mx.uint8)

x = mlxin.load_png("./dog.png")
# print(x.tolist()[0])

mlxin.save_png("./dog2.png", mx.ones([100, 100, 3], dtype=mx.uint8))