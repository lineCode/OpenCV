/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	matrixCalculate.cpp
* Brief: Mat��������ͳ�����Եļ���,��ֵ�����Э��������Сֵ��
* Source:
* Status: 
* Date:	[1/21/2015 jmy]
************************************************************************/


#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/*
 *	���ܣ���Mat�ľ�ֵ�ͱ�׼��
 *  ������
 *  ״̬��
 */
int matrixmeanStdDev(Mat&src)
{
	Mat mean, stddev;
	//CvScalar mean, stddev;//�ýṹ��1.0�汾��ʹ��
	meanStdDev(src, mean, stddev);
	std::cout << "mean��" << std::endl << mean << std::endl << "stddev:" << std::endl << stddev << std::endl;
	waitKey();
	return 0;
}

/*
 *	���ܣ�����Mat��Э�������
 *  ������Mat��ÿ��element����һ����������򵥵ı��ǣ�B,G,R����ɫͨ����ֵ
 *  ״̬��Э�������ļ����д��󣬺�����Ĳ�һ��
 */
int matrixcalcCovarMatrix(Mat&src)
{
	Mat_<float> samples[3]; //�����ṩ3��������ÿ��������ά����4
	for (int i = 0; i < 3;i++)
	{
		samples[i].create(1, 4);
		samples[i](0, 0) = i * 4 + 1;
		samples[i](0, 1) = i * 4 + 2;
		samples[i](0, 2) = i * 4 + 3;
		samples[i](0, 3) = i * 4 + 4;
		cout <<" samples[i]:"<<endl<<samples[i] << endl;
	}
	
	Mat_<double> covMat; //����õ���Э������ά�Ⱥ�������ά����ȣ���������������ÿ����������֮��������
	Mat_<double> meanMat;
	calcCovarMatrix(samples,3,covMat, meanMat, CV_COVAR_NORMAL);
	//calcCovarMatrix(&samples, 1, covMat, meanMat, CV_COVAR_NORMAL|CV_COVAR_ROWS);

	string fileName = "covarMatrix.txt";
	FileStorage fs(fileName, FileStorage::WRITE);
	fs << "meanMat" << meanMat;
	fs << "covMat" << covMat;
	fs.release();

	return 0;
}


/*
 *	���ܣ�����Mat��mean��sum
 *  ������
 *  ״̬��
 */
int matrixMeanSum(Mat&src)
{
	Scalar meanv = mean(src);//����ÿ��ͨ���ľ�ֵ
	Scalar sumv = sum(src); //����ÿ��ͨ���ĺ�
	std::cout <<"meanv: "<< meanv << std::endl;
	std::cout <<"sumv: "<< sumv << std::endl;
	
	return 0;
}

/*
 *	���ܣ�����Mat�ľ���ͷ���
 *  ������
 *  ״̬��
 */
int matrixNorm(Mat &src)
{
	Mat mzeros = Mat::zeros(5, 5, CV_8U);
	Mat mones = Mat::ones(5, 5, CV_8U);
	double maxv=norm(mones, NORM_L2);
	cout << maxv << endl;
	

	return 0;
}

//���ܲ�����
//int main()
//{
//	Mat src = imread("samples/rgb.png", 1);
//	//matrixmeanStdDev(src);
//	//matrixMeanSum(src);
//	//matrixNorm(src);
//	matrixcalcCovarMatrix(src);
//
//	std::cin.get();
//	return 0;
//}
