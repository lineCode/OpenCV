/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	rgb2hsv.cpp
* Brief: opencv����HSV�ķ�ɫ�ָ�,�ؼ�����ѧϰɫ�ʿռ�ת��
* Source:http://www.cnblogs.com/xingma0910/archive/2013/07/18/3197502.html
* Status: 
* Date:	[1/10/2015 jmy]
************************************************************************/

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


/*
 *	���ܣ���HSV��ɫ�ռ��ͼ����з�ɫģ�ͷָ�
 *  ���룺src-�������ͼ��
 *  �����imgout-���ͼ��
 *  ״̬��
 */
IplImage* SkinSegmentHSV(IplImage* src,IplImage* imgout)
{
	//����һЩ�м�ָ�룬ָ��������е��м����
	IplImage* HSV = NULL;  
	IplImage* HImg= NULL;
	IplImage* SImg= NULL;
	IplImage* VImg= NULL;
	IplImage* result = NULL;//ָ�����Ľ��

	if (!src||!imgout)  {   return NULL;  }

	//��ȡ����ͼ��Ĵ�С
	CvSize SrcSize = cvGetSize(src);

	//Ϊ�м���ָ�����洢�ռ�
	HSV = cvCreateImage(SrcSize,8,3);
	HImg= cvCreateImage(SrcSize,8,1);  
	SImg= cvCreateImage(SrcSize,8,1);  
	VImg= cvCreateImage(SrcSize,8,1);  

	result= cvCreateImage(cvGetSize(imgout),8,1);//����Ĭ�ϵ�BGR

	//��ͼ���RGB��ɫ�ռ�ת����HSV�ռ�
	cvCvtColor(src,HSV,CV_BGR2HSV);
	//��HSV�ֽ�Ϊ���ŵ�ͨ��ͼ�񣬱��ں�����д���  
	cvSplit(HSV,HImg,SImg,VImg,NULL);
	
	int i, j;  int value = 0;

	//��ͨ��H���д���ɫ�� 
	for (i = 0; i < HImg->height; i++)
	{  
		for (j = 0; j < HImg->width; j++)
     	{   
			value = cvGetReal2D(HImg,i,j);   
			if (value >=0 && value <= 25){     
				 *(HImg->imageData+i*HImg->widthStep+j) = 255;  //��ֵ�� 
			}    
			else {     
				*(HImg->imageData+i*HImg->widthStep+j) = 0;    
			}               
		}  
	}

	//��ͨ��S���д������Ͷ�
	for (i = 0; i < SImg->height; i++)  
	{   
		for (j = 0; j < SImg->width; j++)
	   {    
			value = cvGetReal2D(SImg, i, j);
			if (value >= 26 && value <= 200) {
				*(SImg->imageData + i*SImg->widthStep + j) = 255;
			}
			else {
				*(SImg->imageData + i*SImg->widthStep + j) = 0;
			}
	   }
	}
	//��ͨ��V���д������� 
	for (i = 0; i < VImg->height; i++)  
	{  
		for (j = 0; j < VImg->width; j++)   
		{   
			value = cvGetReal2D(VImg,i,j);    
			if (value >=20 && value <= 200)	{     
				*(VImg->imageData+i*VImg->widthStep+j) = 255;     
			}     
			else {    
				*(VImg->imageData+i*VImg->widthStep+j) = 0;     
			}    
		}   
	}

	//HSV����õ����Ľ����
	cvAnd(HImg,SImg,result,0);  
	cvAnd(VImg,result,result,0); 

	//�����õ��Ľ��������̬ѧ��ʴ����ȥ��С���򲿷�
	cvErode(result,result);  
	cvErode(result,result); 
	cvErode(result,result);  //��ʴ���Σ�����һ��
	cvDilate(result,result);

	//�������Ľ����ֵ�����ͼ��
	cvCopy(result,imgout);  

	//�ͷ������Դ  
	cvReleaseImage(&src);  
	cvReleaseImage(&HSV);  
	cvReleaseImage(&HImg); 
	cvReleaseImage(&SImg); 
	cvReleaseImage(&VImg);  
	cvReleaseImage(&result);

	//���ش����Ľ��
	return imgout;

}

//���ܲ�����
//int main()
//{
//	IplImage*src=cvLoadImage("samples/hsv.png");
//	IplImage*output=cvCreateImage(cvGetSize(src),src->depth,1);
//	//cvCvtColor(output,output,CV_BGR2HSV);
//	SkinSegmentHSV(src,output);
//	waitKey();
//}