#include <iostream>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	if(argc != 2) {
		printf("usage: createDataset <Video Path>\n");
		return -1;
	}
	VideoCapture cap(argv[1]);
	if(!cap.isOpened()) {
		printf("No video data\n");
		return -1;
	}

	Mat frame;
	int counter = 1;

	while(true) {
		for(int i = 0; i < 6; i++) {
			cap.read(frame);
		}
		if(frame.data) {
			string filePath = "s";
			filePath += to_string(counter) + ".jpg";
			counter++;
			resize(frame, frame, Size(10, 10));
			imwrite(filePath, frame);
			namedWindow("Display Image", WINDOW_AUTOSIZE );
    		imshow("Display Image", frame);
    		waitKey(1);
		}
		if(counter > 500) {
			break;
		}
	}
	return 0;
}