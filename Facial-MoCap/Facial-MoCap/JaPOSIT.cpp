#include "JaPOSIT.h"



JaPOSIT::JaPOSIT()
{
	rv = vector<double>(3);
	tv = vector<double>(3);
	rvec = Mat(rv);
	tvec = Mat(tv);
}


JaPOSIT::~JaPOSIT()
{
}

void JaPOSIT::init(FaceModel* in)
{
	faceModel = in;
	op = Mat(*faceModel->getFeatures());
	
	//Reducing to being deviation from mean
	cv::Scalar m = mean(op);
	//op = op-m

	std::cout << "Model Points" << op << std::endl;

	rvec = Mat(rv);
	
	double d[9] = { 1, 0, 0,
				   0, -1, 0,
				   0, 0, -1 };

	cv::Rodrigues(Mat(3, 3, CV_64FC1, d), rvec);
	tv[0] = 0;
	tv[1] = 0;
	tv[2] = 1;
	tvec = Mat(tv);

	camMatrix = Mat(3, 3, CV_64FC1);
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


void JaPOSIT::loadWithPoints(vector<Point2f> imgPoints, Mat & img)
{
	Mat ip(imgPoints);
	int max_d = MAX(img.rows, img.cols);
	camMatrix = (Mat_<double>(3, 3) << max_d, 0, img.cols / 2.0,
		0, max_d, img.rows / 2.0,
		0, 0, 1.0);

	std::cout << "Cam Matrix:" << std::endl << camMatrix << std::endl;

	double dc[] = { 0,0,0,0 };
	solvePnP(op, imgPoints, camMatrix, Mat(1, 4, CV_64FC1, dc), rvec, tvec, false, CV_EPNP);

	Mat rotM(3, 3, CV_64FC1, rot);
	cv::Rodrigues(rvec, rotM);
	double* r = rotM.ptr<double>();

	double pm[12] = {r[0], r[1], r[2], tv[0],
					 r[3], r[4], r[5], tv[1],
					 r[6], r[7], r[8], tv[2]};

	Matx34d P(pm);
	Mat KP = camMatrix * Mat(P);

	//reproject object points - check validity of found projection matrix
	for (unsigned i = 0; i < op.rows; i++)
	{
		Mat_<double> X = (Mat_<double>(4, 1) << op.at<float>(i, 0), op.at<float>(i, 1), op.at<float>(i, 2), 1.0);

		Mat_<double> opt_p = KP * X;
		Point2f opt_p_img(opt_p(0) / opt_p(2), opt_p(1) / opt_p(2));

		cv::circle(img, opt_p_img, 4, cv::Scalar(0, 0, 255), 1);
	}
	rotM = rotM.t(); //Transposing in order to conform with majorness of OpenGL matrix
}

double* JaPOSIT::getOpenGLMatrix()
{
	double d[16] = { rot[0], rot[1], rot[2], 0,
					 rot[3], rot[4], rot[5], 0,
					 rot[6], rot[7], rot[8], 0,
					 0,		 0,		 0,		 1 };
	return d;
}
