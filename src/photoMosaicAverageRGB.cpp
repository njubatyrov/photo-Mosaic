#include <iostream>
#include <stdio.h>
#include <string>
#include <algorithm>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/** Global variables */
int M;
int N;
int patchSize = 20;
int patchCnt = 500;

Mat resultImage;
Mat inputImage;

vector < Mat > patch;
vector < Scalar > patchMean;

void printImage(Mat& image) {
	namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);
    waitKey(0);
}

void readPatches() {
	for(int i = 1; i <= patchCnt; i++) {
		string fileName = "Assets/datas/";
		fileName += to_string(i) + ".jpg";
		Mat image = imread(fileName, 1);
		patch.push_back(image);
		patchMean.push_back(mean(image));
	}
}

void update(Mat image) {
	Scalar meanVal = mean(image);
	double minVal = 100000.0;
	int minPos = 0;
	for(int i = 0; i < patch.size(); i++) {
		double first = patchMean[i].val[0] - meanVal.val[0];
		double second = patchMean[i].val[1] - meanVal.val[1];
		double third = patchMean[i].val[2] - meanVal.val[2];
		if(sqrt(first * first + second * second + third * third) < minVal) {
			minVal = sqrt(first * first + second * second + third * third);
			minPos = i;
		}
	}
	patch[minPos].copyTo(image);
}
int main(int argc, char** argv) {
	if(argc != 2) {
		printf("usage: photoMosaicAverageRGB <Image_Path>\n");
		return -1;
	}

    inputImage = imread( argv[1], 1 );

    if(!inputImage.data) {
        printf("No image data \n");
        return -1;
    }

    // Adjust image size to the patch size
    M = inputImage.rows;
	N = inputImage.cols;
    N = N - N % patchSize;
    M = M - M % patchSize;
    resize(inputImage, inputImage, Size(N, M));
    resultImage = inputImage.clone();
    
    // Reads all images from datas
    readPatches();

    for(int i = 0; i < N; i += patchSize) {
    	for(int j = 0; j < M; j += patchSize) {
    		Mat tmp = resultImage(Rect(i, j, patchSize, patchSize));
    		update(tmp);	
    	}
    }
    double alpha = 0.50;
    double beta = 1 - alpha;
    addWeighted(resultImage, alpha, inputImage, beta, 0.0, resultImage);
   	printImage(resultImage);
    
	return 0;
}