/*	Code for this class was obtained from code.opencv.org
 *  Full URL: http://code.opencv.org/projects/opencv/repository/revisions/master/entry/samples/cpp/delaunay2.cpp
 *	Note: Slight modifications were made, either to update for OpenGL 3.0 or to make it easier for me to use
 */
#pragma once
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include "opencv\highgui.h"
#include "TImage.h"

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
	~Delaunay();

	//Draws the lines connecting the subdivision's points together onto the
	//provided img matrix.
	static void drawSubdiv(Mat& img, Subdiv2D& subdiv);

	//Locates the given point in the given matrix
	static void locatePoint(Mat& img, Point2f fp, Subdiv2D& subdiv);

	//Draws the Delaunay mask onto the face provided with the
	//number of features given.
	static void drawMask(Mat& img, int numFeatures);

	//Draw the point of the subdivision onto the image
	static void drawSubdivPoint(Mat& img, Point2f fp);

	//Paints the inner portion of the triangles
	static void paintVoronoi(Mat& img, Subdiv2D& subdiv);

	//Creates a subdivision based on annotation data
	static Subdiv2D findSubdiv(TImage* tImg);

	//Provides a quick overview of the Delaunay functions when run.
	static void help();

	//Runs a sample of Delaunay triangulation by inserting random points into the subdivision
	static void runSample();

	//Runs a sample of what a face looks like with the mask overlaid
	static void drawSample(string filePath);

private:
	//The colors that the items are colored with
	static Scalar activeColor;
	static Scalar delaunayColor;
};