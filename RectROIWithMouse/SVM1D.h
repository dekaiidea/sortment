#include <iostream>
#include <fstream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/stitching/stitcher.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>


using namespace cv;
//using namespace std;



class SVM1D
{
private:
	CvSVM SVM;
	int size;
	float hranges[2];
	const float *ranges[1];
	int channels;

public:
	void getsvm();
	float getrespond(Mat &image);
	~SVM1D();
	SVM1D()
	{
		size = 256;
		hranges[0] = 0.0;
		hranges[1] = 255.0;
		ranges[0] = hranges;
		channels = 0;
	};
	

};
