/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	pyr.cpp
* Brief: 
* Source:
* Status: 
* Date:	[1/26/2015 jmy]
************************************************************************/
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int pyr(Mat&src)
{
	Mat pyrdown, pyrup, pyrbuild,diff,dst;
	pyrDown(src, pyrdown);
	pyrUp(src, pyrup);
	pyrUp(pyrdown, pyrbuild);
	//absdiff(src, pyrbuild(Range(0, src.rows), Range(0, src.cols)),diff);
	diff.create(src.size(), src.type());
	resize(pyrbuild, pyrbuild,src.size(),0,0,INTER_AREA); //�����ع����ͼ��ʹ�����ԭͼ��һ���Ĵ�С
	absdiff(src, pyrbuild, diff);
	

	//�µ�Ӧ��:��������ֵ�˲�����Ϊmean shift�ָ��㷨�ĳ�ʼ������
	//pyrMeanShiftFiltering()��ʹ�ã��ο�Detection/filtersmooth/meanshift_smooth_segmentaiton.cpp�ļ�


	imshow("pyrdown", pyrdown);
	imshow("pyrup",pyrup);
	imshow("pyrbuild", pyrbuild);
	imshow("pydiff", diff); //�����ʾ�ع���ͼ���ԭͼ��Ĳ����С
	
	waitKey();

	return 0;
}

//���ܲ�����
//int main()
//{
//	Mat src = imread("samples\\200.png");
//	pyr(src);
//
//	return 0;
//}