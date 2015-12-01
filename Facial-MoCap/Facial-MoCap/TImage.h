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
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using cv::Mat;
using cv::Point2f;
using std::vector;
using std::string;

class TImage
{
public:
	TImage();
	TImage(string filePath);
	~TImage();

	/*
	Loads all training images found in the provided directory, following the format
	laid out in TrainingSet.txt file.
	Input format is: ../path/to/
	*/
	static vector<TImage*>* loadAllTraining(string filePath);

	/*
	Be sure to call this function whenever you expect the generated vector
	to go out of scope as it is a large memory object.
	*/
	static void deleteAllTraining(vector<TImage*>* imgs);

	Mat* const& getImg();

	vector<Point2f>* const& getPoints();
	
	void loadWarpedImg(Mat warpedImg);

	int getRows();
	int getCols();

private:
	//Image loading function
	bool loadImg(string* filePath);

	//Annotation loading
	bool loadFeaturePoints(string* filePath);

	//Matrix that holds image data
	Mat img;

	Mat warpedImg;

	//Vector of triangles in the image
	vector<Point2f> points;
};