#include "opencv\highgui.h"
#include "FaceTrack.h"
#include "WebCam.h"
#include <iostream>

using namespace cv;

int main(int argc, char* argv[])
{
	WebCam* cam = new WebCam(0);
	//Initialize capture of Webcam

	namedWindow("Face Tracker", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

#pragma region Inital Face Finding
	{
		//TODO: Find a face using Haar and then calculate good features to track on the face
		Mat firstFrame = cam->getFrame();
	}
#pragma endregion

	while (1)
	{
		//Where captured frame resides
		Mat frame = cam->getFrame();

		//show the frame in "Face Tracker" window
		imshow("Face Tracker", frame);

		//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		if (waitKey(30) == 27)
		{
			std::cout << "esc key is pressed by user" << std::endl;
			break;
		}
	}
	return 0;

}