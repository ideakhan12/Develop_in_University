import numpy as np

data = np.array([[1,2,3,4,5],
                [6,7,8,9,10],
                [11,12,13,14,15],
                [16,17,18,19,20]])

data1 = np.array([[1,2],[3,4]])
data2 = np.array([[5,6],[7,8],[9,10]])

patterns = data.T
input = patterns

print(np.dot(data1,data2))