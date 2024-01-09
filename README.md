# mlxin

Load data directly into mlx

```python
import mlx.core as mx
import mlxin

x = mlxin.load_jpeg("./image.jpg")
print(x.shape)
# [2117, 3176, 3]
```

## Supports

### Images
- PNG
- JPEG

## Installation

```shell
python -m pip install .
```