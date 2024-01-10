# mlxin

Load data directly into mlx

```python
import mlx.core as mx
import mlxin

# array([2117, 3176, 3], dtype=uint8)
jpg_img = mlxin.load_jpeg("./dog.jpg")

# array([1920, 1080, 4], dtype=uint8)
png_img = mlxin.load_png("./dog.png")

# array([2000, 10], dtype=float32)
csv_data = mlxin.load_csv("./dog_data.csv")

mlxin.save_csv("./dog_copy.csv", [...], csv_data)
mlxin.save_png("./test.png", mx.ones([256, 256, 3], dtype=mx.uint8))
```

## Supports

### Files
- CSV

### Images
- PNG
- JPEG

## Installation

```shell
python -m pip install .
```