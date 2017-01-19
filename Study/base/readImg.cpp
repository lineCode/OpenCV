#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std; 

//OpenCV1.0�ӿڵ�
void Load_Show_Img(char* filepath);


/*
 *	���ܣ�OpenCV2.0�ӿڶ�ȡ����ʾͼ��
 *  ���룺
 *  �����
 *  ״̬��
 */
int cv2readImg()
{ 
	Mat image;
	image = imread("samples/lena.jpg", CV_LOAD_IMAGE_COLOR);	
	if(!image.data ) {
		cout <<  "Could not open or find the image" << endl ;
		return -1;
	}

	//��������  
	//����Ϊ�����ص�ͼƬ����������֣����ֵ�λ�ã��ı������½ǣ������壬��С����ɫ  
	string words= "good luck";  
	putText( image, words, Point( image.rows/2,image.cols/4),CV_FONT_HERSHEY_COMPLEX, 1, Scalar(255, 0, 0) );  

	namedWindow( "Display window", CV_WINDOW_AUTOSIZE ); // Create a window for display.
	imshow( "Display window", image );                   // Show our image inside it.

	waitKey(0);											 // Wait for a keystroke in the window
	return 0;
}

/*
 *	����:OpenCV1.0�ӿڶ�ȡ����ʾͼ��
 *  ���룺
 *  �����
 *  ״̬��
 */

void cv1readImg()
{
	//cvLoadImage( filename, -1 ); Ĭ�϶�ȡͼ���ԭͨ���� 
	//cvLoadImage( filename, 0 );  ǿ��ת����ȡͼ��Ϊ�Ҷ�ͼ 
	//cvLoadImage( filename, 1 );  ��ȡ��ɫͼ 
    IplImage *pImg;
	if(	(pImg = cvLoadImage("samples/lena.jpg", 1)) != 0 )
	{
		cvNamedWindow( "Image", 1 );
		cvShowImage( "Image", pImg );//��ʾͼ��
		cvWaitKey(0); 
		cvDestroyWindow( "Image" );
		cvReleaseImage( &pImg ); //�ͷ�ͼ��
		cvWaitKey(0); 
	}
	else{
		cout<<"����ʧ��"<<endl;
		cvWaitKey(0); 
	}
}


//���ܲ�����
int main_read()
{
	cv2readImg();
	waitKey(0);
	return 0;
}

