#pragma once
#include "opencv\cv.h"

using namespace cv;

class FaceTrack
{
public:
	FaceTrack();
	~FaceTrack();

	bool HaarFaceDetect(Mat* frame);
private:
};