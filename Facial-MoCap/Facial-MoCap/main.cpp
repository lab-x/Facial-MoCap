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

#ifdef _DEBUG
void KeyboardHandler(unsigned char Key, int x, int y) {

	switch (Key) {

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
#endif

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


//int main(int argc, char* argv[])
//{
//	//Pulls in training image for creation of AAM
//	AAM aam = AAM();
//	aam.buildAAM(path);
//	
//	//Sample delaney drawing on pre-annotated images
//	Delaunay::drawSample(path + "01-1m");
//	Delaunay::drawSample(path + "01-6m");
//
//	//Gets one frame from the camera
//	Mat firstFrame = cam->getFrame();
//
//	while (1)
//	{
//		//Captures the current frame
//		Mat frame = cam->getFrame();
//
//		//show the frame in "Face Tracker" window
//		imshow("Face Tracker", frame);
//
//		//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
//		if (waitKey(30) == 27)
//		{
//			std::cout << "esc key is pressed by user" << std::endl;
//			break;
//		}
//	}
//	return 0;
//}

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	//Init OpenGL With Double Buffer in RGB Mode
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowSize(800, 600);

	glutCreateWindow("3D Model");

	//Set Display Handler
	glutDisplayFunc(Display);

#ifdef _DEBUG
	//Set Keyboard Handler
	glutKeyboardFunc(KeyboardHandler);
	glutSpecialFunc(SpecialKeyboardHandler);
#endif

	glutReshapeFunc(ReshapeWindow);
	glutVisibilityFunc(Visible);

	face->Init();

	//OpenGL Main Loop
	glutMainLoop();

	return 0;
}