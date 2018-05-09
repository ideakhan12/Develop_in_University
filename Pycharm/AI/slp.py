'''
제목 : Single Layer Perceptron
주제 : SLP를 이용한 결정경계 찾기
'''

import matplotlib.pyplot as plt
import numpy as np

dataNum = 1000

trainData1 = np.random.randn(dataNum,2)
trainData2 = np.random.randn(dataNum,2) + 5
trainData = np.zeros((dataNum*2,2))

trainData[0:dataNum,:] = trainData1
trainData[dataNum:dataNum*2,:] = trainData2

trainout = np.zeros((dataNum*2,1))
trainout[dataNum:dataNum*2,:] = np.zeros((dataNum,1)) + 1

plt.plot(trainData1[:,0], trainData1[:,1],"*")
plt.plot(trainData2[:,0], trainData2[:,1],"*")

step = 100
input = 2; out = 1
w = np.random.rand(input, out); b = np.random.randn(1)
a = np.arange(-3,6,0.1)

eta = 0.5

for j in range (1, step, 1) :
    for i in range (0, dataNum*2, 1) :
        x = trainData[i]
        ry = trainout[i]
        if (np.dot(x,w)+b) > 0 :
            y = 1
        else :
            y = 0
        e = ry-y

        dw = eta * e * np.transpose([np.array(x)])
        db = eta*e*1

        w = w+ dw
        b = b+db

plt.plot(a,(-w[0,0]*a-b)/w[1,0],'r')

plt.show()