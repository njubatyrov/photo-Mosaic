#include <iostream>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <time.h>

#include <opencv2/opencv.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

/** Global variables */
int M;
int N;
int patchSize = 30;
int patchCnt = 381;
int patchSmallSize = 10;
int patchSmallCnt = 436;

Mat resultImage;
Mat inputImage;

vector < Mat > patch, patchSmall;
vector < Scalar > patchMean;

CascadeClassifier face_cascade;

String face_cascade_name = "/Users/nursultan/Documents/opencv-2.4.13/data/haarcascades/haarcascade_frontalface_alt.xml";

void printImage(Mat& image) {
	namedWindow("Display Image", WINDOW_AUTOSIZE );
    imshow("Display Image", image);
    waitKey(0);
}

void readPatches() {
	for(int i = 1; i <= patchCnt; i++) {
		string fileName = "Assets/datas/b";
		fileName += to_string(i) + ".jpg";
		Mat image = imread(fileName, 1);
		patch.push_back(image);
		// patchMean.push_back(mean(image));
	}

	for(int i = 1; i <= patchSmallCnt; i++) {
		string fileName = "Assets/datas/s";
		fileName += to_string(i) + ".jpg";
		Mat image = imread(fileName, 1);
		patchSmall.push_back(image);
		patchMean.push_back(mean(image));
	}
}

void update(Rect area) {
	Mat image = inputImage(area);
	Scalar meanVal = mean(image);
	double minVal = 100000.0;
	int minPos = 0;
	for(int i = 0; i < patchSmall.size(); i++) {
		double first = patchMean[i].val[0] - meanVal.val[0];
		double second = patchMean[i].val[1] - meanVal.val[1];
		double third = patchMean[i].val[2] - meanVal.val[2];
		if(sqrt(first * first + second * second + third * third) < minVal) {
			minVal = sqrt(first * first + second * second + third * third);
			minPos = i;
		}
	}
	image = resultImage(area);
	patchSmall[minPos].copyTo(image);
}

void detectFaces() {
	if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face cascade\n"); return; };
	vector<Rect> faces;
	Mat frame_gray;
	cvtColor( inputImage, frame_gray, CV_BGR2GRAY );
  	equalizeHist( frame_gray, frame_gray );
	face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
	for( size_t i = 0; i < faces.size(); i++ ) {
	    Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
	    // ellipse( inputImage, center, Size( faces[i].width*0.7, faces[i].height*0.7), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );	    
   	}
   	Point center( faces[0].x + faces[0].width*0.5, faces[0].y + faces[0].height*0.5 );
   	for(int i = 0; i < N; i += patchSmallSize) {
   		for(int j = 0; j < M; j += patchSmallSize) {
   			if(sqrt((center.x - i - patchSmallSize) * (center.x - i - patchSmallSize) + (center.y - j - patchSmallSize) * (center.y - j - patchSmallSize)) <= faces[0].width * 0.7) {
   				update(Rect(i, j, patchSmallSize, patchSmallSize));
   			}
   		}
   	}
   	// printImage(inputImage);
}

int main(int argc, char** argv) {

	srand(time(NULL));

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
    		int foo = rand() % patchCnt;
    		patch[foo].copyTo(tmp);
    	}
    }
   	detectFaces();
    double alpha = 0.55;
    double beta = 1 - alpha;
    addWeighted(resultImage, alpha, inputImage, beta, 0.0, resultImage);
   	printImage(resultImage);
    
	return 0;
}