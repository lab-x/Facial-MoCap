#pragma once
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using cv::Mat;
using cv::Point2f;
using cv::Subdiv2D;
using std::vector;
using std::string;

class TrainingImage
{
public:
	TrainingImage();
	TrainingImage(string filePath);
	~TrainingImage();

private:
	//Image loading function
	bool loadImg(string* filePath);

	//Annotation loading
	bool loadAnnotations(string* filePath);

	//Matrix that holds image data
	Mat img;

	//Vector of triangles in the image
	vector<Point2f*[3]> triangles;
};

