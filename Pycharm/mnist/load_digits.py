import os
import struct
import numpy as np
import matplotlib.pyplot as plt

def load_mnist (path, kind='train') :
    labels_path = os.path.join(path, '%s-labels.idx1-ubyte' %kind)
    images_path = os.path.join(path, '%s-images.idx3-ubyte' %kind)


    with open (labels_path, 'rb') as lbpath :
        magic, n = struct.unpack('>ll', lbpath.read(8))
        labels = np.fromfile(lbpath, dtype=np.uint8)

    with open (images_path, 'rb') as imgpath :
        magic, num, rows, cols = struct.unpack('>llll', imgpath.read(16))
        images = np.fromfile(imgpath, dtype=np.uint8).reshape(len(labels), 784)

    return images, labels

X_train, y_train = load_mnist('./data', kind='train')
print('학습 샘플수 \t: %d , 컬럼수 : %d'  %(X_train.shape[0], X_train.shape[1]))
X_test, y_test = load_mnist('./data', kind='t10k')
print('테스트 샘플수\t: %d, 컬럼수 : %d' %(X_test.shape[0], X_test.shape[1]))

fig, ax = plt.subplots(nrows=5, ncols=5, sharex=True, sharey=True)
ax = ax.ravel()

for i in range(25) :
    img = X_train[y_train==6][i].reshape(28,28)
    ax[i].imshow(img, cmap='Greys', interpolation='nearest')

ax[0].set_xticks([])
ax[0].set_yticks([])
plt.tight_layout()
plt.show()