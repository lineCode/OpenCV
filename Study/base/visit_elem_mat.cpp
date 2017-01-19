/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	visit_elem_mat.cpp
* Brief: ����Mat��ÿ�����ص�ֵ���£�
* Source:http://blog.csdn.net/xiaowei_cqu/article/details/19839019
* Status: ����ɨ�貿����ɣ����ٶ�̫����
* Date:	[1/10/2015 jmy]
************************************************************************/

#include <opencv2/core/core.hpp>
#include <highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void visit_elem(string picname)
{
	int div=4;
	Mat img=imread(picname);
	namedWindow("origin",1);
    imshow("origin",img);

	/********��ʽһ��at<typename>(i,j)*******/
	Mat_<uchar> im=img; //���ｫһ�����ص�����ͨ���ֿ�������ԭ����һ�б��3��
	//Mat_<Vec3b> im=img;
	for (int i=0;i<img.rows;i++)
	{
		for (int j=0;j<img.cols;j++)
		{
			img.at<Vec3b>(i,j).val[0]=img.at<Vec3b>(i,j).val[0]/div*div+div/2;
			img.at<Vec3b>(i,j).val[1]=img.at<Vec3b>(i,j).val[1]/div*div+div/2;
			img.at<Vec3b>(i,j).val[2]=img.at<Vec3b>(i,j).val[2]/div*div+div/2;
			
			im(i,j)=im(i,j)/div*div+div/2;
		}
	}
	namedWindow("output",1);
	imshow("output",im);
	waitKey(0);
	
}

int sortm(const int &v1, const int &v2)
{
	return v1 > v2;
}

/*
 *	���ܣ���ɨ��
 *  ���룺
 *  �����
 *  ״̬��
 */

int wscan(char* wnum,char*nnum,string picname)
{
	//int w = 9;
	int w = atoi(wnum);
	int nmean = atoi(nnum);
	cout << "win size=" << w <<"nmean= "<<nmean<<endl;
	int num = 0;
	int mean = 0;
	vector<int> v1;
	Mat img = imread(picname,0);
	Mat result;
	img.copyTo(result);
	//imshow("OriginImg", img);

	int myu,myd,nxl,nxr;//������������
	double duration=static_cast<double>(getTickCount());
	for (int i = 0; i < img.rows; i++)
	{
		myu=(i - w / 2) > 0 ? (i - w / 2) : 0;
		myd=(i + w / 2)>img.rows ? img.rows:(i+w/2);
		for (int j = 0; j < img.cols; j++)
		{
			num++;
			cout << "row: "<<i<<" col: " <<j<< endl;
			//cout << "row " << i << " col: " << j << ": " << int(img.at<uchar>(i, j)) << endl;
			nxl=(j - w / 2) > 0 ? (j - w / 2) : 0;
			nxr=(j + w / 2)>img.cols ? img.cols : (j + w / 2);
			for (int m = myu; m<myd;m++)
			{
				//uchar*data=img.ptr<uchar>(m);
				for (int n = nxl; n<nxr; n++)
				{
					//v1.push_back(data[n]);
					v1.push_back(img.at<uchar>(m, n));
				}
			}
			//��vector�е�Ԫ�ؽ�������ȥ�����ֵ��Ȼ��ȡǰ5��ֵ��ƽ��ֵ
			std::sort(v1.begin(), v1.end(), sortm);
			for (int i = 1; i <= nmean;i++)	{
				mean = mean + v1[i];
			}
			mean = mean / nmean;
			
			//�Ըõ�ҶȽ������¸�ֵ
			result.at<uchar>(i, j) = mean;
			
			//�������
			v1.clear();
		}
	}
	duration=static_cast<double>(getTickCount())-duration;
	duration/=getTickFrequency();
	duration=duration/1000;
	cout<<"��ʱ(��)��"<<duration<<endl;
	imwrite("background.png",result);
	imshow("Background", result);
	waitKey(0);
	return 0;
}

//���ܲ�����
//int main(int argc,char*argv[])
//{
//	//string filename="samples/lena.jpg";
//	//visit_elem(filename);
//	if (argc != 4){
//		cout << "please input the win size and pic" << endl;
//		cout << "usage" << argv[0] << "[win size] [nmeans] [picnanme]" << endl;
//		return -1;
//	}
//	
//	wscan(argv[1],argv[2],argv[3]);
//	return 0;
//}