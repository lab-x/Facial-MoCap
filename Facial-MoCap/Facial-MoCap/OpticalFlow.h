#pragma once
#include "opencv\cv.h"

using namespace cv;

class OpticalFlow
{
public:
	OpticalFlow();
	~OpticalFlow();

	void Init(Mat image);

private:
	Mat image_prev, image_next;
	std::vector<Point> features_prev, features_next;
};

