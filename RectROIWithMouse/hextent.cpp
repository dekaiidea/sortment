#include "hextent.h"


hextent::hextent()
{
}


hextent::~hextent()
{
}
int* hextent::dhtent(Mat &image, Mat &Yimage, vector<int> p, double thresnum, int n, int Dvalue)
{
	Mat HTImg;
	int* i;
	i =  new int[2];
	threshold(image, HTImg, thresnum, 255, CV_THRESH_BINARY);

	vector<vector<cv::Point>> contours;
	if (n == 1)
	{
		HTImg = ~HTImg;
		cv::findContours(HTImg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

		// 寻找最大连通域  
		double maxArea = 0;
		vector<cv::Point> maxContour;
		for (size_t i = 0; i < contours.size(); i++)
		{
			double area = cv::contourArea(contours[i]);
			if (area > maxArea)
			{
				maxArea = area;
				maxContour = contours[i];
			}
		}
		contours.push_back(maxContour);
		Mat maxcontourImg(HTImg.rows , HTImg.cols , CV_8UC1, Scalar(0));
		drawContours(maxcontourImg, contours, contours.size() - 1, Scalar(255), -1);
		maxcontourImg.copyTo(HTImg);
	}
		
	
	/*Mat element = getStructuringElement(MORPH_RECT,Size(1,2));
	erode(HTImg, HTImg, element);
	dilate(HTImg, HTImg, element);*/
	Mat AvgImgx = HTImg(Range::all(), Range(1, 1));
	Yimage = HTImg(Range(1, 1), Range::all());

	reduce(HTImg, AvgImgx, 1, CV_REDUCE_AVG);
	reduce(HTImg, Yimage, 0, CV_REDUCE_AVG);
	/*erode(Yimage, Yimage, element);
	dilate(Yimage, Yimage, element);*/


	int k = HTImg.rows;
	while (AvgImgx.at<uchar>(Point(0, k-1)) != 0 && k > 1)
	{
		k--;
	}
	if (k < HTImg.rows - Dvalue / 5 && Dvalue!=0)
		k = HTImg.rows -  Dvalue / 5;
	Rect recttemp;
	recttemp.x = 0;
	recttemp.y = k;
	recttemp.width = 1;
	recttemp.height = HTImg.rows-k;
	rectangle(AvgImgx, recttemp, Scalar(0), -1);

	vector <int>().swap(p);
	int mindataone = 0;
	int maxdataone = 0;
	for (int j = 0; j < HTImg.rows; j++)
	{
		int BV = AvgImgx.at<uchar>(Point(0, j));
		if (BV != 0)
			p.push_back(j);
	}
	if (p.size() != 0)
	{
		mindataone = *min_element(p.begin(), p.end());
		maxdataone = *max_element(p.begin(), p.end());
	}
	i[0] = mindataone;
	i[1] = maxdataone;
	return i;
}
