#pragma once
#include "opencv2\core.hpp"
#include "opencv2\calib3d.hpp"
#include "opencv2\imgproc.hpp"
#include "FaceModel.h"

using std::vector;
using cv::Point2f;
using cv::Vec2i;
using cv::Point3f;
using cv::Mat_;
using cv::solvePnP;
using cv::Matx34d;

class JaPOSIT
{
public:
	JaPOSIT();
	~JaPOSIT();

	//This function will load a pointer to the input FaceModel object
	void init(FaceModel* in);

	//This function maps the point indices from the training matrix to the
	// 3D anthromorphic face model
	void mapPoints(vector<Point2f> imgFeatures);

	//The 3D model being used to show 3D pose
	FaceModel* faceModel;

	//Loads the PnP with the relevant points
	void loadWithPoints(vector<Point2f> imgPoints, Mat& img);

	//Retrieve a matrix for use in OpenGL
	double* getOpenGLMatrix();

private:
	//Holds the map data between the image space and 3D space
	vector<Vec2i> map;

	Mat camMatrix;
	Mat op;

	vector<double> rv, tv;

	Mat rvec, tvec;

	double rot[9] = { 0 };
};