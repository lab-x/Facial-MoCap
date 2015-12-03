#include "AAM.h"

AAM::AAM()
{
	meanModel = vector<Vec3i>();
	minX = INT_MAX;
	minY = INT_MAX;
	maxY = 0;
	maxX = 0;
}

AAM::~AAM()
{
	delete meanPoints;
}

void AAM::buildAAM(string filePath)
{
	std::cout << "Loading Training Set";
	vector<TImage*>* imgs = TImage::loadAllTraining(filePath);
	std::cout << "...Done" << std::endl;
	
	int cols = imgs->at(0)->getCols();
	int rows = imgs->at(0)->getRows();
	
	{//Loads the annotation points into a PCA set and finds the mean shape model
		std::cout << "Add to PCA Set\n";
		//Creates a matrix that has rows = #training images and cols = #features tracked
		Mat pcaSet = Mat::eye(imgs->size(), imgs->at(0)->getPoints()->size() * 2, CV_64F);
		//Iterates through all training images warping and adding the feature points to the PCA set
		for (unsigned i = 0; i < imgs->size(); i++)
			loadPCAPoints(imgs->at(i)->getPoints(), pcaSet, i);
		std::cout << "...Done" << std::endl;
		std::cout << "Create PCA Object";
		shapeModel = PCA(pcaSet,				//pass the point data
			Mat(),					//no pre-computed mean vector, let PCA engine compute
			CV_PCA_DATA_AS_ROW,		//Data is stored as a row as the vectors span across columns
			pcaSet.cols				//How many principal components to retain
			);
		std::cout << "...Done" << std::endl;
		std::cout << "Generating Mean Shape Model";
		genMeanShapeModel(imgs->at(0), cols, rows);
		std::cout << "...Done" << std::endl;
	}

	{//Loads the relevant image points into the PCA set and finds the mean appearance
		std::cout << "Warp and add to PCA Set";
		//Creates a matrix that has rows = #training images and cols = #relevant pixels
		int pcaCols = (maxX - minX) * (maxY - minY);
		Mat pcaSet = Mat::eye(imgs->size(), pcaCols, CV_64F);
		for (unsigned i = 0; i < imgs->size(); i++)
			loadPCAPixels(imgs->at(i), pcaSet, i);
		std::cout << "...Done" << std::endl;
		std::cout << "Create PCA Object";
		appearModel = PCA(pcaSet,
			Mat(),
			CV_PCA_DATA_AS_ROW,
			pcaSet.cols
			);
		std::cout << "...Done" << std::endl;
		std::cout << "Generating Mean Appearance Model";
		genMeanAppearanceModel();
		std::cout << "...Done" << std::endl;
	}
	TImage::deleteAllTraining(imgs);
}

int AAM::findPoint(float x, float y, vector<Point2f>* points)
{
	if (x > maxX)
		maxX = x+10;
	else if (x < minX)
		minX = x-10;
	if (y > maxY)
		maxY = y;
	else if (y < minY)
		minY = y;


	for (unsigned i = 0; i < points->size(); i++)
		if (points->at(i).x == x && points->at(i).y == y)
			return i;


	return INT_MAX;
}

Mat AAM::warpToMean(TImage * img)
{
	Mat *matImg = img->getImg();
	int rows = img->getRows();
	int cols = img->getCols();
	vector<Point2f>* point = img->getPoints();
	Mat warp_mat(2, 3, CV_32FC1);
	Mat warp_dst, warp_mask;
	Mat warp_final = Mat();
	for (unsigned i = 0; i < meanModel.size(); i++)
	{
		warp_dst = Mat::zeros(rows, cols, matImg->type());
		warp_mask = Mat::zeros(rows, cols, matImg->type());

		Point2f src[3] = { point->at(meanModel[i](0)), point->at(meanModel[i](1)), point->at(meanModel[i](2)) };
		Point2f dest[3] = { meanPoints->at(meanModel[i](0)), meanPoints->at(meanModel[i](1)), meanPoints->at(meanModel[i](2)) };

		//Affine Transform
		warp_mat = cv::getAffineTransform(src, dest);
		// Apply Transform to src
		cv::warpAffine(*matImg, warp_dst, warp_mat, warp_dst.size());
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
	return warp_final;
}

void AAM::loadPCAPoints(const vector<Point2f>* points, Mat & pcaSet, unsigned index)
{
	int j = 0;
	//Iterates through the points passed in and adds them to the pcaSet in order
	for (unsigned i = 0; i < points->size(); i++)
	{
		pcaSet.at<double>(index, j++) = points->at(i).x;
		pcaSet.at<double>(index, j++) = points->at(i).y;
	}
}

void AAM::loadPCAPixels(TImage * img, Mat & pcaSet, unsigned index)
{
	Mat warped = warpToMean(img);
	int k = 0;
	cv::cvtColor(warped, meanAppearance, cv::COLOR_BGR2GRAY);
	for (unsigned i = minY; i < maxY; i++)
		for (unsigned j = minX; j < maxX; j++)
			if(meanAppearance.at<uchar>(j,i) != 0)
				pcaSet.at<double>(index, k++) = meanAppearance.at<uchar>(j, i);
}

void AAM::genMeanShapeModel(TImage* img, int cols, int rows)
{
	//Convenient use of a global variable
	meanPoints = img->getPoints();
	Subdiv2D subdiv = Delaunay::findSubdiv(img);
	vector<Vec6f> triangles;
	subdiv.getTriangleList(triangles);
	//Initializes the triangle path that all images will follow
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
	//This portion of the function initializes the meanPoints property with the mean as
	// calculated by the PCA
	meanPoints = new vector<Point2f>(meanPoints->size());
	int j = 0;
	for (size_t i = 0; i < meanPoints->size(); i++)
	{
		meanPoints->at(i).x = shapeModel.mean.at<double>(0, j++);
		meanPoints->at(i).y = shapeModel.mean.at<double>(0, j++);
	}
}

void AAM::genMeanAppearanceModel()
{
	int k = 0;
	for (unsigned i = minY; i < maxY; i++)
		for (unsigned j = minX; j < maxX; j++)
		{
			if (meanAppearance.at<uchar>(j, i) != 0)
				meanAppearance.at<uchar>(j, i) = appearModel.mean.at<double>(0, k++);
		}
#ifdef _DEBUG
	cv::imshow("Mean Appearance", meanAppearance);
#endif
}
