from random import uniform
import numpy as np
import matplotlib.pyplot as plt

Dim = 2  #입력 차원
N = 1000 #학습데이터수

X = 2*np.random.rand(N,Dim)-1 #입력데이터 생성

for i in range(0,N,1) :
    x = X[i,:]
    plt.scatter(x[0],x[1],c='b')
plt.show()