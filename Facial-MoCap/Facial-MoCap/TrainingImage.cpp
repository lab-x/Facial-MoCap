#include "TrainingImage.h"



TrainingImage::TrainingImage()
{
}

TrainingImage::TrainingImage(string filePath)
{
#if defined(_DEBUG)
	assert(!loadImg(&filePath));
	assert(!loadAnnotations(&filePath));
#else
	if (!loadImg(&filePath))
		throw std::exception("That image could not be read!");
	if (!loadAnnotations(&filePath))
		throw std::exception("The annotation could not be read!");
#endif
}


TrainingImage::~TrainingImage()
{
}

bool TrainingImage::loadImg(string* filePath)
{
	img = cv::imread(*filePath + ".jpg", 1);
	if (img.empty())
		return false;
}

bool TrainingImage::loadAnnotations(string * filePath)
{
	std::ifstream in(*filePath + ".asf");
	if (in.bad())
		return false;
	int pointNum;
	std::string line;
	while (std::getline(in, line))
	{
		if (line[0] != '#' && line != "")
		{
			std::istringstream iss(line);
			iss >> pointNum;
			break;
		}
	}
	int tmp = 0;
	for (unsigned i = 0; i < pointNum; i++)
	{
		while (std::getline(in, line))
		{
			if (line[0] != '#' && line != "")
				break;
		}
		std::istringstream iss(line);
		Point2f point;
		iss >> tmp;
		iss >> tmp;
		iss >> point.x;
		point.x *= img.cols;
		iss >> point.y;
		point.y *= img.rows;
	}
}
