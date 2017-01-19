/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	regionGrow.cpp
* Brief: ���������㷨��������������������ɫ��
* Source:http://www.pudn.com/downloads225/sourcecode/graph/opencv/detail1058703.html
* Status: ��ɵ��Ժ���⣬�Ż���
* Date:	[1/24/2015 jmy]
************************************************************************/


#include "cv.h"
#include "highgui.h"
#include "cxcore.h"
#include <iostream>

void RegionGrow4(int nSeedX, int nSeedY, int &b, IplImage* lpImage, int* lpNewImage, int nWidth, int nHeight)
{
	int nDx[]={-1,0,1,-1,1,-1,0,1};
	int nDy[]={-1,-1,-1,0,0,1,1,1};
	lpNewImage[nSeedY*nWidth+nSeedX] =b;

	// �����ջ���洢����
	int * pnGrowQueX ;
	int * pnGrowQueY ;
	// ����ռ�
	pnGrowQueX = new int [nWidth*nHeight];
	pnGrowQueY = new int [nWidth*nHeight];
	
	// �����ջ�������յ�
	// ��nStart=nEnd, ��ʾ��ջ��ֻ��һ����
	int nStart;
	int nEnd ;
	//��ʼ��
	nStart = 0 ;
	nEnd = 0 ;
	
	// �����ӵ������ѹ��ջ
	pnGrowQueX[nEnd] = nSeedX;
	pnGrowQueY[nEnd] = nSeedY;
	
	// ��ǰ���ڴ��������
	int nCurrX =nSeedX;
	int nCurrY =nSeedY;
	
	// ѭ�����Ʊ���
	int k ;
	
	// ͼ��ĺ�������,�����Ե�ǰ���ص�4������б���
	int xx,yy;


	while (nStart<=nEnd)
	{
		// ��ǰ���ӵ������
		nCurrX = pnGrowQueX[nStart];
		nCurrY = pnGrowQueY[nStart]; 

		// �Ե�ǰ���4������б���
		for (k=0; k<8; k++) 
		{ 
			// 4�������ص�����
			xx = nCurrX+nDx[k] ;
			yy = nCurrY+nDy[k];
					
			int green_1=((uchar*)(lpImage->imageData + lpImage->widthStep*yy))[xx*3];
			int blue_1=((uchar*)(lpImage->imageData + lpImage->widthStep*yy))[xx*3+1];
			int yellow_1=((uchar*)(lpImage->imageData + lpImage->widthStep*yy))[xx*3+2];
			
			int green_2=((uchar*)(lpImage->imageData + lpImage->widthStep*nCurrY))[nCurrX*3];
			int blue_2=((uchar*)(lpImage->imageData + lpImage->widthStep*nCurrY))[nCurrX*3+1];
			int yellow_2=((uchar*)(lpImage->imageData + lpImage->widthStep*nCurrY))[nCurrX*3+2];

			// �ж�����(xx��yy) �Ƿ���ͼ���ڲ�, �Ƿ��Ѿ������
			// lpNewImage[yy*nWidth+xx]==0 ��ʾ��û�д���
			// �����������ж�����(xx��yy)�͵�ǰ����(nCurrX,nCurrY) ����ֵ��ľ���ֵ
			if ( (xx < nWidth) && (xx>=0) && (yy>=0) && (yy<nHeight) 
				&& lpNewImage[yy*nWidth+xx]==0
				&& abs(green_1-green_2)<10
				&& abs(blue_1-blue_2)<10
				&& abs(yellow_1-yellow_2)<10)
			{
				nEnd++;
				// ����(xx��yy) ѹ��ջ
				pnGrowQueX[nEnd] = xx; 
				pnGrowQueY[nEnd] = yy;
				lpNewImage[yy*nWidth+xx] = b;
			}
		}
		nStart++;
	}
	b++;
	// �ͷ��ڴ�
	delete []pnGrowQueX;
	delete []pnGrowQueY;
	pnGrowQueX = NULL ;
	pnGrowQueY = NULL ;
}
void RegionGrow_ColorEx(IplImage *lpImage, IplImage* dstImg, int nWindth, int nHeight)
{
	int *Flag = new int[nHeight*nWindth];
	//memset(Flag,0,nHeight*nWindth);
	int i, j;
	int m = 1;
	for (i = 0; i < nHeight; i++)
	{
		for (int j = 0; j < nWindth; j++)
		{
			Flag[i*nWindth + j] = 0;
		}
	}


	for (i = 0; i < nHeight; i = i + 10)
	{
		for (j = 0; j < nWindth; j = j + 2)
		{
			if (Flag[i*nWindth + j] == 0)
			{
				RegionGrow4(j, i, m, lpImage, Flag, nWindth, nHeight);	//����������������
			}
		}
	}

	//����ͬ��ĻҶ�ƽ��ֵ
	long *sum_1 = new long[m + 1];
	long *sum_2 = new long[m + 1];
	long *sum_3 = new long[m + 1];
	int  *count = new int[m + 1];
	for (i = 0; i <= m; i++){

		count[i] = 0;
	}
	for (i = 0; i <= m; i++){
		sum_1[i] = 0;
		sum_2[i] = 0;
		sum_3[i] = 0;
	}
	for (i = 0; i < nHeight; i++)
	{
		for (j = 0; j < nWindth; j++)
		{
			int t = Flag[i*nWindth + j];
			if (t > 0)
			{
				int a = ((uchar*)(lpImage->imageData + lpImage->widthStep*i))[j * 3];
				int b = ((uchar*)(lpImage->imageData + lpImage->widthStep*i))[j * 3 + 1];
				int c = ((uchar*)(lpImage->imageData + lpImage->widthStep*i))[j * 3 + 2];

				sum_1[t] += a;
				sum_2[t] += b;
				sum_3[t] += c;
				count[t]++;
			}
		}
	}

	for (i = 0; i < m + 1; i++)
	{
		if (count[i]>0)
		{
			sum_1[i] = sum_1[i] / count[i];
			sum_2[i] = sum_2[i] / count[i];
			sum_3[i] = sum_3[i] / count[i];

		}
	}

	// ��������
	cvZero(dstImg);
	int a = 0;
	for (i = 0; i < nHeight; i++)
	{
		for (j = 0; j < nWindth; j++)
		{
			int t = Flag[i*nWindth + j];
			if (t > 0)
			{
				((dstImg->imageData + dstImg->widthStep*i))[j * 3] = (uchar)sum_1[t];
				((dstImg->imageData + dstImg->widthStep*i))[j * 3 + 1] = (uchar)sum_2[t];
				((dstImg->imageData + dstImg->widthStep*i))[j * 3 + 2] = (uchar)sum_3[t];
			}
		}
	}

	delete[]sum_1;
	delete[]sum_2;
	delete[]sum_3;
	delete[]count;
	delete[]Flag;
}


//���ܲ�����
//int main(int argc, char* argv[])
//{
//	IplImage* src = cvLoadImage("10.bmp");
//	IplImage* dst=cvCreateImage(cvSize(src->width,src->height),8,3);
//	cvNamedWindow("SRC",1);
//	cvShowImage("SRC",src);
//
//	RegionGrow_ColorEx(src,dst,src->width,src->height);
//	cvSaveImage("RegionGrow.bmp",dst);
//	cvNamedWindow("RegionGrow",1);
//	cvShowImage("RegionGrow",dst);
//	cvWaitKey(-1);
//	return 0;
//}

