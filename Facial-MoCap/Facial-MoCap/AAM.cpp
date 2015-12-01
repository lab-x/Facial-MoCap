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

void AAM::generateMeanModel(TImage* img)
{
	int cols = img->getCols();
	int rows = img->getRows();
	vector<Point2f>* points = img->getPoints();
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
			vec[0] = findPoint(triangles[i][0], triangles[i][1], points);
			vec[1] = findPoint(triangles[i][2], triangles[i][3], points);
			vec[2] = findPoint(triangles[i][4], triangles[i][5], points);
			meanModel.push_back(vec);
#if defined(_DEBUG)
			std::cout << vec << std::endl;
#endif
		}
	}
}
