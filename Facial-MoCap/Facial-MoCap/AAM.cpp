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
	std::cout << "Warp Images to Mean Model and Add to PCA Set\n";
	//Creates a matrix that has rows = #training images and cols = #features tracked
	Mat pcaSet = Mat::eye(imgs->size(), meanPoints->size() * 2, CV_64F);
	//Iterates through all training images warping and adding the feature points to the PCA set
	for (unsigned i = 0; i < imgs->size(); i++)
	{
		std::cout << i << ": Warping";
		warpToMean(imgs->at(i));
		std::cout << "...Done; PCA Load";
		loadPCAPoints(imgs->at(i)->getPoints(), pcaSet, i);
		std::cout << "...Done\n";
	}
	std::cout << "...Done" << std::endl;
	std::cout << "Create PCA Set";
	shapeModel = PCA(pcaSet,				//pass the point data
					Mat(),					//no pre-computed mean vector, let PCA engine compute
					CV_PCA_DATA_AS_ROW,		//Data is stored as a row as the vectors span across columns
					pcaSet.cols				//How many principal components to retain
					);
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

void AAM::warpToMean(TImage * img)
{
	int rows = img->getRows();
	int cols = img->getCols();
	vector<Point2f>* point = img->getPoints();
	Mat warp_mat(2, 3, CV_32FC1);
	Mat warp_dst, warp_mask;
	Mat warp_final = Mat();
	for (unsigned i = 0; i < meanModel.size(); i++)
	{
		warp_dst = Mat::zeros(rows, cols, img->getImg()->type());
		warp_mask = Mat::zeros(rows, cols, img->getImg()->type());

		Point2f src[3] = { point->at(meanModel[i](0)), point->at(meanModel[i](1)), point->at(meanModel[i](2)) };
		Point2f dest[3] = { meanPoints->at(meanModel[i](0)), meanPoints->at(meanModel[i](1)), meanPoints->at(meanModel[i](2)) };

		//Affine Transform
		warp_mat = cv::getAffineTransform(src, dest);
		// Apply Transform to src
		cv::warpAffine(*img->getImg(), warp_dst, warp_mat, warp_dst.size());
		cv::Point precious[3] = { dest[0], dest[1], dest[2] };
		cv::fillConvexPoly(warp_mask, precious, 3, CV_RGB(255, 255, 255), CV_AA, 0);
		warp_dst.copyTo(warp_final, warp_mask);
#if defined(_DEBUG)
		cv::imshow("Final", warp_final);
	}
	cv::waitKey(10);
#else
	}
#endif
	img->loadWarpedImg(warp_final);
}

void AAM::loadPCAPoints(const vector<Point2f>* points, Mat & pcaSet, unsigned index)
{
	//Iterates through the points passed in and adds them to the pcaSet in order
	for (unsigned i = 0; i < points->size(); i++)
	{
		pcaSet.at<double>(index, i) = points->at(i).x;
		pcaSet.at<double>(index, i+1) = points->at(i).y;
	}
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
