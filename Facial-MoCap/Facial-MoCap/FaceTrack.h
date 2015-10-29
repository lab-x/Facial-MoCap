#pragma once
#include "opencv\cv.h"
#include "opencv2\imgproc\imgproc.hpp"

using namespace cv;

class FaceTrack
{
public:
	FaceTrack();
	~FaceTrack();

	bool HaarFaceDetect(Mat* frame);
private:
};