/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	frameMean.cpp
* Brief: ƽ��ֵȥ�룬��ȥ����
* Source:http://blog.csdn.net/xiaowei_cqu/article/details/7610665
* Status: 
* Date:	[1/15/2015 jmy]
************************************************************************/

#include <cv.h>
#include <highgui.h>
#include <iostream>

using std::cout;
using std::endl;



/*
 *	���ܣ�������Ƶ��ȡָ��֮֡���ƽ��ֵ
 *  ������
 *  ״̬:
 */
int frameMean()
{
	CvCapture* capture = cvCaptureFromFile( "samples/video/test_o2.mp4");//"test.avi");//
	IplImage* frame = NULL;
	IplImage* framegray = NULL;
	IplImage * framesum = NULL;
	cvNamedWindow("diff", 1);

	int start = 0, curframe = 0, meannum = 6;
	while (cvGrabFrame(capture)) 
	{
		curframe = cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES);
		start = (curframe - meannum > 0) ? (curframe - meannum) : 0;
		cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, start);
		frame = cvRetrieveFrame(capture);// ��ȡ��ǰ֡
		framegray= cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
		cvCvtColor(frame, framegray, CV_BGR2GRAY);
		cout << "==================================" << endl;
		cout << "��ǰ֡��" << curframe << endl
			 << "ȡƽ����֡��ʼ��: " << start << endl;
		int countstart = start;
		while (start<curframe)
		{
			cout << "��ʼ�ۻ�" << start << endl;
			frame = cvRetrieveFrame(capture);// ��ȡ��ǰ֡
			cvCvtColor(frame, framegray, CV_BGR2GRAY);
			if (framesum == NULL){
				framesum = cvCreateImage(cvGetSize(frame), IPL_DEPTH_32F, 1);
				cvZero(framesum);
			}
			cvAcc(framegray, framesum);

			//char testname[100];
			//sprintf(testname, "%s%d%s", "image", start, ".jpg");
			//cvShowImage(testname, frame);
			//cvSaveImage(testname, frame);

			cvGrabFrame(capture);
			start++;
		}
		cout << "�ۻ���ϣ���ʼ����ƽ��" << curframe << "-[" << countstart << "---" << curframe - 1 <<"]"<< endl;
		IplImage * frameavg = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
		if (curframe>meannum){
			cvConvertScale(framesum, frameavg, 1.0 / meannum);
		}
		else{
			cvConvertScale(framesum, frameavg, 1.0 / curframe);
		}

		cvAbsDiff(framegray,frameavg,framegray);
		//cvThreshold(framegray,framegray,155,255,CV_THRESH_BINARY);
		cvShowImage("diff", framegray);
		//cvSaveImage("diff_4.jpg", frame);
		cvReleaseImage(&frameavg);
		cvReleaseImage(&framegray);
		
		char c = cvWaitKey(10);
		if (c==27)	{
			break;
		}
		
	}
	
	cvReleaseCapture(&capture);
	cvDestroyWindow("diff");
	return 0;
}

//���ܲ�����
