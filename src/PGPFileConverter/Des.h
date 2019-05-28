#pragma once
#include <string>
using namespace std;

class Des
{
public:
	Des();
	~Des();
	void sbox(int[], int[], int, int);
	void cmp_fun(int);
	void left_shift(int[], int);
	void gen_keys(int* _key);
	void En_De(int[], int);
	void print();
	string GetEncryptedTxt();
	char GetDecryptedTxt();
	int* GetCipher();

private:
	int l[4];
	int r[4];
	int keys[2][8];
	int ct[8];

};

