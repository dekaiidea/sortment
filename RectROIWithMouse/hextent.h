#include <iostream>
#include <fstream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/stitching/stitcher.hpp"
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

#pragma once
class hextent
{
public:
	int* dhtent(Mat &image, vector<int> p, double thresnum,int n = 0,int Dvalue = 0);
	hextent();
	~hextent();
};

