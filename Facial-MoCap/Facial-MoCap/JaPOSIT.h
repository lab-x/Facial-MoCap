#pragma once
#include "opencv2\core.hpp"
#include "FaceModel.h"

using std::vector;
using cv::Point2f;
using cv::Vec2i;

class JaPOSIT
{
public:
	JaPOSIT();
	~JaPOSIT();

	//This function will load a pointer to the input FaceModel object
	void loadFaceModel(FaceModel* in);

	//This function maps the point indices from the training matrix to the
	// 3D anthromorphic face model
	void mapPoints(vector<Point2f> imgFeatures);

	FaceModel* faceModel;

private:
	//Holds the map data between the image space and 3D space
	vector<Vec2i> map;
	
};