#pragma once

#include "opencv2\opencv.hpp"
#include <iostream>

using namespace cv;

class WebCam
{
public:
	WebCam(int camIndex);
	~WebCam();
	Mat getFrame();
private:
	VideoCapture cap;
};
