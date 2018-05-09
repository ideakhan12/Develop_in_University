'''
제목 : SingleNeuron.py
주제 : 단일 뉴런 weight 및 bias 학습 및 그래프
'''

import matplotlib.pyplot as plt

training_rate_arr = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]

input, target = 1, 5

for training_rate in training_rate_arr :
    weight, bias = 1., 1.
    weight_arr = []
    error_arr = []
    print("*************** Trainig rate is %f ***************" %(training_rate))
    for i in range (1,101) :
        sigma = weight*input+bias
        output = sigma
        error = (target-output)**2*0.5
        if output==target :
            print("Success   %d times   output is %.15e   weight is %.15e   bias is %.15e   error rate is %.15e" %(i, output, weight, bias, error))
        else :
            print("Fail      %d times   output is %.15e   weight is %.15e   bias is %.15e   error rate is %.15e" %(i, output, weight, bias, error))

        weight_arr.append(weight)
        error_arr.append(error)

        weight = weight - training_rate*(output-target)
        bias = bias - training_rate*(output-target)

    plt.title("Training rate is %f" % training_rate)
    i = range(1, 101)
    plt.plot(i, weight_arr, color='blue', linestyle = '-', label='weight')
    plt.plot(i, error_arr, color='red', linestyle = '-', label = 'error rate')
    plt.legend(loc='upper right', frameon = False)
    plt.show()

