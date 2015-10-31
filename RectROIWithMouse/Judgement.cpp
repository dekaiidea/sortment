#include "Judgement.h"
#include "hextent.h"


bool SortByM1(const Point &v1, const Point &v2)//注意：本函数的参数的类型一定要与vector中元素的类型一致  
{
	return v1.x < v2.x;//升序排列  
}


//Judgement::Judgement()
//{
//}


Judgement::~Judgement()
{
}


int Judgement::JudgementYON(Mat &image,int n,int m)
{
	int success = 0;
	MatND dstHist;
	Mat histoImg = image.clone();
	calcHist(&histoImg, 1, &channels, Mat(), dstHist, 1, &size, ranges);
	Mat dstImg(256, 256, CV_8U, Scalar(0));//画直方图
	double minValue = 0;
	double maxValue = 0;
	Point maxloc;
	minMaxLoc(dstHist, &minValue, &maxValue, NULL, &maxloc);
	cout << "	     " << n << "." << m<< "	     " << maxValue << endl;
	int hpt = saturate_cast<int>(0.9 * 256);
	vector<int> Boundnum;
	int Lnum = 0;
	for (int j = 0; j < 256; j++)
	{
		float binValue = dstHist.at<float>(j);
		int realValue = saturate_cast<int>(binValue * hpt / maxValue);
		if (realValue != 0)
		{
			rectangle(dstImg, Point(j, 255), Point(j, 256 - realValue), Scalar(255));
			Boundnum.push_back(j);
		}

	}
	int  maxdata = *max_element(Boundnum.begin(), Boundnum.end());
	int  mindata = *min_element(Boundnum.begin(), Boundnum.end());//寻找直方图动态范围

	Rect recttemp;
	recttemp.x = maxloc.x;
	recttemp.y = maxloc.y - int((maxdata - mindata)*0.15);
	recttemp.width = 1;
	recttemp.height = int((maxdata - mindata)*0.3);
	rectangle(dstHist, recttemp, Scalar(0), -1);
	minMaxLoc(dstHist, &minValue, &maxValue, NULL, &maxloc);
	int anoThres = maxloc.y;//寻找次峰值

	Scalar avgnum;
	Mat StdDevImg;
	meanStdDev(histoImg, avgnum, StdDevImg);
	double Stdnum = StdDevImg.at<double>(Point(0, 0));
	cout << "	     " << mindata << "	     " << maxdata << endl;
	cout << "	     " << avgnum[0] << "	     " << Stdnum << endl;


	/*Mat HTImg;
	threshold(histoImg, HTImg, avgnum[0] + 5, 255, CV_THRESH_BINARY);
	erode(HTImg, HTImg, cv::Mat(), cv::Point(-1, -1), 3);
	dilate(HTImg, HTImg, cv::Mat());
	Mat AvgImg = HTImg(Range::all(), Range(1, 1));
	reduce(HTImg, AvgImg, 1, CV_REDUCE_AVG);
	vector <int>().swap(Boundnum);
	int  maxdataone, mindataone = 0;
	for (int j = 0; j < HTImg.rows; j++)
	{
	int BV = AvgImg.at<uchar>(Point(0,j));
	if (BV != 0)
	Boundnum.push_back(j);
	}
	if (Boundnum.size() != 0)
	{
	maxdataone = *max_element(Boundnum.begin(), Boundnum.end());
	mindataone = *min_element(Boundnum.begin(), Boundnum.end());
	}*/

	hextent htent;//找范围类


	/*Mat LTImg;
	threshold(histoImg, LTImg, avgnum[0] - 5, 255, CV_THRESH_BINARY);
	LTImg = ~LTImg;
	erode(LTImg, LTImg, cv::Mat(), cv::Point(-1, -1), 3);
	dilate(LTImg, LTImg, cv::Mat());
	Mat AvgImgtwo = LTImg(Range::all(), Range(1, 1));
	reduce(LTImg, AvgImgtwo, 1, CV_REDUCE_AVG);
	vector <int>().swap(Boundnum);
	int  maxdatatwo, mindatatwo = 0;
	for (int j = 0; j < LTImg.rows; j++)
	{
	int binValue = AvgImgtwo.at<uchar>(Point(0, j));
	if (binValue != 0)
	Boundnum.push_back(j);
	}
	if (Boundnum.size() != 0)
	{
	maxdatatwo = *max_element(Boundnum.begin(), Boundnum.end());
	mindatatwo = *min_element(Boundnum.begin(), Boundnum.end());
	}*/
	int* Btentnum;
	int Bnum[2];
	int Dvulue = 0;
	Mat BAvgImgy;
	Btentnum = htent.dhtent(histoImg, BAvgImgy, Boundnum, avgnum[0] - 5, 1);
	Bnum[0] = *Btentnum;
	Btentnum++;
	Bnum[1] = *Btentnum;
	cout << "	     " << Bnum[0] << "	     " << Bnum[1] << endl;//黑色

	int* Wtentnum;
	int Wnum[2];
	Mat WAvgImgy;
	double Tnum = avgnum[0] + 5;
	do
	{

		Wtentnum = htent.dhtent(histoImg, WAvgImgy, Boundnum, Tnum, 0, Bnum[1] - Bnum[0]);
		Wnum[0] = *Wtentnum;
		Wtentnum++;
		Wnum[1] = *Wtentnum;
		cout << "	     " << Wnum[0] << "	     " << Wnum[1] << endl;//白色
		if (Wnum[1]<Bnum[0])
			Tnum = Tnum - 3;
		else
			Tnum = Tnum + 3;
	} while (Wnum[1] - Wnum[0] >= Bnum[1] - Bnum[0] && Wnum[1]>Bnum[0]);
	cout << endl;

	Mat AndImg;
	bitwise_and(BAvgImgy, WAvgImgy, AndImg);
	int AndNum = countNonZero(AndImg);


	if (2 * Bnum[0]<Wnum[0] && AndNum>0.2*histoImg.cols)
	{
		cout << "该点是缺陷1" << endl;//符合黑上白下一定是缺陷，反之不一定，补充满足白色区域要出现在黑色区域下方
	}
	else if (Wnum[1]<Bnum[1] && Wnum[0]>Bnum[0] && Wnum[1] - Wnum[0]<0.5*(Bnum[1] - Bnum[0]))
	{
		cout << "该点是水渍1" << endl;
		success++;
	}
	else
	{
		int ThreStep = maxdata - mindata;
		int StepNum = 30;
		int OrStep = mindata + int(ThreStep / 10);
		int Dstep = int(ThreStep / 30.0 + 0.5);
		Mat TempImg;
		histoImg.copyTo(TempImg);
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		Point pointSN, maxPoint = Point(0, 0);
		int Marknum = 0;
		for (int i = 0; i < StepNum; i++)
		{
			vector<Point> SN;
			OrStep = OrStep + Dstep;
			threshold(histoImg, TempImg, OrStep, 255, CV_THRESH_BINARY);

			
			/*Mat element = getStructuringElement(MORPH_RECT,Size(2,2));
			erode(TempImg, TempImg, cv::Mat());
			dilate(TempImg, TempImg, cv::Mat());*/
			TempImg = ~TempImg;


			/*stringstream strstrone;
			strstrone << "水渍动态图" << i << ".jpg";
			imwrite(strstrone.str(), TempImg);*/

			Mat BoundImg(TempImg.rows, TempImg.cols, CV_8UC1, Scalar(255));
			Rect Wrect;
			Wrect.x = 1;
			Wrect.y = 1;
			Wrect.width = BoundImg.cols - 2;
			Wrect.height = BoundImg.rows - 2;
			rectangle(BoundImg, Wrect, Scalar(0), -1);
			
			Mat PlusImg(TempImg.rows+2, TempImg.cols+2, CV_8UC1, Scalar(255));
			Mat PlusROI = PlusImg(Rect(1, 1, TempImg.cols, TempImg.rows));
			TempImg.copyTo(PlusROI);

			findContours(PlusImg, contours, hierarchy, RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
			for (size_t j = 0; j < contours.size(); j++)
			{
				double area = cv::contourArea(contours[j]);
				pointSN.x = int(area);
				pointSN.y = j;
				SN.push_back(pointSN);
			}

			if (contours.size() != 0)
			{
				sort(SN.begin(), SN.end(), SortByM1);
				maxPoint = SN.back();
				if (OrStep > anoThres - 5 && OrStep<anoThres + 20)
					Dstep = 1;
				else
				{
					Dstep = int(ThreStep / 30.0 + 0.5);
				}
				int k = maxPoint.y;
				

				Mat MarkImg(TempImg.rows, TempImg.cols, CV_8UC1, Scalar(0));
				drawContours(MarkImg, contours, k, Scalar(255), -1);
				bitwise_and(BoundImg, MarkImg, MarkImg);
				int Mbound = 0;//判断轮廓是否到边界
				Mbound = countNonZero(MarkImg);
				if (Mbound>0.5*(histoImg.cols + histoImg.rows))
					break;
				
				/*Mat Ione = BoundImg(Range::all(), Range(1, 1));
				if (countNonZero(Ione)>0.4*Ione.rows);
					Mbound++;		
				Mat Itwo = BoundImg(Range::all(), Range(BoundImg.cols, BoundImg.cols));
				if (countNonZero(Itwo)>0.4*Itwo.rows);
					Mbound++;
				Mat Ithree = BoundImg(Range(1,1), Range::all());
				if (countNonZero(Ithree)>0.4*Ithree.cols);
					Mbound++;
					Mat Ifour = BoundImg(Range(BoundImg.rows, BoundImg.rows), Range::all());
				if (countNonZero(Ifour)>0.4*Ifour.cols);
					Mbound++;*/
				/*if (Mbound > 1)
					break;*/

				int son = hierarchy[k][2];
				if (son >= 0)//父轮廓
				{
					int sonarea = 0;
					for (size_t j = 0; j < contours.size(); j++)
					{
						if (hierarchy[j][3] == k&&contourArea(contours[j])>2)
							sonarea = sonarea + contourArea(contours[j]);
					}
					if (50 * sonarea>maxPoint.x)//此处忽略一些偶然出现的中空点
						Marknum++;
				}

			}

		}

		if (Marknum >= 2)//缺陷点也可能偶然出现包含
		{
			cout << "该点是水渍2" << endl;
			success++;
		}	
		else
			cout << "该点是缺陷2" << endl;
	}
	return success;
}