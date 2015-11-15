#include "Delaunay.h"



Delaunay::Delaunay()
{
}


Delaunay::~Delaunay()
{
}

//Draw the point in space onto the matrix representation of the image
void Delaunay::drawSubdivPoint(Mat & img, Point2f fp, Scalar color)
{
	cv::circle(img, fp, 3, color, cv::FILLED, cv::LINE_8, 0);
}

//
void Delaunay::drawSubdiv(Mat & img, Subdiv2D & subdiv, Scalar delaunayColor)
{
#if 1
	std::vector<cv::Vec6f> triangleList;
	subdiv.getTriangleList(triangleList);
	std::vector<cv::Point> pt(3);

	for (size_t i = 0; i < triangleList.size(); i++)
	{
		cv::Vec6f t = triangleList[i];
		pt[0] = cv::Point(cvRound(t[0]), cvRound(t[1]));
		pt[1] = cv::Point(cvRound(t[2]), cvRound(t[3]));
		pt[2] = cv::Point(cvRound(t[4]), cvRound(t[5]));

		//draw lines
		cv::line(img, pt[0], pt[1], delaunayColor, 1, cv::LINE_AA, 0);
		cv::line(img, pt[1], pt[2], delaunayColor, 1, cv::LINE_AA, 0);
		cv::line(img, pt[2], pt[0], delaunayColor, 1, cv::LINE_AA, 0);
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

void Delaunay::locatePoint(Mat & img, Subdiv2D & subdiv, Point2f fp, Scalar activeColor)
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

	drawSubdivPoint(img, fp, activeColor);
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
