#pragma once
#define MAXSIZE 200
#include <string>
using namespace std;

class Rsa
{
public:
	Rsa();
	~Rsa();
	int prime(long int);
	void ce();
	long int cd(long int);
	long int* encrypt(long int key);
	long int* decrypt(long int key);
	long int DistributePubkey(int keynum) { return e[keynum]; }	// keynum번째 public key 분배
	long int DistributePrikey(int keynum) { return d[keynum]; }
	void KeyGen();
	// keynum번째 private key 분배
	
	void SetMsg(string _msg)
	{
		strcpy_s(msg, _msg.c_str());
	}

	void SetEncrypt(int* _en, int size)
	{
		for (i = 0; i < size; i++)
			en[i] = _en[i];
	}

	void Init();

	void run();

private :
	long int p;
	long int q;
	long int n;
	long int t;
	long int flag;
	long int e[MAXSIZE];
	long int d[MAXSIZE];
	long int temp[MAXSIZE];
	long int j;
	long int m[MAXSIZE];
	long int en[MAXSIZE];
	long i;
	char msg[MAXSIZE];


};

