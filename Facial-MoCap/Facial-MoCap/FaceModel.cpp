#include "FaceModel.h"

const std::vector<Point3f> FaceModel::Model3D = { { -7.308957f,0.913869f,0.000000f },{ -6.775290f,-0.730814f,-0.012799f },{ -5.665918f,-3.286078f,1.022951f },{ -5.011779f,-4.876396f,1.047961f },{ -4.056931f,-5.947019f,1.636229f },{ -1.833492f,-7.056977f,4.061275f },{ 0.000000f,-7.415691f,4.070434f },{ 1.833492f,-7.056977f,4.061275f },{ 4.056931f,-5.947019f,1.636229f },{ 5.011779f,-4.876396f,1.047961f },{ 5.665918f,-3.286078f,1.022951f },{ 6.775290f,-0.730814f,-0.012799f },{ 7.308957f,0.913869f,0.000000f },{ 5.311432f,5.485328f,3.987654f },{ 4.461908f,6.189018f,5.594410f },{ 3.550622f,6.185143f,5.712299f },{ 2.542231f,5.862829f,4.687939f },{ 1.789930f,5.393625f,4.413414f },{ 2.693583f,5.018237f,5.072837f },{ 3.530191f,4.981603f,4.937805f },{ 4.490323f,5.186498f,4.694397f },{ -5.311432f,5.485328f,3.987654f },{ -4.461908f,6.189018f,5.594410f },{ -3.550622f,6.185143f,5.712299f },{ -2.542231f,5.862829f,4.687939f },{ -1.789930f,5.393625f,4.413414f },{ -2.693583f,5.018237f,5.072837f },{ -3.530191f,4.981603f,4.937805f },{ -4.490323f,5.186498f,4.694397f },{ 1.330353f,7.122144f,6.903745f },{ 2.533424f,7.878085f,7.451034f },{ 4.861131f,7.878672f,6.601275f },{ 6.137002f,7.271266f,5.200823f },{ 6.825897f,6.760612f,4.402142f },{ -1.330353f,7.122144f,6.903745f },{ -2.533424f,7.878085f,7.451034f },{ -4.861131f,7.878672f,6.601275f },{ -6.137002f,7.271266f,5.200823f },{ -6.825897f,6.760612f,4.402142f },{ -2.774015f,-2.080775f,5.048531f },{ -0.509714f,-1.571179f,6.566167f },{ 0.000000f,-1.646444f,6.704956f },{ 0.509714f,-1.571179f,6.566167f },{ 2.774015f,-2.080775f,5.048531f },{ 0.589441f,-2.958597f,6.109526f },{ 0.000000f,-3.116408f,6.097667f },{ -0.589441f,-2.958597f,6.109526f },{ -0.981972f,4.554081f,6.301271f },{ -0.973987f,1.916389f,7.654050f },{ -2.005628f,1.409845f,6.165652f },{ -1.930245f,0.424351f,5.914376f },{ -0.746313f,0.348381f,6.263227f },{ 0.000000f,0.000000f,6.763430f },{ 0.746313f,0.348381f,6.263227f },{ 1.930245f,0.424351f,5.914376f },{ 2.005628f,1.409845f,6.165652f },{ 0.973987f,1.916389f,7.654050f },{ 0.981972f,4.554081f,6.301271f } };

const int FaceModel::MeanDelaunayTriangles[95][3] = { { 0,1,50 },{ 0,21,38 },{ 0,28,21 },{ 0,49,28 },{ 0,50,49 },{ 1,2,39 },{ 1,39,50 },{ 2,3,39 },{ 3,4,46 },{ 3,46,39 },{ 4,5,46 },{ 5,6,45 },{ 5,45,46 },{ 6,7,44 },{ 6,44,45 },{ 7,8,44 },{ 8,9,43 },{ 8,43,44 },{ 9,10,43 },{ 10,11,43 },{ 11,12,54 },{ 11,54,43 },{ 12,13,20 },{ 12,20,55 },{ 12,33,13 },{ 12,55,54 },{ 13,14,20 },{ 13,33,14 },{ 14,15,19 },{ 14,19,20 },{ 14,31,15 },{ 14,32,31 },{ 14,33,32 },{ 15,16,19 },{ 15,30,16 },{ 15,31,30 },{ 16,17,18 },{ 16,18,19 },{ 16,29,17 },{ 16,30,29 },{ 17,29,57 },{ 17,57,18 },{ 18,56,19 },{ 18,57,56 },{ 19,55,20 },{ 19,56,55 },{ 21,22,38 },{ 21,28,22 },{ 22,23,36 },{ 22,27,23 },{ 22,28,27 },{ 22,36,37 },{ 22,37,38 },{ 23,24,35 },{ 23,26,24 },{ 23,27,26 },{ 23,35,36 },
{ 24,25,34 },{ 24,26,25 },{ 24,34,35 },{ 25,26,48 },{ 25,47,34 },{ 25,48,47 },{ 26,27,49 },{ 26,49,48 },{ 27,28,49 },{ 29,30,35 },{ 29,34,57 },{ 29,35,34 },{ 30,31,36 },{ 30,36,35 },{ 34,47,57 },{ 39,40,50 },{ 39,46,40 },{ 40,41,52 },{ 40,46,41 },{ 40,51,50 },{ 40,52,51 },{ 41,42,52 },{ 41,44,42 },
{ 41,45,44 },{ 41,46,45 },{ 42,43,53 },{ 42,44,43 },{ 42,53,52 },{ 43,54,53 },{ 47,48,57 },{ 48,49,51 },{ 48,51,52 },{ 48,52,56 },{ 48,56,57 },{ 49,50,51 },{ 52,53,56 },{ 53,54,55 },{ 53,55,56 } };

FaceModel::FaceModel()
{
}


FaceModel::~FaceModel()
{
}

void FaceModel::cvPrintMat(Mat * Matrix)
{
	std::cout << Matrix << std::endl;
}

void FaceModel::Anthropometric3DModel(int LineWidth)
{
	int k;

	glColor3f(0.0, 0.0, 0.0);

	glLineWidth(LineWidth);

	glPushMatrix();

	glBegin(GL_LINES);
	for (k = 0; k<95; k++) {

		glVertex3f(Model3D[MeanDelaunayTriangles[k][0]].x, Model3D[MeanDelaunayTriangles[k][0]].y, Model3D[MeanDelaunayTriangles[k][0]].z);
		glVertex3f(Model3D[MeanDelaunayTriangles[k][1]].x, Model3D[MeanDelaunayTriangles[k][1]].y, Model3D[MeanDelaunayTriangles[k][1]].z);

		glVertex3f(Model3D[MeanDelaunayTriangles[k][1]].x, Model3D[MeanDelaunayTriangles[k][1]].y, Model3D[MeanDelaunayTriangles[k][1]].z);
		glVertex3f(Model3D[MeanDelaunayTriangles[k][2]].x, Model3D[MeanDelaunayTriangles[k][2]].y, Model3D[MeanDelaunayTriangles[k][2]].z);

		glVertex3f(Model3D[MeanDelaunayTriangles[k][2]].x, Model3D[MeanDelaunayTriangles[k][2]].y, Model3D[MeanDelaunayTriangles[k][2]].z);
		glVertex3f(Model3D[MeanDelaunayTriangles[k][0]].x, Model3D[MeanDelaunayTriangles[k][0]].y, Model3D[MeanDelaunayTriangles[k][0]].z);

	}
	glEnd();



	glColor3f(1.0, 0.0, 0.0);
	glPointSize(LineWidth + 2);

	glBegin(GL_POINTS);
	for (k = 0; k < 58; k++)
	{
		glVertex3f(Model3D[k].x, Model3D[k].y, Model3D[k].z);
	}
	glEnd();

	glPopMatrix();
}

void FaceModel::DrawAxes(float Size, float Radius)
{

	glPushMatrix();


	//Draw z axis
	glColor3f(0.0, 0.0, 1.0);
	glPushMatrix();
	gluCylinder(Quadric, Radius, Radius, Size, 40, 40);
	glTranslatef(0, 0, Size);
	gluCylinder(Quadric, Radius * 2, 0, 1, 40, 40);
	glPopMatrix();


	//Draw x axis
	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);	//rotate over y 90º
	gluCylinder(Quadric, Radius, Radius, Size, 40, 40);
	glTranslatef(0, 0, Size);
	gluCylinder(Quadric, Radius * 2, 0, 1, 40, 40);
	glPopMatrix();

	//Draw y axis
	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);	//rotate over x -90º
	gluCylinder(Quadric, Radius, Radius, Size, 40, 40);
	glTranslatef(0, 0, Size);
	gluCylinder(Quadric, Radius * 2, 0, 1, 40, 40);
	glPopMatrix();



	glPopMatrix();
}

const vector<Point3f>* FaceModel::getFeatures()
{
	return &Model3D;
}

void FaceModel::Init(void) {
	//Clear The Background Color
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glShadeModel(GL_SMOOTH);
}