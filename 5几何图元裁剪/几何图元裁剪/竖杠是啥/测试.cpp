//#include<iostream>
//using namespace std;
//
//void tentotwo(int a)
//{
//	if (a)
//	{
//		tentotwo(a / 2);
//		cout << a % 2;
//	}
//}
//
//int main()
//{
//	unsigned int a = 0;
//	a = a | 7;
//	tentotwo(a);
//	return 0;
//}
#include<iostream>
using namespace std;
int a1 = 1, a2 = 2;
void output(int&a1)
{
	cout << a1 << endl;
}
int main()
{
	output(a1);
	output(a2);
	return 0;
}