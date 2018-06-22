import numpy as np
import matplotlib.pyplot as plt

data = np.array([[0.1875, 0.75, 0.625, 0.0625, 0.9375, 0.4375, 0.75, 0.625, 1, 0, 0.25, 0.75, 0.875, 0.375, 0.625, 0.6875, 0.375, 0.9375, 0.75, 0.0625],
                 [0.5, 1, 1, 0.4375, 0, 0.5, 0.5625, 0, 0, 0.5, 0.5, 0, 0.25, 1, 0.5, 0, 0, 0.375, 0.4375, 0],
                 [0.3125, 0.5, 0.5, 0.5, 0.1875, 0.9375, 0.9375, 0.4375, 0.1875, 0.125, 0.8125, 0.4375, 0.8125, 0.6875, 0.75, 0.5, 0.0625, 0.5625, 0.75, 0.125],
                 [0.4375, 0.75, 0.75, 0.4375, 0.0625, 0.5, 0.625, 0.75, 0.0625, 0.6875, 0.875, 0.5625, 0.3125, 0.3125, 0.6875, 0.4375, 0.375, 0.75, 0.6875, 0.0625],
                 [0.0625, 0.4375, 0.5, 0.1875, 0.6875, 0.75, 0.625, 0.75, 0.75, 0.375, 0.375, 0.6875, 0.1875, 0.875, 0.9375, 0.375, 0, 0.1875, 0.75, 0.125],
                 [0, 0.75, 0.25, 0, 0, 0.625, 0.4375, 0, 0, 0.5, 0.5, 0, 0, 0.5, 0.5625, 0, 0, 0.1875, 0.625, 0],
                 [0.1875, 1, 1, 0.875, 0.0625, 0.8125, 0.75, 0, 0.4375, 0.0625, 0.875, 0.25, 0.75, 0.625, 0.8125, 0.25, 0.0625, 0.6875, 0.9375, 0.125],
                 [0.6875, 0.875, 1, 0.8125, 0.5625, 0.1875, 0.0625, 1, 0, 0.4375, 0.75, 0.75, 0.1875, 0.625, 1, 0.75, 0.5625, 0.75, 0.75, 0.5625],
                 [0.1875, 0.9375, 1, 0.6875, 0.6875, 0.4375, 0.25, 0.75, 0.75, 0.25, 0.25, 0.875, 0.6875, 0.625, 0.5, 0.8125, 0.0625, 0.6875, 0.6875, 0.1875],
                 [0, 0.25, 1, 0.25, 0, 0, 1, 0.25, 0, 0.125, 1, 0.25, 0.25, 0.9375, 1, 0.25, 0, 0.3125, 0.9375, 0.25],
                 [0.375, 1, 1, 1, 0, 0.8125, 0.5, 0, 0.1875, 0.1875, 0.9375, 0.125, 0.75, 0.625, 0.75, 0.25, 0.0625, 0.75, 0.9375, 0.0625],
                 [0.75, 0.8125, 0.8125, 0.9375, 0.0625, 0.25, 0.3125, 1, 0.25 , 1, 1, 0.375, 0.625, 0.9375, 1, 0.875, 0.125, 0.25, 0.25, 0.1875],
                 [0.5, 0.9375, 1, 0.375, 0.9375, 0.1875, 0.1875, 0.9375, 0.8125, 0, 0, 1, 0.875, 0.375, 0.1875, 0.9375, 0.1875, 0.8125, 1, 0.5],
                 [0.25, 1, 1, 0, 0.25, 1, 1, 0, 0.25, 1, 1, 0, 0.25, 1, 1, 0, 0.25, 1, 1, 0],
                 [0.6875, 1, 1, 1, 0.375, 0.9375, 0.4375, 0, 0.1875, 0.25, 0.8125, 0.375, 0.8125, 0.6875, 0.625, 0.5, 0.0625, 0.5625, 0.75, 0.1875],
                 [0.6875, 0.8125, 0.8125, 0.75, 0.375, 0.125, 0.1875, 1, 0.0625, 0.75, 0.8125, 0.6875, 0.0625, 0.5625, 1, 0.9375, 0.1875, 0.75, 0.75, 0.75],
                 [0.5, 0.875, 0.875, 0.625, 1, 0.125, 0.25, 0.75, 0.75, 0, 0.5, 0.6875, 0.875, 0.25, 0.9375, 0.3125, 0.5625, 0.75, 0.5625, 0],
                 [0.5625, 1, 1, 0.6875, 0, 0, 1, 0, 0.0625, 0.375, 0.875, 0, 0.1875, 0.9375, 0.75, 0, 0, 0.5, 0.5625, 0],
                 [0.6875, 1, 1, 0.875, 0.1875, 0.9375, 0.25, 0.125, 0, 0.1875, 0.9375, 0.125, 0.875, 0.4375, 0.75, 0.25, 0.1875, 0.8125, 0.875, 0.125],
                 [0.8125, 1, 0.9375, 0.5, 0.25, 0.125, 0.5625, 0.75, 0, 0.5625, 1, 0.6875, 0.6875, 0.1875, 0.375, 0.75, 0.375, 1, 0.9375, 0.3125],
                 [0.0625, 0.875, 1, 0.3125, 0.625, 0.6875, 0.3125, 0.75, 0.75, 0.25, 0.25, 0.75, 0.625, 0.625, 0.3125, 0.75, 0.0625, 0.8125, 1, 0.375],
                 [0, 0.75, 1, 0.5, 0, 0.75, 1, 0.5, 0, 0.75, 1, 0.5, 0, 0.75, 1, 0.5, 0, 0.75, 1, 0.5],
                 [0.75, 1, 0.75, 0.8125, 0.0625, 0.8125, 0.625, 0, 0, 0, 0.875, 0.25, 0.5, 0.4375, 1, 0.0625, 0.1875, 0.75, 0.5625, 0],
                 [0.8125, 0.8125, 0.9375, 0.625, 0.25, 0, 0.0625, 1, 0.0625, 0.75, 0.5625, 0.875, 0, 0.4375, 1, 0.8125, 0.5, 0.9375, 0.75, 0.75],
                 [0.375, 0.9375, 0.9375, 0.3125, 1, 0.25, 0.4375, 0.6875, 0.8125, 0.375, 0.25, 0.75, 0.3125, 0.875, 0.8125, 0.625, 0, 0.375, 0.75, 0.3125],
                 [0.1875, 0.9375, 0, 0, 0.1875, 0.9375, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0.625, 0.1875, 0],
                 [0.5, 0.75, 0.75, 0.75, 0.375, 0.875, 0.75, 0.25, 0, 0.0625, 0.9375, 0.125, 0.625, 0.3125, 0.8125, 0.25, 0.1875, 0.875, 0.9375, 0.0625],
                 [0.8125, 1, 0.875, 0.1875, 0.5, 0, 0.4375, 0.9375, 0, 0.625, 0.625, 1, 0.5, 0.6875, 1, 0.6875, 0.4375, 0.5, 0.5, 0.3125],
                 [0.1875, 0.9375, 1, 0.8125, 0.6875, 0.5, 0, 0.875, 0.75, 0.375, 0, 0.875, 0.1875, 0.875, 0.8125, 0.8125, 0, 0.1875, 0.4375, 0.0625],
                 [0, 0, 0.875, 0.625, 0, 0.1875, 1, 0.375, 0, 0.5, 1, 0.125, 0, 0.8125, 0.875, 0, 0.125, 1, 0.625, 0],
                 [0.5, 0.75, 0.75, 0.5625, 0.375, 0.9375, 0.625, 0.125, 0, 0.1875, 1, 0, 0.875, 0.5, 0.9375, 0, 0.1875, 0.75, 0.375, 0],
                 [0.8125, 1, 0.75, 0.1875, 0.75, 0.0625, 0.5625, 0.75, 0, 0.75, 1, 0.5, 0.4375, 0.25, 0.6875, 0.5, 0.375, 0.875, 1, 0.4375],
                 [0.25, 1, 1, 0.625, 0.6875, 0.625, 0.1875, 0.875, 0.6875, 0.4375, 0, 0.75, 0.375, 0.875, 0.125, 0.875, 0, 0.625, 1, 0.75],
                 [0.125, 1, 0, 0, 0, 1, 0, 0, 0, 0.8125, 0.25, 0, 0, 0.75, 0.25, 0, 0, 0.375, 0.125, 0],
                 [0.5, 0.75, 0.6875, 0.5, 0.5625, 0.9375, 0.625, 0.5, 0, 0.5625, 0.875, 0, 0.6875, 0.3125, 1, 0, 0.375, 0.875, 0.9375, 0],
                 [0.1875, 0.6875, 0.75, 0.375, 0.3125, 0.625, 0.625, 0.75, 0, 0.5, 1, 0.9375, 0.5, 0.375, 0.3125, 1, 0.1875, 0.875, 1, 0.75],
                 [0.0625, 0.8125, 1, 0.4375, 0.625, 0.625, 0.375, 0.75, 0.75, 0.3125, 0.25, 0.75, 0.5, 0.8125, 0.6875, 0.625, 0, 0.625, 0.8125, 0.0625],
                 [0, 0.4375, 0.5, 0, 0, 0.5, 0.5, 0, 0, 0.5, 0.5, 0, 0.5, 1, 0.5, 0, 0, 0.5625, 0.4375, 0],
                 [0.875, 0.875, 0.75, 0.875, 0.1875, 0.9375, 0.375, 0, 0, 0.25, 0.9375, 0, 0.5, 0.25, 1, 0, 0.5, 0.875, 0.625, 0],
                 [0.3125, 1, 0.9375, 0.375, 0.1875, 0.3125, 0.25, 0.9375, 0, 0.875, 0.875, 0.875, 0.4375, 0.5, 0.875, 0.3125, 0.3125, 0.75, 0.6875, 0.0625]
])

def sigmoidFunc (totalInput) :
    return 1.0/(np.ones(totalInput.shape)+np.exp(-1.0*totalInput))

def outputDeltas (output, target) :
    sigmoidDeriv = output * (np.ones(output.shape) - output)
    return 2*(target-output) * sigmoidDeriv

def hiddenDeltas (outputDeltas, hiddenOutputs, outputWeights) :
    sigmoidDeriv = hiddenOutputs * (np.ones(hiddenOutputs.shape) - hiddenOutputs)
    return (np.dot(outputWeights.T, outputDeltas.T)) * sigmoidDeriv

patterns = data.T
nPats = data.shape[0] # data의 행 개수 (40)
nTrainingPats = 20
nTestPats = 20
nInputs = data.shape[1] # data의 열 개수 (20)
nHidden = 10
nOutputs = 4

# Hidden, Output 가중치 랜덤하게 초기화
hiddenWeights = 0.5 * (np.random.rand(nHidden, nInputs+1) - np.ones((nHidden, nInputs+1)) * 0.5)
outputWeights = 0.5 * (np.random.rand(nOutputs, nHidden+1) - np.ones((nOutputs, nHidden+1)) * 0.5)
input = patterns
target = np.zeros((nOutputs, nPats))
classNum = 0
eta = 0.1
nEpochs = 1000  # 반복횟수
totalNEpochs = 0

for pat in range (0,nPats,1) :
    target[classNum, pat] = 1
    classNum = classNum + 1
    if classNum >= nOutputs :
        classNum = 0

ErrorsLastNEpochs = np.zeros((1,nEpochs))
TestErrorLastNEpochs = np.zeros((1,nEpochs))

for epoch in range (0, nEpochs, 1) :
    sumSqrError = 0.0
    sumSqrTestError = 0.0
    outputWGrad = np.zeros(outputWeights.shape)
    hiddenWGrad = np.zeros(hiddenWeights.shape)

    for pat in range (0, 40, 1) :
        # hstack - n by p 배열과 n by k 배열, 두 배열이 있으면 n by k+p 인 배열로 만듦, 즉 행의 수가 같은 두 배열을 옆으로 연결
        # input[:,pat] = 모든 행의 pat번째 열을 리스트로, 즉 각 열을 리스트로 뽑음
        inp = np.hstack([input[:,pat], np.array([1])])
        hiddenNetInputs = np.dot(hiddenWeights, inp)
        hiddenStates = sigmoidFunc(hiddenNetInputs)
        hidStatesBias = np.hstack([hiddenStates, np.array([1])])
        outputNetInputs = np.dot(outputWeights, hidStatesBias)
        outputStates = sigmoidFunc(outputNetInputs)

        targetStates = target[:,pat]
        error = outputStates - targetStates
        sumSqrError = sumSqrError + np.dot(error, error)

        outputDel = outputDeltas(outputStates, targetStates)
        outputWGrad = outputWGrad + np.dot(np.array([outputDel]).T, np.array([hidStatesBias]))
        hiddenDel = hiddenDeltas(outputDel, hidStatesBias, outputWeights)
        hiddenDelArray = np.array([hiddenDel])
        hiddenWGrad = hiddenWGrad + np.dot(hiddenDelArray[:,0:nHidden].T, np.array([inp]))

    outputWChange = eta * outputWGrad
    outputWeights = outputWeights + outputWChange
    hiddenWChange = eta * hiddenWGrad
    hiddenWeights = hiddenWeights + hiddenWChange

    for pat in range(nTrainingPats+1, nPats, 1) :
        inp = np.hstack([input[:, pat], np.array([1])])
        hiddenNetInputs = np.dot(hiddenWeights, inp)
        hiddenStates = sigmoidFunc(hiddenNetInputs)
        hidStatesBias = np.hstack([hiddenStates, np.array([1])])
        outputNetInputs = np.dot(outputWeights, hidStatesBias)
        outputStates = sigmoidFunc(outputNetInputs)

        targetStates = target[:, pat]
        error = outputStates - targetStates
        sumSqrTestError = sumSqrTestError + np.dot(error, error)

    #gradsize = np.linalg.norm(np.vstack([hiddenWGrad[:],outputWGrad[:]]))
    totalNEpochs = totalNEpochs + 1
