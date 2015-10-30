#include "SVM1D.h"
#include "hextent.h"



//SVM1D::SVM1D()
//{
//}

bool SortByM1(const Point &v1, const Point &v2)//注意：本函数的参数的类型一定要与vector中元素的类型一致  
{
	return v1.x < v2.x;//升序排列  
}

SVM1D::~SVM1D()
{
}

void SVM1D::getsvm()
{
	MatND dstHist;
	float labels[22] = { -1.0 };
	int j = 0;
	
	Mat trainingDataMat;

	double t = static_cast<double>(getTickCount());
	for (int i = 1; i <= 15; i++)
	{
		stringstream strstr;
		strstr <<"水渍"<< i << ".png";
		Mat histoImg = imread(strstr.str());
		cvtColor(histoImg, histoImg, CV_BGR2GRAY);

		/*	for (int k = 0; k < 7; k++)
		{
		fout << (double)hu_moment[k] << "/n";
		fout << endl;
		fout.close();

		}*/
		calcHist(&histoImg, 1, &channels, Mat(), dstHist, 1, &size, ranges);


		Mat dstImg(256, 256, CV_8U, Scalar(0));//画直方图
		double minValue = 0;
		double maxValue = 0;
		Point maxloc;
		minMaxLoc(dstHist, &minValue, &maxValue, NULL, &maxloc);
		cout << "	     " <<  i << "	     " << maxValue << endl;
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
		rectangle(dstHist, recttemp,Scalar(0),-1);
		minMaxLoc(dstHist, &minValue, &maxValue, NULL, &maxloc);
		int anoThres = maxloc.y;//寻找次峰值

		Scalar avgnum;
		Mat StdDevImg;
		meanStdDev(histoImg, avgnum, StdDevImg);
		double Stdnum = StdDevImg.at<double>(Point(0,0));
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
		Btentnum = htent.dhtent(histoImg, Boundnum, avgnum[0] - 5,1);
		Bnum[0] = *Btentnum;
		Btentnum++;
		Bnum[1] = *Btentnum;
		cout << "	     " << Bnum[0] << "	     " << Bnum[1] << endl;//黑色

		int* Wtentnum;
		int Wnum[2];
		double Tnum = avgnum[0] + 5;
		do
		{
			
			Wtentnum = htent.dhtent(histoImg, Boundnum, Tnum, 0, Bnum[1] - Bnum[0]);
			Wnum[0] = *Wtentnum;
			Wtentnum++;
			Wnum[1] = *Wtentnum;
			cout << "	     " << Wnum[0] << "	     " << Wnum[1] << endl;//白色
			if (Wnum[1]<Bnum[0])
				Tnum = Tnum - 3;
			else
				Tnum = Tnum + 3;
		} while (Wnum[1] - Wnum[0]>=Bnum[1] - Bnum[0] && Wnum[1]>Bnum[0]);
		cout << endl;
		
		if (2 * Bnum[0]<Wnum[0])
		{
			cout << "该点是缺陷1"<<endl;//符合黑上白下一定是缺陷，反之不一定
		}
		else if (Wnum[1]<Bnum[1] && Wnum[0]>Bnum[0] && Wnum[1] - Wnum[0]<0.5*(Bnum[1] - Bnum[0]))
		{
			cout << "该点是水渍1" << endl;
		}
		else
		{
			int ThreStep = maxdata - mindata;
			int StepNum = 30;
			int OrStep = mindata + int(ThreStep/10);
			int Dstep = int(ThreStep/30.0+0.5);
			Mat TempImg;
			histoImg.copyTo(TempImg);
			vector<vector<Point>> contours;
			vector<Vec4i> hierarchy;
			vector<Point> SN;
			Point pointSN, maxPoint = Point(0,0);
			int Marknum = 0;
			for (int i = 0; i < StepNum; i++)
			{
				OrStep = OrStep + Dstep;
				threshold(histoImg, TempImg, OrStep, 255, CV_THRESH_BINARY);
				/*Mat element = getStructuringElement(MORPH_RECT,Size(2,2));
				erode(TempImg, TempImg, cv::Mat());
				dilate(TempImg, TempImg, cv::Mat());*/
				TempImg = ~TempImg;
				findContours(TempImg, contours, hierarchy, RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
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
					if (OrStep > anoThres-5 && OrStep<anoThres +20)
						Dstep = 1;
					else
					{						
						Dstep = int(ThreStep / 30.0+0.5);
					}						
					int k = maxPoint.y;
					int son = hierarchy[k][2];
					if (son >= 0)//父轮廓
					{
						int sonarea = 0;
						for (size_t j = 0; j < contours.size(); j++)
						{
							if (hierarchy[j][3] == k&&contourArea(contours[j])>2)
								sonarea = sonarea + contourArea(contours[j]);
						}
						if (50 * sonarea>maxPoint.x)
							Marknum++;
					}
						
				}
				
			}

			if (Marknum >= 2)//缺陷点也可能偶然出现包含
				cout << "该点是水渍2" << endl;
			else
				cout << "该点是缺陷2" << endl;

		}
		/*stringstream strstrone;
		strstrone << "水渍直方图" << i << ".jpg";
		imwrite(strstrone.str(), dstImg);*/

		

		dstHist = dstHist.t();
		/*normalize(dstHist, dstHist, 255.0, 0.0, NORM_MINMAX);*/
		trainingDataMat.push_back(dstHist);

	}
	t = static_cast<double>(getTickCount() - t);
	t /= cv::getTickFrequency();
	t = t / 1000;
	cout << "处理时间: " << t << "s" << endl;
	//Mat labelsMat(22, 1, CV_32FC1, labels);
	//labels[20] = 1.0, labels[21] = 1.0;

	//CvSVMParams params;
	//params.svm_type = CvSVM::C_SVC;
	//params.kernel_type = CvSVM::LINEAR;
	//params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

	//
	//SVM.train(trainingDataMat, labelsMat, Mat(), Mat(), params);
	/*Mat imageone = (Mat_<float>(1, 2) << 4, 5);*/
	
}

float SVM1D::getrespond(Mat &image)
{
	MatND dstHist;
	calcHist(&image, 1, &channels, Mat(), dstHist, 1, &size, ranges);
	dstHist = dstHist.t();
	float response = SVM.predict(dstHist);
	return response;
}
