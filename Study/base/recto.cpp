#include<iostream>  
#include<opencv2/opencv.hpp>  

using namespace cv;
using namespace std;

int recto()
{
	Rect rect(0, 0, 50, 50);
	Point point(20, 20);
	Rect rect1(2, 2, 10, 10);
	Rect rect2(20, 20, 100, 100);
	Size size(5, 5);

	cout << "��ʼ��" << endl;
	cout << "�������Ͻǵĺ����꣺" << rect.x << endl;
	cout << "�������Ͻǵ������꣺" << rect.y << endl;
	cout << "���εĿ�ȣ�" << rect.width << endl;
	cout << "���εĸ߶ȣ�" << rect.height << endl;
	cout << "���εĳߴ磺" << rect.size() << endl;
	cout << "���ε������" << rect.area() << endl;
	cout << "�ж�(20, 20)������ڲ��ھ����ڣ�" << rect.contains(point) << endl;
	/*cout << "�жϾ���dstrect��������ڲ��ھ����ڣ�" << rect.inside(rect1) << endl;*/
	cout << "�������Ͻǵĵ����꣺" << rect.tl() << endl;
	cout << "�������½ǵĵ����꣺" << rect.br() << endl;

	//����Ľ���  
	cout << "����Ľ�����" << (rect1 & rect) << endl;
	//����Ĳ���  
	cout << "����Ĳ�����" << (rect | rect2) << endl;

	//�������ƽ�Ʋ���  
	cout << "�������ƽ�Ʋ�����" << (rect + point) << endl;
	cout << "�������ƽ�Ʋ�����" << (rect - point) << endl;
	//����������Ų���  
	cout << "����������Ų�����" << (rect + size) << endl;

	system("pause");

	return 0;
}

int main()
{
	recto();
}