/************************************************************************
* Copyright(c) 2015 tuling56
*
* File:	xml_whole.cpp
* Brief: OpenCV xml��д
* Source:http://blog.csdn.net/augusdi/article/details/8865515
* Status: ��ɵ���
* Date:	[1/14/2015 jmy]
************************************************************************/

#include <cxcore.h>
#include <highgui.h>
//#include <opencv2/core/core.hpp>


static int xml_write(void)
{
	// �����ļ��洢����
	CvFileStorage *fs=cvOpenFileStorage("test.xml",0,CV_STORAGE_WRITE);
	// дע��
	cvWriteComment(fs,"����дXML�ļ�",1);
	
	// ��ʼд�ṹ��������ͼmap��Ҳ���������ֵ�����ڵ㼯��
	cvStartWriteStruct(fs,"Employee",CV_NODE_MAP);
	cvWriteComment(fs,"MAP���ͣ����������䣬нˮ",1);// ע��
    cvWriteString(fs, "name", "��Խ", 0);	// ����
	cvWriteInt(fs,"age",18);// ����
	cvWriteReal(fs,"salary",2780.3);// нˮ
	cvWriteInt(fs,"sales_count",4);// ���۴���
	{
		
		int sales_record[]={30000,4200,50090};// ���۾�������
		cvWriteComment(fs,"SEQ���ͣ����ۼ�¼",1);// ע��
		// ��ʼд�ṹ������������sequence�������ֵ�����ڵ㼯��
		cvStartWriteStruct(fs,"sales_record",CV_NODE_SEQ);
		cvWriteRawData(fs,sales_record,3,"i");// ǰ3�����ۼ�¼
		cvWriteInt(fs,0,6100);// ��4�����ۼ�¼��ע��������
		cvEndWriteStruct(fs);// ����
	}
	{
		
		cvWriteComment(fs,"MAP���ͣ�����",1);// ע��
		cvStartWriteStruct(fs,"Parent",CV_NODE_MAP);// ��ʼ
		cvWriteString(fs,"father","��˴");// ����
		cvWriteString(fs,"mother","����");// ĸ��
		cvEndWriteStruct(fs);// ����
	}
	cvEndWriteStruct(fs);	// ����
	cvReleaseFileStorage(&fs);// �ͷ��ļ��洢����
	return 0;
}


static int xml_read(void)
{
	
	CvFileNode * node, *node2;// �ļ��ڵ�
	char * str;
	int count;

	CvFileStorage *fs = cvOpenFileStorage("test.xml",0,CV_STORAGE_READ);// ��XML�ļ�
	// ��õ�һ�����ݽڵ�
	node = cvGetFileNodeByName(fs,0,"Employee");
	str = const_cast<char*>(cvReadStringByName(fs, node, "name"));
	printf("\n����=%s",str);
	printf("\n����=%d",cvReadIntByName(fs,node,"age"));
	printf("\nнˮ=%g",cvReadRealByName(fs,node,"salary"));
	count = cvReadIntByName(fs,node,"sales_count");
	printf("\n����%d��",count);
	int* d = (int*)cvAlloc(sizeof(int)*count);
	if(d)
	{
		int i;
		node2 = cvGetFileNodeByName(fs,node,"sales_record");
		if(node2)
		{
			cvReadRawData(fs,node2,d,"i");
			printf("\n���ۼ�¼=");
			for(i=0;i<count;i++)
				printf("%d, ",d[i]);
		}
		cvFree(&d);
	}
	// ��õڶ���ڵ�
	node = cvGetFileNodeByName(fs,node,"Parent");
	printf("\n���ڵ�=%s",cvGetFileNodeName(node));
	str = const_cast<char*>(cvReadStringByName(fs, node, "father"));
	printf("\n����=%s",str);
	str = const_cast<char*>(cvReadStringByName(fs, node, "mother"));
	printf("\nĸ��=%s",str);
	return 0;
}


//���ܲ�����
//int main()
//{
//	xml_write();
//	xml_read();
//	getchar();
//	return 0;
//}