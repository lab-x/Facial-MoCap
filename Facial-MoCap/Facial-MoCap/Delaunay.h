/*	Code for this class was obtained from code.opencv.org
 *  Full URL: http://code.opencv.org/projects/opencv/repository/revisions/master/entry/samples/cpp/delaunay2.cpp
 *	Note: Slight modifications were made, either to update for OpenGL 3.0 or to make it easier for me to use
 */
#pragma once
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv\highgui.h"

//usings
using cv::Mat;
using cv::Point2f;
using cv::Scalar;
using cv::Subdiv2D;

class Delaunay
{
public:
	Delaunay();
	~Delaunay();

	static void drawSubdivPoint(Mat& img, Point2f fp);
	static void drawSubdiv(Mat& img, Subdiv2D& subdiv);
	static void locatePoint(Mat& img, Subdiv2D& subdiv, Point2f fp);
	static void paintVoronoi(Mat& img, Subdiv2D& subdiv);
	static void help();
	static void runSample();

private:
	static Scalar activeColor;
	static Scalar delaunayColor;
};

