#include "FaceTrack.h"


FaceTrack::FaceTrack()
{
}

FaceTrack::~FaceTrack()
{
}

bool FaceTrack::HaarFaceDetect(Mat *frame)
{
	//Clone current frame
	Mat original = frame->clone();
	//Convert to grayscale
	Mat gray;
	cv::cvtColor(original, gray, CV_BGR2GRAY);
	//Find faces in frame
	std::vector< Rect_<int> > faces;
	return true;
}
