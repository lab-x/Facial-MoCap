#include "TImage.h"


TImage::TImage()
{
}

TImage::TImage(string filePath)
{
#if defined(_DEBUG)
	assert(loadImg(&filePath));
	assert(loadFeaturePoints(&filePath));
#else
	if (!loadImg(&filePath))
		throw std::exception("The image " + filePath + " could not be read!");
	if (!loadFeaturePoints(&filePath))
		throw std::exception("The annotation " + filePath + " could not be read!");
#endif
}


TImage::~TImage()
{
}

vector<TImage*>* TImage::loadAllTraining(string filePath)
{
	vector<TImage*>* imgVector = new vector<TImage*>();
	//Load all images with their feature points returning a vector with all the TrainingImages
	// in the provided directory.
	std::ifstream trainingSet(filePath + "TrainingSet.txt");
	assert(!trainingSet.bad());
	std::string faceSet;
	while (std::getline(trainingSet, faceSet))
	{
		for (int i = 1; i <= 6; i++)
		{
			faceSet[3] = 0x30 + i;
			imgVector->push_back(new TImage(filePath + faceSet));
		}
	}
	return imgVector;
}

void TImage::deleteAllTraining(vector<TImage*>* imgs)
{
	for (unsigned i = 0; i < imgs->size(); i++)
		delete imgs->at(i);
	delete imgs;
}

Mat* const & TImage::getImg()
{
	return &img;
}

vector<Point2f>* const & TImage::getPoints()
{
	return &points;
}

int TImage::getRows()
{
	return img.rows;
}

int TImage::getCols()
{
	return img.cols;
}

bool TImage::loadImg(string* filePath)
{
	img = cv::imread(*filePath + ".jpg", 1);
	if (img.empty())
		return false;
	return true;
}

bool TImage::loadFeaturePoints(string * filePath)
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
	points = vector<Point2f>(pointNum);
	for (unsigned i = 0; i < points.size(); i++)
	{
		while (std::getline(in, line))
		{
			if (line[0] != '#' && line != "")
				break;
		}
		std::istringstream iss(line);
		iss >> pointNum;
		iss >> pointNum;
		iss >> points[i].x;
		points[i].x *= img.cols;
		iss >> points[i].y;
		points[i].y *= img.rows;
	}
	return true;
}