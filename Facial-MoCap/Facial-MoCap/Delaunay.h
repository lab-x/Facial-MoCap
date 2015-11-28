/*	Code for this class was obtained from code.opencv.org
 *  Full URL: http://code.opencv.org/projects/opencv/repository/revisions/master/entry/samples/cpp/delaunay2.cpp
 *	Note: Slight modifications were made, either to update for OpenGL 3.0 or to make it easier for me to use
 */
#pragma once
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "opencv\highgui.h"

//usings
using std::vector;
using std::string;
using cv::Mat;
using cv::Point2f;
using cv::Scalar;
using cv::Subdiv2D;
using cv::Vec6f;

class Delaunay
{
public:
	Delaunay(string filePath);
	~Delaunay();

	void drawSubdiv(Mat& img, Subdiv2D& subdiv);
	void locatePoint(Mat& img, Point2f fp);
	void drawMask(Mat img, int numFeatures);
	void calcMeanMask(string filePath);
	//Draw the point of the subdivision onto the image
	void drawSubdivPoint(Mat& img, Point2f fp);
	static void paintVoronoi(Mat& img, Subdiv2D& subdiv);
	static void help();
	static void runSample();
	static void drawSample();

private:
	Scalar activeColor;
	Scalar delaunayColor;

	Subdiv2D

	vector<Vec6f> meanMask;
};