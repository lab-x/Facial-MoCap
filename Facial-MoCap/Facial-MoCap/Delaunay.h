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
	Delaunay();
	//This constructor calls the mean mask function during construction
	Delaunay(string filePath);
	~Delaunay();

	//Draws the lines connecting the subdivision's points together onto the
	//provided img matrix.
	void drawSubdiv(Mat& img, Subdiv2D& subdiv);

	//Locates the given point in the given matrix
	void locatePoint(Mat& img, Point2f fp, Subdiv2D& subdiv);

	//Draws the Delaunay mask onto the face provided with the
	//number of features given.
	void drawMask(Mat& img, int numFeatures);

	//Draw the point of the subdivision onto the image
	void drawSubdivPoint(Mat& img, Point2f fp);

	//Paints the inner portion of the triangles
	void paintVoronoi(Mat& img, Subdiv2D& subdiv);

	/*
	Creates the mean mask shape onto which every other
	recognized shape is projected onto with Affine transforms.
	Takes a filepath pointing to the training image one wishes to
	use for this purpose. Highly suggest using constructor
	*/
	void calcMeanMask(string filePath);

	vector<Point2f[3]>* Delaunay::makeTrianglePointsVector(Subdiv2D& subdiv);

	//Warp to meanMask model
	void warpTextureFromTriangles(Subdiv2D & srcSubDiv, Mat& originalImg, Mat& warp_final);

	//Creates a subdivision based on annotation data
	static Subdiv2D findSubdiv(string filePath);

//Static Sample Delaunay functionality demos
	//Provides a quick overview of the Delaunay functions when run.
	static void help();

	//Runs a sample of Delaunay triangulation by inserting random points into the subdivision
	static void runSample();

	//Runs a sample of what a face looks like with the mask overlaid
	static void drawSample(string filePath);

	//Guidelines to follow when warping
	static void warpSample(string meanLoc, string testLoc);

private:
	//The colors that the items are colored with
	Scalar activeColor;
	Scalar delaunayColor;

	//Provides the triangle points of the mean mask for simpler transform
	vector<Point2f[3]> *meanMask;
};