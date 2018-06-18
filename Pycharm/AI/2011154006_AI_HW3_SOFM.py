import numpy as np
import matplotlib.pyplot as plt

Dim = 2  #입력 차원
N = 1000 #학습데이터수
X = 2*np.random.rand(N,Dim)-1 #입력데이터 생성
INP = X.shape[1] #INP = 2, 입력데이터
R = 10 #입력 뉴런수
C = 10 #입력 뉴런수
OUT = R*C #출력 뉴런수
w = np.random.rand(INP,OUT)*0.6-0.3
eta = 0.5 #학습률
sig = 1 #이웃소속도
mStep = 10 #반복횟수

plt.scatter(X[:,0],X[:,1])
plt.plot(w[0, :], w[1, :], 'ko--')
plt.show()

def som_alpha(i, j, sig, NumC):
    x1 = np.zeros((1, 2))
    x2 = np.zeros((1, 2))

    x1[0, 0] = np.floor(i / NumC)
    x1[0, 1] = np.mod(i, NumC)
    x2[0, 0] = np.floor(j / NumC)
    x2[0, 1] = np.mod(j, NumC)

    r = np.dot((x1 - x2), (x1 - x2).T)
    return np.exp(-r / (2 * pow(sig, 2))) * (1 / (np.sqrt(2 * np.pi) * sig))

for j in range(2,mStep,1): #반복학습시작
    for i in range(0,N,1) : #각 데이터에 대한 학습 시작
        x = X[i,:] # 입력 데이터 선택
        dist = np.zeros((1,OUT))
        for k in range(0,OUT,1) : # 출력 뉴런 가중치와의 거리계산
            dist[0,k] = np.dot((x-np.array([w[:,k]])),(x-np.array(w[:,k])).T) # np.array([w[:,k]])는 w[:,k]의 값으로 이루어진 배열
        wini = np.argmin(dist) # 승자뉴런 선정, argmin : 제일 작은 값에 인덱스 반환
        alpha = np.zeros((1,OUT))
        dw = np.zeros((INP,OUT))
        for k in range(0,OUT,1) :
            alpha[0,k] = som_alpha(wini,k,sig,C) #소속도 계산
            dw[:,k] = (x-np.array([w[:,k]]))*alpha[0,k] #가중치 수정항 계산

        w = w+eta*dw # 가중치 수정
        sum_dw = np.zeros((1,N))
        sum_dw[0,i]=np.trace(np.dot(dw.T,dw)) # 가중치 변화량 계산

    sig = sig*0.9 #이웃소속도 파라미터 감소
    if np.mean(sum_dw) < 0.001 : #가중치 변화 없을 시 학습완료
        j = mStep+1
'''
for i in range(0,N,1) :
    x = X[i,:]
    dist = np.zeros((1,OUT))
    for k in range(0,OUT,1) :
        dist[0,k] = np.dot((x-np.array([w[:,k]])),(x-np.array(w[:,k])).T)
    wini = np.argmin(dist)
'''

plt.scatter(X[:,0],X[:,1])
plt.plot(w[0, :], w[1, :], 'ko--')
plt.show()