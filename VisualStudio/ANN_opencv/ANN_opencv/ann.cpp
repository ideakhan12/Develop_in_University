/* 기존 만들어두었던 opencv 프로젝트에서 opencv 속성 값 가져오기
   새로 만든 프로젝트에서 속성관리자 -> win32 우클릭 기존 속성값 추가
   기존 프로젝트 opencv 속성값 추가
*/

#include "opencv2\opencv.hpp"

#define TRAININGDATA 500
#define TESTDATA 10000
#define IMAGESIZE 900

using namespace cv;
using namespace std;

void labelData(Mat& data, Mat classes);
void ANN(Mat& trainData, Mat& trainClasses, Mat& testData, Mat& testClasses);

int main() {
	Mat trainData = Mat(TRAININGDATA, 2, CV_32FC1);
	Mat testData = Mat(TESTDATA, 2, CV_32FC1);

	randu(trainData, 0, 1);
	randu(testData,  0, 1);

	Mat trainClasses = Mat(trainData.rows, 2, CV_32FC1);
	Mat testClasses = Mat(testData.rows, 2, CV_32FC1);

	labelData(trainData, trainClasses);

	ANN(trainData, trainClasses, testData, testClasses);

	return 0;
}

void labelData(Mat& data, Mat classes) {
	for (int i = 0; i < data.rows; i++) {
		float x = data.at<float>(i, 0);
		float y = data.at<float>(i, 1);

		if ((x < 0.5 && y < 0.5) || (x > 0.5 && y > 0.5)) {
			classes.at<float>(i, 0) = -1;
			classes.at<float>(i, 1) = -1;
		}
		else {
			classes.at<float>(i, 0) = 1;
			classes.at<float>(i, 1) = 1;
		}
	}
}

void ANN(Mat& trainData, Mat& trainClasses, Mat& testData, Mat& testClasses) {
	Mat layer = Mat(3, 1, CV_32SC1);

	layer.row(0) = Scalar(2);
	layer.row(1) = Scalar(10);
	layer.row(2) = Scalar(2);

	CvANN_MLP ann;
	CvANN_MLP_TrainParams param;
	CvTermCriteria criteria;
	criteria.max_iter = 500;
	criteria.epsilon = 0.00001f;
	criteria.type = CV_TERMCRIT_ITER | CV_TERMCRIT_EPS;

	param.train_method = CvANN_MLP_TrainParams::BACKPROP;
	param.bp_dw_scale = 0.1f;
	param.bp_moment_scale = 0.1f;
	param.term_crit = criteria;
	ann.create(layer);

	ann.train(trainData, trainClasses, Mat(), Mat(), param);

	Mat dstImage = Mat::zeros(Size(IMAGESIZE, IMAGESIZE), CV_8UC3);
	Mat output = Mat(1, 2, CV_32FC1);


	for (int i = 0; i < testData.rows; i++) {
		Mat temp = testData.row(i);

		ann.predict(temp, output);

		int x = temp.at<float>(0, 0) * IMAGESIZE;
		int y = temp.at<float>(0, 1) * IMAGESIZE;

		if (output.at<float>(0, 0) + output.at<float>(0,1) <  0)
			circle(dstImage, Point(x, y), 2, Scalar(0, 255, 0));
		else
			circle(dstImage, Point(x, y), 2, Scalar(0, 0, 255));
	}

	imshow("dstImage", dstImage);
	waitKey(0);
}