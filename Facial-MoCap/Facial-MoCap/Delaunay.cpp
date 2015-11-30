#include "Delaunay.h"

Delaunay::Delaunay()
{
	delaunayColor = Scalar(255, 0, 0);
}

//Definition of statics
Delaunay::Delaunay(string filePath)
{
	calcMeanMask(filePath);
	delaunayColor = Scalar(255, 0, 0);
}


Delaunay::~Delaunay()
{
	delete meanMask;
}

//Draw the point in space onto the matrix representation of the image
void Delaunay::drawSubdivPoint(Mat & img, Point2f fp)
{
	cv::circle(img, fp, 3, activeColor, cv::FILLED, cv::LINE_8, 0);
}

void Delaunay::drawSubdiv(Mat & img, Subdiv2D & subdiv)
{
#if 1
	bool draw;
	std::vector<cv::Vec6f> triangleList;
	subdiv.getTriangleList(triangleList);
	std::vector<cv::Point> pt(3);

	for (size_t i = 0; i < triangleList.size(); i++)
	{
		cv::Vec6f t = triangleList[i];
		pt[0] = cv::Point(cvRound(t[0]), cvRound(t[1]));
		pt[1] = cv::Point(cvRound(t[2]), cvRound(t[3]));
		pt[2] = cv::Point(cvRound(t[4]), cvRound(t[5]));
		draw = true;

		for (size_t j = 0; j < 3; j++)
			if (pt[j].x>img.cols || pt[j].y>img.rows || pt[j].x < 0 || pt[j].y < 0)
				draw = false;
		if (draw)
		{
			//draw lines
			cv::line(img, pt[0], pt[1], delaunayColor, 1, cv::LINE_AA, 0);
			cv::line(img, pt[1], pt[2], delaunayColor, 1, cv::LINE_AA, 0);
			cv::line(img, pt[2], pt[0], delaunayColor, 1, cv::LINE_AA, 0);
		}
	}
#else

	std::vector<cv::Vec4f> edgeList;
	subdiv.getEdgeList(edgeList);
	for (size_t i = 0; i < edgeList.size(); i++)
	{
		cv::Vec4f e = edgeList[i];
		cv::Point pt0 = Point(cvRound(e[0]), cvRound(e[1]));
		cv::Point pt1 = Point(cvRound(e[2]), cvRound(e[3]));
		cv::line(img, pt0, pt1, delaunayColor, 1, cv::LINE_AA, 0);
	}
#endif
}

void Delaunay::locatePoint(Mat & img, Point2f fp, Subdiv2D & subdiv)
{
	int e0 = 0, vertex = 0;

	subdiv.locate(fp, e0, vertex);

	if (e0 > 0)
	{
		int e = e0;
		do 
		{
			Point2f org, dst;
			if (subdiv.edgeOrg(e, &org) > 0 && subdiv.edgeDst(e, &dst) > 0)
				cv::line(img, org, dst, activeColor, 3, cv::LINE_AA, 0);

			e = subdiv.getEdge(e, Subdiv2D::NEXT_AROUND_LEFT);
		} while (e != e0);
	}

	drawSubdivPoint(img, fp);
}

void Delaunay::paintVoronoi(Mat & img, Subdiv2D & subdiv)
{
	std::vector<std::vector<Point2f>> facets;
	std::vector<Point2f> centers;
	subdiv.getVoronoiFacetList(std::vector<int>(), facets, centers);

	std::vector<cv::Point> ifacet;
	std::vector<std::vector<cv::Point>> ifacets(1);

	for (size_t i = 0; i < facets.size(); i++)
	{
		ifacet.resize(facets[i].size());
		for (size_t j = 0; j < facets[i].size(); j++)
			ifacet[j] = facets[i][j];

		Scalar color;
		color[0] = rand() & 255;
		color[1] = rand() & 255;
		color[2] = rand() & 255;
		cv::fillConvexPoly(img, ifacet, color, 8, 0);

		ifacets[0] = ifacet;
		polylines(img, ifacets, true, Scalar(), 1, cv::LINE_AA, 0);
		cv::circle(img, centers[i], 3, Scalar(), cv::FILLED, cv::LINE_AA, 0);
	}
}

void Delaunay::drawMask(Mat& img, int numFeatures)
{
	std::string win = "Face Tracker";
	cv::Rect rect(0, 0, img.cols, img.rows);
	Subdiv2D subdiv(rect);
	for (unsigned i = 0; i < numFeatures; i++)
	{
		Point2f point;
		//TODO: Find points that correspond to statistical 
		// model and pass them into here efficiently
		point.x *= img.cols;
		point.y *= img.rows;
		subdiv.insert(point);
	}
	drawSubdiv(img, subdiv);
	cv::imshow(win, img);
}

void Delaunay::calcMeanMask(string filePath)
{
	Subdiv2D subdiv = findSubdiv(filePath);
	meanMask = makeTrianglePointsVector(subdiv);
}

vector<Point2f[3]>* Delaunay::makeTrianglePointsVector(Subdiv2D& subdiv)
{
	vector<Vec6f> in;
	subdiv.getTriangleList(in);
	vector<Point2f[3]>* out = new vector<Point2f[3]>(in.size());
	int j = 0;
	for (unsigned i = 0; i < in.size(); i++)
	{
		if (in[i][0] > 1000 || in[i][1] > 1000 || in[i][0] < 0 || in[i][1] < 0 ||
			in[i][2] > 1000 || in[i][3] > 1000 || in[i][2] < 0 || in[i][3] < 0 ||
			in[i][4] > 1000 || in[i][5] > 1000 || in[i][4] < 0 || in[i][5] < 0)
		{
		}
		else
		{
			out->at(i)[0] = cv::Point(cvRound(in[i][0]), cvRound(in[i][1]));
			out->at(i)[1] = cv::Point(cvRound(in[i][2]), cvRound(in[i][3]));
			out->at(i)[2] = cv::Point(cvRound(in[i][4]), cvRound(in[i][5]));
			//++j;
		}
	}
	return out;
}


void Delaunay::warpTextureFromTriangles(Subdiv2D& srcSubDiv, Mat& originalImg, Mat& warp_final)
{
	Mat warp_mat(2, 3, CV_32FC1);
	Mat warp_dst, warp_mask;
	vector<Point2f[3]>* srcTri = makeTrianglePointsVector(srcSubDiv);
	for (unsigned i = 4; i < srcTri->size(); i++)
	{
		warp_dst = Mat::zeros(originalImg.rows, originalImg.cols, originalImg.type());
		warp_mask = Mat::zeros(originalImg.rows, originalImg.cols, originalImg.type());

		//Affine Transform
		warp_mat = cv::getAffineTransform(srcTri->at(i), meanMask->at(i));
		std::cout << warp_mat << std::endl;
		std::cout << meanMask->at(i)[0] << " " << meanMask->at(i)[1] << " " << meanMask->at(i)[2] << std::endl;
		std::cout << srcTri->at(i)[0] << " " << srcTri->at(i)[1] << " " << srcTri->at(i)[2] << std::endl;
		// Apply Transform to src
		cv::warpAffine(originalImg, warp_dst, warp_mat, warp_dst.size());
		cv::Point precious[3];
		precious[0] = meanMask->at(i)[0];
		precious[1] = meanMask->at(i)[1];
		precious[2] = meanMask->at(i)[2];
		cv::fillConvexPoly(warp_mask, precious, 3, CV_RGB(255, 255, 255), CV_AA, 0);
		warp_dst.copyTo(warp_final, warp_mask);
		cv::imshow("Final", warp_final);
		cvWaitKey(100);
	}
	delete srcTri;
}

Subdiv2D Delaunay::findSubdiv(string filePath)
{
	Delaunay delau = Delaunay();

	Mat img = cv::imread(filePath + ".jpg", 1);
	assert(!img.empty());
	cv::Rect rect(0, 0, img.cols, img.rows);

	Subdiv2D subdiv(rect);

	std::ifstream in(filePath + ".asf");
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
		subdiv.insert(point);
	}
	return subdiv;
}

void Delaunay::help()
{
	std::cout << "\nThis program demostrates iterative construction of\n"
		"delaunay triangulation and voronoi tesselation.\n"
		"It draws a random set of points in an image and then delaunay triangulates them.\n"
		"Usage: \n"
		"./delaunay \n"
		"\nThis program builds the traingulation interactively, you may stop this process by\n"
		"hitting any key.\n";
}

void Delaunay::runSample()
{
	help();

	cv::Rect rect(0, 0, 600, 600);

	Delaunay delau = Delaunay();

	Subdiv2D subdiv(rect);
	Mat img(rect.size(), CV_8UC3);

	img = Scalar::all(0);
	std::string win = "Delaunay Demo";
	imshow(win, img);

	for (int i = 0; i < 200; i++)
	{
		Point2f fp((float)(rand() % (rect.width - 10) + 5),
			(float)(rand() % (rect.height - 10) + 5));

		delau.locatePoint(img, fp, subdiv);
		imshow(win, img);

		if (cv::waitKey(100) >= 0)
			break;

		subdiv.insert(fp);

		img = Scalar::all(0);
		delau.drawSubdiv(img, subdiv);
		imshow(win, img);

		if (cv::waitKey(100) >= 0)
			break;
	}

	img = Scalar::all(0);
	delau.paintVoronoi(img, subdiv);
	imshow(win, img);

	cv::waitKey(0);
}

void Delaunay::drawSample(string filePath)
{
	help();

	Delaunay delau = Delaunay();

	Mat img = cv::imread(filePath + ".jpg", 1);
	if (img.empty())
	{
		std::cout << "Could not read image!" << std::endl;
		return;
	}
	cv::Rect rect(0, 0, img.cols, img.rows);

	Subdiv2D subdiv(rect);

	std::string win = filePath;
	std::ifstream in(filePath + ".asf");
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
		subdiv.insert(point);
	}
	delau.drawSubdiv(img, subdiv);
	cv::imshow(win, img);
}

void Delaunay::warpSample(string meanLoc, string testLoc)
{
	Mat testImg = cv::imread(testLoc + ".jpg", 1);
	drawSample(meanLoc);
	drawSample(testLoc);
	Delaunay delau = Delaunay(meanLoc);
	Subdiv2D test = delau.findSubdiv(testLoc);
	Mat warp_final = Mat();
	delau.warpTextureFromTriangles(test, testImg, warp_final);
	cv::imshow("Final", warp_final);
}