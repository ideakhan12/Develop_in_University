/*
코드 제목 : 인공신경망 알고리즘을 활용하여 1개의 노드 학습
코드 세부 내용 : Input값이 1일때 Output 값이 5가 되도록 1개의 노드를 교사학습
*/

#include <stdio.h>
#include <math.h>

#define INPUT 1
#define BIAS 1
#define TARGET_OUTPUT 5

int main() {
	double weight = 1, alpha = 0.1, sigma, y;
	int i = 1;

	printf("-----------------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf(" Is training Success?            output              target_output        Training Rate               Weight                       Error             \n");

	// Runnig 100 Times for each Training Rate
	while (alpha <= 0.9) {
		printf("-----------------------------------------------------------------------------------------------------------------------------------------------------\n");
		while (i <= 100) {

			// SIGMA function
			sigma = INPUT * weight + BIAS;

			// Activation function
			y = sigma;

			if (y == TARGET_OUTPUT) {
				printf("          Yes            %.15e             %d                %lf           %.15e       %.15e\n", y, TARGET_OUTPUT, alpha, weight, pow(TARGET_OUTPUT - y, 2) / 2);
			}
			else {
				printf("          No             %.15e             %d                %lf           %.15e       %.15e\n", y, TARGET_OUTPUT, alpha, weight, pow(TARGET_OUTPUT - y, 2) / 2);
			}

			// Training weigiht
			weight = weight - alpha * (y - TARGET_OUTPUT);
			i++;
		}

		alpha += 0.1;
		i = 1;
		weight = 1;
	}
	printf("-----------------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf(" Is training Success?            output              target_output        Training Rate               Weight                       Error             \n");
	printf("-----------------------------------------------------------------------------------------------------------------------------------------------------\n");
	return 0;
}