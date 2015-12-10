#pragma once
/***************************************************************************
*   Copyright (C) 2007 by pedromartins   *
*   pedromartins@isr.uc.pt   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/



//Compile with OpenGL and OpenCV libs
// g++ glAnthropometric3DModel.cpp -I /usr/local/include/opencv/ -L /usr/local/lib -lcv -lhighgui -lcvaux -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm -o glAnthropometric3DModel


#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>


#include <opencv\cv.h>
#include <opencv\cvaux.h>
#include <opencv2\highgui\highgui.hpp>


#define cvM32f(Matrix,i,j) CV_MAT_ELEM(*(Matrix),float,(i),(j))


//Angle Conversion Macros
#define PI (3.141592653589793)

#define rad2deg(Theta) (Theta)*180/PI
#define deg2rad(Theta) (Theta)*PI/180

using cv::Mat;
using cv::Point3f;
using std::vector;

class FaceModel
{
public:
	FaceModel();
	~FaceModel();

	//Display Matrix
	void cvPrintMat(Mat* Matrix);

	void Anthropometric3DModel(int LineWidth);

	void DrawAxes(float Size, float Radius);

	const vector<Point3f>* getFeatures();

	//Set Texture Mapping Parameters
	void Init(void);

	GLfloat WorldTx = 0, WorldTy = 0, WorldTz = 0, WorldRoll = 0, WorldPitch = 0, WorldYaw = 0;
	GLfloat FaceTx = 0, FaceTy = 0, FaceTz = 0, FaceRoll = 0, FacePitch = 0, FaceYaw = 0;
private:
	static const vector<Point3f> Model3D;
	static const int MeanDelaunayTriangles[95][3];
	GLUquadricObj*Quadric = gluNewQuadric();
};

