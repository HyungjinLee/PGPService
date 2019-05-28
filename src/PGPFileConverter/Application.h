#pragma once
#include <string>
using namespace std;

class Application
{
public:
	Application();
	void ReadFile(string filename);
	~Application();

	void run();

	string GetPlainTxt() { return plaintext;  }

private:
	string plaintext;

};

