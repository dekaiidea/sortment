#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <opencv2/ml/ml.hpp>
#include "SVM1D.h"

using namespace cv;
using namespace std;


int main(int argc, char *argv[])
{
	
	
	SVM1D YON;//Yes Or No
	YON.getsvm();
	Mat testImg;
	float num[16];
	float zeroone[16] = {0.0};
	zeroone[14] = 1.0, zeroone[15] = 1.0;
	Mat zeroMat(16, 1, CV_32FC1, zeroone);

	//for (int n = 1; n <= 10; n++)
	//{ 
	//	stringstream strstrtwo;
	//	strstrtwo <<"к╝ву"<< n << ".jpg";
	//	testImg = imread(strstrtwo.str());
	//	float response = YON.getrespond(testImg);
	//	if (int(response) == 1)
	//	{
	//		cout << "1" << "	" << n << endl;
	//		num[n-23] = 1.0;
	//	}
	//	else
	//	{
	//		cout << "0" << "	" << n << endl;
	//		num[n-23] = 0.0;
	//	}	
	//}
	/*Mat correctMat(16, 1, CV_32FC1, num);
	normalize(correctMat, correctMat, 255.0, 0.0, NORM_MINMAX);
	normalize(zeroMat, zeroMat, 255.0, 0.0, NORM_MINMAX);

	Mat numMat;
	bitwise_xor(zeroMat, correctMat, numMat);
	
	int a = countNonZero(numMat);
	double k = (16.0 - a)/16.0;
	cout << k<< endl;*/
	getchar();
	return 0;
}
