#pragma once

#include "opencv\highgui.h"
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
