//-----------------------------------������˵����----------------------------------------------
//		��������:������OpenCV���Ž̳�֮ʮ�塿ˮ����ɽ��OpenCV��ˮ����㷨��Floodfill���� ��������Դ�� 
//		��������IDE�汾��Visual Studio 2010
//   		��������OpenCV�汾��	2.4.9
//		2014��6��3�� Created by ǳī
//		ǳī��΢����@ǳī_ë���� http://weibo.com/1723155442/profile?topnav=1&wvr=5&user=1
//		��Դ��http://blog.csdn.net/poem_qianmo/article/details/28261997
//      ˵�����ó���������̣���꣬���������¼��������������⻹ǣ�浽��Ĥ�������ǳ�ȫ��
//----------------------------------------------------------------------------------------------

//-----------------------------------��ͷ�ļ��������֡�---------------------------------------  
//      ����������������������ͷ�ļ�  
//----------------------------------------------------------------------------------------------
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

//-----------------------------------�������ռ��������֡�---------------------------------------  
//      ����������������ʹ�õ������ռ�  
//-----------------------------------------------------------------------------------------------   
using namespace cv;
using namespace std;


//-----------------------------------��ȫ�ֱ����������֡�--------------------------------------  
//      ������ȫ�ֱ�������  
//-----------------------------------------------------------------------------------------------  
//����ԭʼͼ��Ŀ��ͼ���Ҷ�ͼ����ģͼ���ָ���ͼ��
Mat g_srcImage, g_dstImage, g_grayImage, g_maskImage, g_SegmentImage;   
int g_nFillMode = 1;   //��ˮ����ģʽ����ʼ����Ϊ����̶������ģʽ
int g_nLowDifference = 20, g_nUpDifference = 20;//�������ֵ���������ֵ
int g_nConnectivity = 4;//��ʾfloodFill������ʶ���Ͱ�λ����ֵͨ
int g_bIsColor = true;//�Ƿ�Ϊ��ɫͼ�ı�ʶ������ֵ�����������bug�ģ���ԭʼͼ���ǻҶ�ͼ����ת��Ϊ�Ҷ�ͼ��ʱ��ͻ������
bool g_bUseMask = false;//�Ƿ���ʾ��Ĥ���ڵĲ���ֵ
int g_nNewMaskVal = 255;//�µ����»��Ƶ�����ֵ�����Ƶ�������ͼ���ֵ


//-----------------------------------��ShowHelpText( )������----------------------------------  
//      ���������һЩ������Ϣ  
//----------------------------------------------------------------------------------------------  
static void ShowHelpText()  
{  
	//���һЩ������Ϣ  
	printf("\n\n\n\t��ӭ������ˮ���ʾ������~\n\n");  
	printf( "\n\n\t��������˵��: \n\n"  
		"\t\t�����ͼ������- ������ˮ������\n"  
		"\t\t���̰�����ESC��- �˳�����\n"  
		"\t\t���̰�����1��-  �л���ɫͼ/�Ҷ�ͼģʽ\n"  
		"\t\t���̰�����2��- ��ʾ/������Ĥ����\n"  
		"\t\t���̰�����3��- �ָ�ԭʼͼ��\n"  
		"\t\t���̰�����4��- ʹ�ÿշ�Χ����ˮ���\n"  
		"\t\t���̰�����5��- ʹ�ý��䡢�̶���Χ����ˮ���\n"  
		"\t\t���̰�����6��- ʹ�ý��䡢������Χ����ˮ���\n"  
		"\t\t���̰�����7��- ������־���ĵͰ�λʹ��4λ������ģʽ\n"  
		"\t\t���̰�����8��- ������־���ĵͰ�λʹ��8λ������ģʽ\n"  
		"\n\n\t\t\t\t\t\t\t\t byǳī\n\n\n"  
		);  
}  


//-----------------------------------��onMouse( )������--------------------------------------  
//      �����������ϢonMouse�ص�����
//---------------------------------------------------------------------------------------------
static void onMouse( int event, int x, int y, int, void* )
{
	// ��������û�а��£��㷵��
	if( event != CV_EVENT_LBUTTONDOWN )
		return;

	//-------------------��<1>����floodFill����֮ǰ�Ĳ���׼�����֡�---------------
	Point seed = Point(x,y);
	
	//�������ģʽ
	int LowDifference = (g_nFillMode == 0) ? 0 : g_nLowDifference;//�շ�Χ����ˮ��䣬��ֵ��Ϊ0��������Ϊȫ�ֵ�g_nLowDifference
	int UpDifference = (g_nFillMode == 0) ? 0 : g_nUpDifference;//�շ�Χ����ˮ��䣬��ֵ��Ϊ0��������Ϊȫ�ֵ�g_nUpDifference
	
	
	//��ʶ����0~7λΪg_nConnectivity��8~15λΪg_nNewMaskVal����8λ��ֵ��16~23λΪCV_FLOODFILL_FIXED_RANGE����0��
	int flags = g_nConnectivity + (g_nNewMaskVal << 8) + //g_nNewMaskVal����ָ�������Ĥͼ�����
		(g_nFillMode == 1 ? CV_FLOODFILL_FIXED_RANGE : 0);//g_nFillMode���������Ƿ������������صĲ����̶���Χ�����ǵ�ǰ�������������صĲ���򣬾Ϳ��ǵ�ǰ�������������صĲ�����Χ�Ǹ�����
	

	//�������bgrֵ
	int b = (unsigned)theRNG() & 255;//�������һ��0~255֮���ֵ
	int g = (unsigned)theRNG() & 255;//�������һ��0~255֮���ֵ
	int r = (unsigned)theRNG() & 255;//�������һ��0~255֮���ֵ
	Rect ccomp;//�����ػ��������С�߽��������

	//���ػ��������ص���ֵ�����ǲ�ɫͼģʽ��ȡScalar(b, g, r)�����ǻҶ�ͼģʽ��ȡScalar(r*0.299 + g*0.587 + b*0.114)
	Scalar newVal = g_bIsColor ? Scalar(b, g, r) : Scalar(r*0.299 + g*0.587 + b*0.114);
	Mat dst = g_bIsColor ? g_dstImage : g_grayImage;//Ŀ��ͼ�ĸ�ֵ
	
	int area;

	//--------------------��<2>��ʽ����floodFill������-----------------------------
	if( g_bUseMask )
	{
		threshold(g_maskImage, g_maskImage, 1, 128, CV_THRESH_BINARY); //��ֵ����Ĥ
		//floodFill���ص��Ǳ��ػ�����ظ�����ͬʱҲ�޸�������ͼ��
		area = floodFill(dst, g_maskImage, seed, newVal, &ccomp, Scalar(LowDifference, LowDifference, LowDifference),
			Scalar(UpDifference, UpDifference, UpDifference), flags);
		imshow( "mask", g_maskImage );
	}
	else
	{
		//��ʹ����Ĥ��ʱ�򣬰��Ҷ�ͼ�������������Ȳ����
		area = floodFill(dst, seed, newVal, &ccomp, Scalar(LowDifference, LowDifference, LowDifference),
			Scalar(UpDifference, UpDifference, UpDifference), flags);
	}

	imshow("Ч��ͼ", dst); //Ч��ͼ���Ǿ�����ˮ�㷨֮���ͼ��
	cout << area << " �����ر��ػ�\n"; 
}


//-----------------------------------��main( )������--------------------------------------------  
//      ����������̨Ӧ�ó������ں��������ǵĳ�������￪ʼ  
//-----------------------------------------------------------------------------------------------  
int main( int argc, char** argv )
{
	//�ı�console������ɫ  
	system("color 2F");    
	//����ԭͼ
	g_srcImage = imread("samples/text_dete/text_img0265.png", 1);
	if( !g_srcImage.data ) {
		printf("Oh��no����ȡͼƬimage0����~�� \n"); 
		return false; 
	}  

	//��ʾ��������
	ShowHelpText();

	g_srcImage.copyTo(g_dstImage);//����Դͼ��Ŀ��ͼ
	cvtColor(g_srcImage, g_grayImage, COLOR_BGR2GRAY);//ת����ͨ���Ĳ�ɫͼ���Ҷ�ͼ
	//����ͳ�����������
	g_maskImage.create(g_srcImage.rows+2, g_srcImage.cols+2, CV_8UC1);//����ԭʼͼ��ĳߴ�����ʼ����Ĥmask����Ĥͼ����޸�������ˮ�㷨��

	namedWindow( "Ч��ͼ",CV_WINDOW_AUTOSIZE );
	namedWindow("SegmentImage", CV_WINDOW_AUTOSIZE);
	g_maskImage = Scalar::all(255);

    //����Trackbar
	createTrackbar( "�������ֵ", "Ч��ͼ", &g_nLowDifference, 255, 0 );
	createTrackbar( "�������ֵ" ,"Ч��ͼ", &g_nUpDifference, 255, 0 );

	//���ص�����
	setMouseCallback( "Ч��ͼ", onMouse, 0 );

	//ѭ����ѯ����
	while(1)
	{
		//����ʾЧ��ͼ
		imshow("Ч��ͼ", g_bIsColor ? g_dstImage : g_grayImage);
		imshow("test", g_maskImage);
		//g_srcImage.copyTo(g_SegmentImage, g_maskImage); //���ⲿ���õ�ʱ��g_maskImageΪ��,����ͳ����⣬����ͼ�����Ϊ�Ҷ�ͼ��
	    //imshow("SegmentImage", g_SegmentImage);
 	   
	#pragma region key
	//��ȡ���̰���
		int c = waitKey(0);
		//�ж�ESC�Ƿ��£������±��˳�
		if( (c & 255) == 27 )
		{
			cout << "�����˳�...........\n";
			break;
		}
#pragma endregion key

    #pragma region keyboard
//���ݰ����Ĳ�ͬ�����и��ֲ���
		switch( (char)c )
		{
			//������̡�1�������£�Ч��ͼ���ڻҶ�ͼ����ɫͼ֮�以��
		case '1':
			if( g_bIsColor )//��ԭ��Ϊ��ɫ��תΪ�Ҷ�ͼ�����ҽ���Ĥmask����Ԫ������Ϊ0
			{
				cout << "���̡�1�������£��л���ɫ/�Ҷ�ģʽ����ǰ����Ϊ������ɫģʽ���л�Ϊ���Ҷ�ģʽ��\n";
				cvtColor(g_srcImage, g_grayImage, COLOR_BGR2GRAY);
				g_maskImage = Scalar::all(0);	//��mask����Ԫ������Ϊ0
				g_bIsColor = false;	//����ʶ����Ϊfalse����ʾ��ǰͼ��Ϊ��ɫ�����ǻҶ�
			}
			else//��ԭ��Ϊ�Ҷ�ͼ���㽫ԭ���Ĳ�ͼimage0�ٴο�����image�����ҽ���Ĥmask����Ԫ������Ϊ0
			{
				cout << "���̡�1�������£��л���ɫ/�Ҷ�ģʽ����ǰ����Ϊ������ɫģʽ���л�Ϊ���Ҷ�ģʽ��\n";
				g_srcImage.copyTo(g_dstImage);
				g_maskImage = Scalar::all(0);
				g_bIsColor = true;//����ʶ����Ϊtrue����ʾ��ǰͼ��ģʽΪ��ɫ
			}
			break;
			//������̰�����2�������£���ʾ/������Ĥ����,��ʼ��ʱ�����봰�ڶ��Ǻ�ɫ��
		case '2':
			if( g_bUseMask )
			{
				destroyWindow( "mask" );
				g_bUseMask = false;
			}
			else
			{
				namedWindow( "mask", 0 );
				g_maskImage = Scalar::all(0); //��Ĥ�����ú�
				imshow("mask", g_maskImage); //��Ĥ�����л�
				g_bUseMask = true;
			}
			break;
			//������̰�����3�������£��ָ�ԭʼͼ��
		case '3':
			cout << "������3�������£��ָ�ԭʼͼ��\n";
			g_srcImage.copyTo(g_dstImage);
			cvtColor(g_dstImage, g_grayImage, COLOR_BGR2GRAY);
			g_maskImage = Scalar::all(0);
			if (g_bUseMask) //����ʱ����ʾ��Ĥ����״̬���������Ĥ����
			{
				imshow("mask", g_maskImage); //��Ĥ�����л�
			}
			break;
			//������̰�����4�������£�ʹ�ÿշ�Χ����ˮ���
		case '4':
			cout << "������4�������£�ʹ�ÿշ�Χ����ˮ���\n";
			g_nFillMode = 0; //0�����̣�����ֻѡ������ӵ���ͬ�����򣬶��������Ƶ�����
			break;
			//������̰�����5�������£�ʹ�ý��䡢�̶���Χ����ˮ���
		case '5':
			cout << "������5�������£�ʹ�ý��䡢�̶���Χ����ˮ���\n";
			g_nFillMode = 1; //���������ӵ�
			break;
			//������̰�����6�������£�ʹ�ý��䡢������Χ����ˮ���
		case '6':
			cout << "������6�������£�ʹ�ý��䡢������Χ����ˮ���\n";
			g_nFillMode = 2; //�����������ӵ㣬Ҳ������������
			break;
			//������̰�����7�������£�������־���ĵͰ�λʹ��4λ������ģʽ
		case '7':
			cout << "������7�������£�������־���ĵͰ�λʹ��4λ������ģʽ\n";
			g_nConnectivity = 4;
			break;
			//������̰�����8�������£�������־���ĵͰ�λʹ��8λ������ģʽ
		case '8':
			cout << "������8�������£�������־���ĵͰ�λʹ��8λ������ģʽ\n";
			g_nConnectivity = 8;
			break;
		}
#pragma endregion keyboard process
	}

	return 0;
}