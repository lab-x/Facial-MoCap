#include "WebCam.h"

WebCam::WebCam(int camIndex)
{
	cap = VideoCapture(0); // open the video camera no. 0

	if (!cap.isOpened())  // if not success, exit program
	{
		throw "Could not connect to camera";
	}
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

	std::cout << "Frame size : " << dWidth << " x " << dHeight << std::endl;
}


WebCam::~WebCam()
{
}

Mat WebCam::getFrame()
{
	Mat frame;

	if (!cap.read(frame))
	{
		throw "Cannot read a frame from video stream";
	}
	return frame;
}
