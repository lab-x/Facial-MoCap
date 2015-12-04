#include "JaPOSIT.h"



JaPOSIT::JaPOSIT()
{
}


JaPOSIT::~JaPOSIT()
{
}

void JaPOSIT::loadFaceModel(FaceModel* in)
{
	faceModel = in;
}

void JaPOSIT::mapPoints(vector<Point2f> imgFeatures)
{
	//The default face model happens to match up nicely with the IMM dataset so
	// the map will be 1-1 indices.

	//Thinking of an algorithm for generalization... One could theoretically move the
	// reference of imgFeatures to the center of the mask and have a threshold that matches
	// certain points. Large margin for error due to mean face model possibly being completely
	// different from the 3D face model. Best practice... face model uploaded has indices in same
	// order as annotations.
	for (unsigned i = 0; i < imgFeatures.size(); i++)
	{
		map.push_back(Vec2i(i, i));
	}
}
