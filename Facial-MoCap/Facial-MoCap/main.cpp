#include "opencv\highgui.h"
#include "WebCam.h"
#include "Delaunay.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <opencv2\imgproc\types_c.h>
#include "AAM.h"

using namespace cv;
using std::string;

//Some globals :(
static WebCam *cam = new WebCam(0);

//Path to images and annotations
const string path = "../../media/IMM/";

void warpTextureFromTriangle(Point2f srcTri[3], Mat originalImage, Point2f dstTri[3], Mat warp_final)
{
	Mat warp_mat(2, 3, CV_32FC1);
	Mat warp_dst, warp_mask;
	CvPoint trianglePoints[3];
	trianglePoints[0] = dstTri[0];
	trianglePoints[1] = dstTri[1];
	trianglePoints[2] = dstTri[2];
	warp_dst = Mat::zeros(originalImage.rows, originalImage.cols, originalImage.type());
	warp_mask = Mat::zeros(originalImage.rows, originalImage.cols, originalImage.type());

	for (int i = 0; i < 3; i++)
	{
		srcTri[i].x -= 190;
		srcTri[i].y -= 250;
		dstTri[i].x -= 190;
		dstTri[i].y -= 250;
	}

	warp_mat = getAffineTransform(srcTri, dstTri);

	//Apply affine transform just found to src img
	Rect roi(190, 250, 240, 150);
	Mat originalImageRoi = originalImage(roi);
	Mat warp_dstRoi = warp_dst(roi);
	warpAffine(originalImageRoi, warp_dstRoi, warp_mat, warp_dstRoi.size());
	cvFillConvexPoly(new IplImage(warp_mask), trianglePoints, 3, CV_RGB(255, 255, 255), CV_AA, 0);
	warp_dst.copyTo(warp_final, warp_mask);
}

PCA loadPCA(Mat& pcaset)
{
	int cols = 58 * 2;
	pcaset = cv::Mat::eye(6, cols, CV_64F);
	for (unsigned i = 0; i < 6; i++)
	{
		std::ifstream in(path);
		int pointNum;
		string line;
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
		for (unsigned j = 0; j < 58 * 2; j += 2)
		{
			while (std::getline(in, line))
			{
				if (line[0] != '#' && line != "")
					break;
			}
			std::istringstream iss(line);
			iss >> tmp;
			iss >> tmp;
			iss >> pcaset.at<double>(i, j);
			iss >> pcaset.at<double>(i, j + 1);
		}
	}
	std::cout << pcaset << std::endl;

	PCA pca(pcaset, cv::Mat(), CV_PCA_DATA_AS_ROW, pcaset.cols);
	return pca;
}

void drawPts(Mat pcaset, PCA pca, PCA pcaTexture, std::vector<CvPoint>& pointsInsideHull, std::vector<int> triangleIndexes)
{
	int val1, val2, val3 = 50;
	int alphaMax = 100;
	int imageCount = 1;

	char fileName[100];
	sprintf(fileName, "09-%dm.jpg", imageCount);

	Mat coeffs = Mat::zeros(1, 3, CV_64F);

	namedWindow("AAM");

	cv::Mat imgFrame = imread(fileName);
	int t, ellap;

	while (1)
	{
		sprintf(fileName, "09-%dm.jpg", imageCount);

		Mat img = cam->getFrame();

		createTrackbar("eigen1", "AAM", &val1, alphaMax);
		createTrackbar("eigen2", "AAM", &val2, alphaMax);
		createTrackbar("eigen3", "AAM", &val3, alphaMax);

		coeffs.at<double>(0, 0) = ((val1 * 1.0 / alphaMax) - 0.5) * 2 * 3 * sqrt(pca.eigenvalues.at<double>(0, 0));
		coeffs.at<double>(0, 1) = ((val2 * 1.0 / alphaMax) - 0.5) * 2 * 3 * sqrt(pca.eigenvalues.at<double>(1, 0));
		coeffs.at<double>(0, 2) = ((val3 * 1.0 / alphaMax) - 0.5) * 2 * 3 * sqrt(pca.eigenvalues.at<double>(2, 0));

		Mat back;
		Mat backTexture;
		Mat aamTexture = Mat::zeros(480, 640, imgFrame.type());

		pca.backProject(coeffs, back);
		pcaTexture.backProject(coeffs, backTexture);

		for (unsigned i = 0; i < pointsInsideHull.size(); i++)
		{
			//Traces the three points for each triple pair of vertices, checking if already evaluated
			for (unsigned j = 0; j < 3;)
			{
				double v = ((backTexture.at<double>(0, 3 * i + j)) * 255);
				v = (v > 255) ? 255 : v;
				v = (v < 0) ? 0 : v;
				aamTexture.at<Vec3b>(pointsInsideHull.at(i))[j] = v;
			}
		}
		imshow("AAM Texture", aamTexture);

		//draw aam
		for (unsigned j = 0; j < (back.cols / 2) - 1; j++)
		{
			Point2f p(back.at<double>(0, 2 * j), back.at<double>(0, 2 * j + 1));
			Point2f p2(back.at<double>(0, 2 * j + 2), back.at<double>(0, 2 * j + 3));
			line(img, p, p2, CV_RGB(255, 0, 0), 3, 8, 0);
			circle(img, p, 4, CV_RGB(128, 0, 0), -1, 8);
			circle(img, p2, 4, CV_RGB(128, 0, 0), -1, 8);
		}

		//warp the generated texture to give a 3D view on a person's face
		for (unsigned i = 0; i < triangleIndexes.size() / 3; i++)
		{
			Point2f sourcePoints[3];
			Point2f destPoints[3];
			for (unsigned j = 0; j < 3; j++)
			{
				int index = triangleIndexes.at(3 * i + j);

				sourcePoints[j].x = pca.mean.at<double>(0, 2 * index);
				sourcePoints[j].y = pca.mean.at<double>(0, 2 * index + 1);
				destPoints[j].x = back.at<double>(0, 2 * index);
				destPoints[j].y = back.at<double>(0, 2 * index + 1);
			}
			warpTextureFromTriangle(sourcePoints, aamTexture, destPoints, img);
		}

		img.copyTo(imgFrame);
		imshow("AAM", imgFrame);
		char c = waitKey(10);
		if (c == 'c') break;
		if (c == '1') imageCount = 1;
		if (c == '2') imageCount = 2;
		if (c == '3') imageCount = 3;

	}
}

int main(int argc, char* argv[])
{
	//Initialize capture of Webcam
	namedWindow("Face Tracker", CV_WINDOW_AUTOSIZE); //create a window
#pragma region Inital Face Finding
	AAM aam = AAM();
	aam.buildAAM("../../media/IMM/");
	Delaunay::drawSample("../../media/IMM/01-1m");
	Delaunay::drawSample("../../media/IMM/01-6m");
	PCA pca = loadPCA(Mat());
	Mat firstFrame = cam->getFrame();
#pragma endregion

	while (1)
	{
		//Where captured frame resides
		Mat frame = cam->getFrame();

		//show the frame in "Face Tracker" window
		imshow("Face Tracker", frame);

		//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		if (waitKey(30) == 27)
		{
			std::cout << "esc key is pressed by user" << std::endl;
			break;
		}
	}
	return 0;

}