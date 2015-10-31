#include "SVM1D.h"
#include "hextent.h"
#include "Judgement.h"



//SVM1D::SVM1D()
//{
//}



SVM1D::~SVM1D()
{
}

void SVM1D::getsvm()
{
	//float labels[22] = { -1.0 };
	//int j = 0;
	
	Mat trainingDataMat;
	int sum = 0;
	double t = static_cast<double>(getTickCount());
	for (int i = 1; i <= 12; i++)
	{
		stringstream strstr;
		strstr <<"缺陷"<< i << ".png";
		Mat histoImg = imread(strstr.str());
		cvtColor(histoImg, histoImg, CV_BGR2GRAY);

		int n = 0;
		for (int j = 0; j <= 4; j++)
		{
			for (int k = 0; k <= 4; k++)
			{
				Rect recttemp;
				recttemp.x = j;
				recttemp.y = k;
				recttemp.width = histoImg.cols - j-1;
				recttemp.height = histoImg.rows - k-1;
				Mat imageROI = histoImg(Rect(recttemp));
				Judgement JYON;
				n++;
				if (JYON.JudgementYON(histoImg, i, n))
					sum++;
			}
			
		}
		
		/*	for (int k = 0; k < 7; k++)
		{
		fout << (double)hu_moment[k] << "/n";
		fout << endl;
		fout.close();

		}*/
		


		

		
		/*stringstream strstrone;
		strstrone << "水渍直方图" << i << ".jpg";
		imwrite(strstrone.str(), dstImg);*/

		

		//dstHist = dstHist.t();
		/*normalize(dstHist, dstHist, 255.0, 0.0, NORM_MINMAX);*/
		//trainingDataMat.push_back(dstHist);

	}
	cout << "成功识别数: " << sum << "个" << endl;
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
