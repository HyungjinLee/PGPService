
#define _CRT_SECURE_NO_WARNINGS

#include "Application.h"
#include "md5.h"
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

Application::Application()
{

}

void Application::run()
{


}

Application::~Application()
{
}

void Application::ReadFile(string filename)
{
	ifstream file(filename);
	int len,count, total;
	char buffer[1024];
	string tmp;
	
		count = 0;
		total = 0;

		if (!file.is_open())
		{
			printf("%s can't be opened\n", filename);
			exit(1);
		}

		else
		{
			while (!file.eof())    // ���� �����Ͱ� ������ ���� �ƴ� �� ��� �ݺ�
			{
				file >> tmp;
				plaintext += tmp;
				plaintext += " ";
			}

		}
		cout << plaintext << endl;

		file.close();
}

