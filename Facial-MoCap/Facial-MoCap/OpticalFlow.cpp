#include "OpticalFlow.h"



OpticalFlow::OpticalFlow()
{
}


OpticalFlow::~OpticalFlow()
{
}

void OpticalFlow::Init(Mat image)
{
	image_next = image;
}

