#include "hextent.h"


hextent::hextent()
{
}


hextent::~hextent()
{
}
int* hextent::dhtent(Mat &image, vector<int> p,double thresnum,int n,int Dvalue )
{
	Mat HTImg;
	int* i;
	i =  new int[2];
	threshold(image, HTImg, thresnum, 255, CV_THRESH_BINARY);
	if (n == 1)
		HTImg = ~HTImg;
	erode(HTImg, HTImg, cv::Mat(), cv::Point(-1, -1), 3);
	dilate(HTImg, HTImg, cv::Mat());
	Mat AvgImg = HTImg(Range::all(), Range(1, 1));
	reduce(HTImg, AvgImg, 1, CV_REDUCE_AVG);

	int k = HTImg.rows;
	while (AvgImg.at<uchar>(Point(0, k-1)) != 0 && k > 1)
	{
		k--;
	}
	if (k < HTImg.rows -  Dvalue / 5)
		k = HTImg.rows -  Dvalue / 5;
	Rect recttemp;
	recttemp.x = 0;
	recttemp.y = k;
	recttemp.width = 1;
	recttemp.height = HTImg.rows-k;
	rectangle(AvgImg, recttemp, Scalar(0), -1);

	vector <int>().swap(p);
	int mindataone = 0;
	int maxdataone = 0;
	for (int j = 0; j < HTImg.rows; j++)
	{
		int BV = AvgImg.at<uchar>(Point(0, j));
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
