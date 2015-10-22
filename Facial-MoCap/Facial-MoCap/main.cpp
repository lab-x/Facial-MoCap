#include "opencv\highgui.h"
int main()
{
	cvNamedWindow("P2", CV_WINDOW_AUTOSIZE);
	CvCapture* capture = cvCreateFileCapture("path to image");
	IplImage* frame;
	while (1)
	{
		frame = cvQueryFrame(capture);
		if (!frame) break; cvShowImage("P2", frame); char c = cvWaitKey(0); if (c == 27) break;
	}
	cvReleaseCapture(&capture); cvDestroyWindow("P2");
}