#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include "opencv\highgui.h"
#include "WebCam.h"
#include "Delaunay.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <opencv2\imgproc\types_c.h>
#include "AAM.h"
#include "FaceModel.h"

using namespace cv;
using std::string;

//Some globals :(
static WebCam *cam = new WebCam(0);

static FaceModel *face = new FaceModel();

//Path to images and annotations
const string path = "../../media/IMM/";

void drawPts(Mat pcaset, PCA pca, PCA pcaTexture, std::vector<CvPoint>& pointsInsideHull, std::vector<int> triangleIndexes)
{
	int val1, val2, val3 = 50;
	int alphaMax = 100;
	int imageCount = 1;

	char fileName[100];

	Mat coeffs = Mat::zeros(1, 3, CV_64F);

	namedWindow("AAM");

	cv::Mat imgFrame = imread(fileName);
	int t, ellap;

	while (1)
	{

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
			//warpTextureFromTriangle(sourcePoints, aamTexture, destPoints, img);
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


//OpenGL Display Routine
void Display(void) {
	int k;

	glClear(GL_COLOR_BUFFER_BIT);




	glPushMatrix();

	glTranslatef(face->FaceTx, face->FaceTy, face->FaceTz);

	glRotatef(face->FaceRoll, 0, 0, 1);
	glRotatef(face->FacePitch, 0, 1, 0);
	glRotatef(face->FaceYaw, 1, 0, 0);


	face->Anthropometric3DModel(3);

	face->DrawAxes(5, 0.1);

	glPopMatrix();




	glutSwapBuffers();


	//Read From FrameBuffer
	glReadPixels(0, 0, face->Image->width, face->Image->height, GL_BGR, GL_UNSIGNED_BYTE, face->Image->imageData);

}

//OpenGL Idle Function
void Idle(void) {
	//printf("glIdle\n");
	glutPostRedisplay();
}

void Visible(int vis) {
	if (vis == GLUT_VISIBLE)
		glutIdleFunc(Idle);
	else
		glutIdleFunc(NULL);
}

void KeyboardHandler(unsigned char Key, int x, int y) {

	switch (Key) {

	case 27: 	cvReleaseImage(&face->Image);
		cvDestroyWindow("Image");
		exit(0); break;


	case 'r': face->WorldRoll += 5; break;
	case 'R': face->WorldRoll -= 5; break;

	case 'p': face->WorldPitch += 5; break;
	case 'P': face->WorldPitch -= 5; break;

	case 'y': face->WorldYaw += 5; break;
	case 'Y': face->WorldYaw -= 5; break;





	case '7': face->FaceRoll += 2; break;
	case '1': face->FaceRoll -= 2; break;

	case '9': face->FacePitch += 2; break;
	case '3': face->FacePitch -= 2; break;

	case '/': face->FaceYaw += 2; break;
	case '*': face->FaceYaw -= 2; break;

	case '6': face->FaceTx += 0.5; break;
	case '4': face->FaceTx -= 0.5; break;

	case '8': face->FaceTy += 0.5; break;
	case '2': face->FaceTy -= 0.5; break;

	case '+': face->FaceTz += 0.5; break;
	case '-': face->FaceTz -= 0.5; break;




	case 's':	cvFlip(face->Image, face->Image);
		cvShowImage("Image", face->Image);
		cvWaitKey(50);
		char Buffer[50];
		static int FrameIndex = 0;
		cvSaveImage(Buffer, face->Image);
		break;



	}




	glRotatef(face->WorldRoll, 0, 0, 1);
	glRotatef(face->WorldPitch, 0, 1, 0);
	glRotatef(face->WorldYaw, 1, 0, 0);


	face->WorldRoll = 0;
	face->WorldPitch = 0;
	face->WorldYaw = 0;

	glutPostRedisplay();
}

void SpecialKeyboardHandler(int Key, int x, int y) {
	switch (Key) {

	case GLUT_KEY_RIGHT: face->WorldTx += 0.5; break;
	case GLUT_KEY_LEFT: face->WorldTx -= 0.5; break;
	case GLUT_KEY_UP: face->WorldTy += 0.5; break;
	case GLUT_KEY_DOWN: face->WorldTy -= 0.5; break;
	case GLUT_KEY_PAGE_UP: face->WorldTz += 0.5; break;
	case GLUT_KEY_PAGE_DOWN: face->WorldTz -= 0.5; break;
	}

	glTranslatef(face->WorldTx, face->WorldTy, face->WorldTz);

	face->WorldTx = 0;
	face->WorldTy = 0;
	face->WorldTz = 0;

	glutPostRedisplay();
}

//Reshape Window Handler
void ReshapeWindow(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 200.0);

	//Using Orthographic Projection
	//glOrtho (0, w, 0, h, -5.0, 5.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -25);


	//glGetDoublev(GL_MODELVIEW_MATRIX, R);

}


int main(int argc, char* argv[])
{
	//Initialize capture of Webcam
	namedWindow("Face Tracker", CV_WINDOW_AUTOSIZE); //create a window
#pragma region Inital Face Finding
	AAM aam = AAM();
	aam.buildAAM(path);
	Delaunay::drawSample(path + "01-1m");
	Delaunay::drawSample(path + "01-6m");
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