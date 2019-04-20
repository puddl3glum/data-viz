import numpy as np

class Transformation:
    def __init__(self, *args, **kwargs):
        if len(args) > 0:
            self.matrix = np.array(*args, **kwargs)
        else:
            self.matrix = np.array( (((1), (0), (0), (0)),
                                     ((0), (1), (0), (0)),
                                     ((0), (0), (1), (0)),
                                     ((0), (0), (0), (1))) )

    def set_transform(self, v: np.array):
        self.matrix[0][3] = v[0]
        self.matrix[1][3] = v[1]
        self.matrix[2][3] = v[2]

    def get_transform(self, v: np.array):
        return self.matrix[0][3]
        return self.matrix[1][3]
        return self.matrix[2][3]

    def multiply_with_vector(self, other):
        transform = self.matrix.dot( np.array((other[0], other[1], other[2], 1)) )
        return transform[:3]

    def __mul__(self, other):
        return Transformation(np.matmul(self.matrix, other.matrix))

    def __rmul__(self, other):
        return Transformation(np.matmul(self.matrix, other.matrix))

    def __str__(self):
        return str(self.matrix)
