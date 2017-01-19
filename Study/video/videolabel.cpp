//Tools for Labeling Video
//Coded by L. Wei
//Date: 9/4/2013
/*
 *���ܣ���Ƶ��ע�ͱ�ע�������������ļ�
 *���ԣ�http://blog.csdn.net/xiaowei_cqu/article/details/8778976
 *Satus:�˹��ܰ��������ļ���д�Ͷ�
 *Info:[11/12/2014 jmy]
 */
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <time.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

//Global variables
bool is_drawing=false;
vector<Rect> biaozhu_boxs; //���еı�ע���ο�
Rect drawing_box;
Mat img_original,img_drawing;

CvFileStorage *fs; //�����ļ�
typedef struct UnitConf
{
	char meaning[20];
	Rect loc;
} s_unitInfo;
s_unitInfo meanloc; //ÿ������������λ��
vector<s_unitInfo> config;

//˵��
static void help()
{
	cout << "This program designed for labeling video \n"
		<<"Coded by L. Wei on 9/4/2013\n"<<endl;
	
	cout<<"Use the mouse to draw rectangle on the image for labeling.\n"<<endl;

	cout << "Hot keys: \n"
		"\tESC - quit the program\n"
		"\tSpace Key - pause/start\n"
		"\tz - undo the last label\n"
		"\tc - clear all the labels\n"
		<<endl;
}

/*
 *	���ܣ�дXML�ļ�
 *  ������
 *  ״̬��
 */
static int xml_write(char* meaning, const Rect loc)
{
	
   // ��ʼд�ṹ��������ͼmap��Ҳ���������ֵ�����ڵ㼯��  
	/*
	const char *attr[4]={"imagename=photoname","width=1","height=2","AreaNum=9"};
	CvAttrList imageinfo=cvAttrList(attr,0);
	*/
	cvStartWriteStruct(fs, meaning, CV_NODE_MAP, meaning, cvAttrList(0, 0));
	//cvWriteComment(fs, "text area info", 1);
	//cvWriteString(fs, "Object", meaning);
	cvWriteInt(fs, "x", loc.tl().x);
	cvWriteInt(fs, "y", loc.tl().y);
	cvWriteInt(fs, "width", loc.width);
	cvWriteInt(fs, "height", loc.height);

	//���нṹ����
	//cvWriteInt(fs, "loc", 4);
	int area_info[4] = { loc.tl().x, loc.tl().y, loc.width, loc.height };
	cvStartWriteStruct(fs, "loc", CV_NODE_SEQ);  //��ʼд�ṹ������������sequence�������ֵ�����ڵ㼯��  
	cvWriteRawData(fs, area_info, 4, "i");
	cvEndWriteStruct(fs);

	cvEndWriteStruct(fs);

	return 0;
}

/*
 *	���ܣ���xml�ļ�
 *  ������
 *  ״̬��
 */
static Rect xml_read(CvFileStorage *fs,char*meaning)
{
	CvFileNode * node;
	
	//��õ�һ�����ݽڵ�
	node = cvGetFileNodeByName(fs, 0, meaning);
	//char * str= const_cast<char*>(cvReadStringByName(fs, node, "loc"));
	printf("\nName=%s", meaning);
		
	//���нṹ�Ķ�ȡ������ <sales_record> 30000 4200 50090 6100</sales_record>
	int* d = (int*)cvAlloc(sizeof(int)*4);
	if (d)
	{
		node = cvGetFileNodeByName(fs, node, "loc");
		if (node)
		{
			cvReadRawData(fs, node, d, "i");
			printf("\nloc=");
			for (int i = 0; i < 4; i++)
				printf("%d ", d[i]);
		}
		//cvFree(&d);
	}
	Rect locinfo=Rect(d[0], d[1], d[2], d[3]);
	cvFree(&d);
	return locinfo;
}

/*
 *	���ܣ��ص���������ʾ���ο�
 *  ������
 *  ״̬��
 */
static void onMouse( int event, int x, int y, int, void* )
{
	switch(event)
	{
		case CV_EVENT_LBUTTONDOWN: 
			//the left up point of the rect
			is_drawing=true;
			drawing_box.x=x;
			drawing_box.y=y;
			break;
		case CV_EVENT_MOUSEMOVE:
			//adjust the rect (use color blue for moving)
			if(is_drawing)
			{
				drawing_box.width=x-drawing_box.x;
				drawing_box.height=y-drawing_box.y;
				img_original.copyTo(img_drawing);
				for(vector<Rect>::iterator it=biaozhu_boxs.begin();	it!=biaozhu_boxs.end();++it)
				{
					rectangle(img_drawing,(*it),Scalar(0,255,0));
				}
				rectangle(img_drawing,drawing_box,Scalar(255,0,0));
			}
			break;
		case CV_EVENT_LBUTTONUP:
			//finish drawing the rect (use color green for finish)
			if(is_drawing)
			{
				drawing_box.width=x-drawing_box.x;
				drawing_box.height=y-drawing_box.y;
				img_original.copyTo(img_drawing);
				for(vector<Rect>::iterator it=biaozhu_boxs.begin();	it!=biaozhu_boxs.end();++it)
				{
					rectangle(img_drawing,(*it),Scalar(0,255,0));
				}
				rectangle(img_drawing,drawing_box,Scalar(0,255,0));
				cout << "Please input the meaning" << endl;
				//gets(meaning);
				cin >>meanloc.meaning;
				meanloc.loc = Rect(x, y, drawing_box.width, drawing_box.height);
				config.push_back(meanloc);
				biaozhu_boxs.push_back(drawing_box);
			}
			is_drawing=false;
			break;
	}
	imshow("Video",img_drawing);
	return;
}

/*
 *	���ܣ���Ƶ��ע���������
 *  ˵����ÿ��עһ�����������Ӧ�Ĵ���λ����Ϣ����Enter��ȷ��
 *  ״̬��
 */
int videolabel(char*videoname,char*result)
{
	help();
	ofstream outfile(result);

	//ԭʼ��Ƶ��ʾ
	namedWindow("Video");
	VideoCapture capture(videoname);
	capture>>img_original; 
	imshow("Video",img_original);
	
	setMouseCallback( "Video", onMouse, 0 ); //���ûص�����

	//�Ա���ע�ĵ�һ֡�Ĵ���,�������о��Σ���һ���������⣩
	img_original.copyTo(img_drawing);
	for(vector<Rect>::iterator it=biaozhu_boxs.begin();it!=biaozhu_boxs.end();++it)
	{
		 cout<<"��ʼ��ʱ�����о���Ҫ����"<<endl;
	     rectangle(img_drawing,(*it),Scalar(0,255,0));
		 outfile<<"[1]"<<(*it).x<<" "<<(*it).y<<" "<<(*it).width<<" "<<(*it).height<<endl;
	}
	
	int frame_counter=1;
	while(1)
	{
		capture >> img_original;  //read the next frame
		int c=waitKey(10);
		if( (c & 255) == 27 ){   //ESC���˳�
			cout << "Exiting ...\n";
			break;
		}
		switch((char)c)
		{
		     case ' ':
				++frame_counter;
				capture>>img_original;  //read the next frame
				if(img_original.empty()){
					cout<<"\nVideo Finished!"<<endl;
					return 0;
				}
				waitKey();
				img_original.copyTo(img_drawing);
				//save and draw all of the labeling rects
				for(vector<Rect>::iterator it=biaozhu_boxs.begin();	it!=biaozhu_boxs.end();++it)
				{
					rectangle(img_drawing,(*it),Scalar(0,255,0));
					outfile<<"["<<frame_counter<<"]"<<(*it).x<<" "<<(*it).y<<" "<<(*it).width<<" "<<(*it).height<<endl;
				}
				break;
			case 'z':
				//undo the latest labeling
				if(!biaozhu_boxs.empty())
				{
					vector<Rect>::iterator it_end=biaozhu_boxs.end();
					--it_end;
					biaozhu_boxs.erase(it_end);
				}
				img_original.copyTo(img_drawing);//���ϴζ�֡Ū����������֡�ϲ�����ʾ
				for(vector<Rect>::iterator it=biaozhu_boxs.begin();	it!=biaozhu_boxs.end();++it)
				{
					rectangle(img_drawing,(*it),Scalar(0,255,0));
				}
				break;

			case 'c':
				//clear all the rects on the image
				biaozhu_boxs.clear();
				img_original.copyTo(img_drawing);
			//default:
				//waitKey();
		}

		for (vector<Rect>::iterator it = biaozhu_boxs.begin(); it != biaozhu_boxs.end(); ++it)
		{
			rectangle(img_original, (*it), Scalar(0, 255, 0));
		}
	   img_original.copyTo(img_drawing);
	   imshow("Video",img_drawing);
	}

	return 0;
}



//���ܲ�����
//int main(int argc,char**argv)
//{
//	char*videoname = "samples/video/test_o2.mp4";
//	char *result = "config.txt";
//	char*configname = "config.xml";
//	
//	//(1)��Ƶ��ע
//	videolabel(videoname, result);
//	
//	//(2)д�����ļ�  
//	fs = cvOpenFileStorage(configname, 0, CV_STORAGE_WRITE, "UTF-8");
//	cvWriteComment(fs, "config info xml file", 1);
//	for (vector<s_unitInfo>::iterator it = config.begin(); it != config.end();++it)
//	{
//		xml_write(it->meaning, it->loc);
//	}
//	cvReleaseFileStorage(&fs);
//    
//	//��3���������ļ�
//	fs = cvOpenFileStorage(configname, 0, CV_STORAGE_READ, "UTF-8");
//	//xml_read(fs, "ww");
//	char* means[20] = { "ww", "erz", "vv" }; //�����ļ������������ȶ���
//	for (int i = 0; i < 20;++i)
//    {
//		if (means[i])
//		{
//			Rect loc=xml_read(fs, means[i]);
//			strcpy(meanloc.meaning, means[i]);
//			meanloc.loc = loc;
//			config.push_back(meanloc);//������е���Ϣ����������config���vector��
//		}
//		else
//			break;
//    }
//	cvReleaseFileStorage(&fs);
//
//	//(4)���Զ�����Ĳ���
//	//����ָ�������ʶ������ɾ����Ӧ��λ����Ϣ��
//
//
//	getchar();
//	return 0;
//}