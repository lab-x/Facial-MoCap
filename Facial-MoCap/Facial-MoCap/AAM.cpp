#include "AAM.h"

AAM::AAM()
{
	meanModel = vector<Vec3i>();
}

AAM::~AAM()
{
}

void AAM::buildAAM(string filePath)
{
	std::cout << "Loading Training Set";
	vector<TImage*>* imgs = TImage::loadAllTraining(filePath);
	std::cout << "...Done" << std::endl;
	std::cout << "Generating Mean Model";
	generateMeanModel(imgs->at(0));
	std::cout << "...Done" << std::endl;
	std::cout << "Warp Images to Mean Model";
	for (unsigned i = 0; i < imgs->size(); i++)
	{
		warpToMean(imgs->at(i));
		std::cout << i;
	}
	std::cout << "...Done" << std::endl;
}

int AAM::findPoint(float x, float y, vector<Point2f>* points)
{
	for (unsigned i = 0; i < points->size(); i++)
	{
		if (points->at(i).x == x && points->at(i).y == y)
			return i;
	}
	return INT_MAX;
}

Mat * AAM::warpToMean(TImage * img)
{
	int rows = img->getRows();
	int cols = img->getCols();
	vector<Point2f>* point = img->getPoints();
	Mat warp_mat(2, 3, CV_32FC1);
	Mat warp_dst, warp_mask;
	for (unsigned i = 0; i < meanModel.size(); i++)
	{
		warp_dst = Mat::zeros(rows, cols, img->getImg()->type());
		warp_mask = Mat::zeros(rows, cols, img->getImg()->type());

		Point2f src[3] = { point->at(meanModel[i](0)), point->at(meanModel[i](1)), point->at(meanModel[i](2))};
		Point2f dest[3] = { meanPoints->at(meanModel[i](0)), meanPoints->at(meanModel[i](1)), meanPoints->at(meanModel[i](2)) };

		//Affine Transform
		warp_mat = cv::getAffineTransform(src, dest);
		std::cout << warp_mat << std::endl;
		// Apply Transform to src
		cv::warpAffine(*img->getImg(), warp_dst, warp_mat, warp_dst.size());
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

void AAM::generateMeanModel(TImage* img)
{
	int cols = img->getCols();
	int rows = img->getRows();
	meanPoints = img->getPoints();
	Subdiv2D subdiv = Delaunay::findSubdiv(img);
	vector<Vec6f> triangles;
	subdiv.getTriangleList(triangles);
	for (unsigned i = 0; i < triangles.size(); i++)
	{
		if (cols > triangles[i][0] && rows > triangles[i][1] &&
			cols > triangles[i][2] && rows > triangles[i][3] &&
			cols > triangles[i][4] && rows > triangles[i][5] &&
			0 <= triangles[i][0] && 0 <= triangles[i][1] &&
			0 <= triangles[i][2] && 0 <= triangles[i][3] &&
			0 <= triangles[i][4] && 0 <= triangles[i][5])
		{
			Vec3i vec = Vec3i();
			vec[0] = findPoint(triangles[i][0], triangles[i][1], meanPoints);
			vec[1] = findPoint(triangles[i][2], triangles[i][3], meanPoints);
			vec[2] = findPoint(triangles[i][4], triangles[i][5], meanPoints);
			meanModel.push_back(vec);
#if defined(_DEBUG)
			std::cout << std::endl << vec;
#endif
		}
	}
}
