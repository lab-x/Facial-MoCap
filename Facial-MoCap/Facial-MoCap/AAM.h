/*
By: Paul Jacob Logas
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
*/

#pragma once
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "TImage.h"
#include "Delaunay.h"
#include <string>

using cv::Mat;
using cv::Point2f;
using cv::Vec3i;
using cv::PCA;
using std::vector;
using std::string;

class AAM
{
public:
	AAM();
	~AAM();

	//Creates the statistical model on which feature recognition and tracking are built
	// Input path is the directory to all annotation files as well as a TrainingSet.txt file
	// describing the files one wishes to use in the model.
	void buildAAM(string filePath);

	//Finds the warp matrix of the input
	Mat warpToMean(TImage* img);

private:
	//Generates the mean model from the mean values of the PCA
	void genMeanShapeModel(TImage * img, int cols, int rows);

	//Loads the point data into the PCA
	void loadPCAPoints(const vector<Point2f>* points, Mat& pcaSet, unsigned index);

	//Finds the point index number of the provided floats
	int findPoint(float x, float y, vector<Point2f>* tImg);

	//This vector holds the order of the triangle vertices and their point indeces for simple warping of triangles.
	vector<Vec3i> meanModel;
	vector<Point2f>* meanPoints;

	Mat meanAppearance;

	/*
	These variables will be used as an offset for the matrix to avoid excess space usage.
	The idea is to use the iteration during generateMeanModel to find the relevent maxes and mins of these values
	then with this information the meanPoints are normalized to this new, smaller frame.
	*/
	int minX, minY, maxX, maxY;

	PCA shapeModel;
	PCA appearModel;


	/*
	* Copyright (c) 2012. Philipp Wagner <bytefish[at]gmx[dot]de>.
	* Released to public domain under terms of the BSD Simplified license.
	*
	* Redistribution and use in source and binary forms, with or without
	* modification, are permitted provided that the following conditions are met:
	*   * Redistributions of source code must retain the above copyright
	*     notice, this list of conditions and the following disclaimer.
	*   * Redistributions in binary form must reproduce the above copyright
	*     notice, this list of conditions and the following disclaimer in the
	*     documentation and/or other materials provided with the distribution.
	*   * Neither the name of the organization nor the names of its contributors
	*     may be used to endorse or promote products derived from this software
	*     without specific prior written permission.
	*
	*   See <http://www.opensource.org/licenses/bsd-license>
	*/
	Mat asRowMatrix(const vector<Mat> src, int rtype, double alpha = 1, double beta = 0);
	Mat norm_0_255(const Mat & src);
};