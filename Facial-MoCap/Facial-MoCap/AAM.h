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

	static int findPoint(float x, float y, vector<Point2f>* tImg);

private:
	//Generates the mean model, usually from the first image in the TImage vector
	// However could be altered to be more versatile.
	void generateMeanModel(TImage* img);

	//This vector holds the order of the triangle vertices and their point indeces for simple warping of triangles.
	vector<Vec3i> meanModel;
};