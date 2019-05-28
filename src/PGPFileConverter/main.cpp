#include "rsa.h"
#include "md5.h"
#include "Des.h"
#include "Application.h"
#include "UserNode.h"
#include <iostream>
#include <bitset>
using namespace std;

char strToChar(const char* str) {
	char parsed = 0;
	for (int i = 0; i < 8; i++) {
		if (str[i] == '1') {
			parsed |= 1 << (7 - i);
		}
	}
	return parsed;
}

void main()
{
	// 로컬에서 평서문을 읽음
	Application app;
	app.ReadFile("a.txt");
	string plaintxt = app.GetPlainTxt();

	// 테스트를 위해 유저 3명 설정
	UserNode Alice(0), Bob(1), Charlie(2);

	// Public, Private 키 분배

	Rsa rsa;
	rsa.KeyGen();
	long int public_key[USERNUM];
	
	for(int i=0;i<USERNUM;i++)
		public_key[i] = rsa.DistributePubkey(i);
	
	// public key 0 - Alice
	// public key 1 - Bob
	// public key 2 - Charlie

	Alice.GrantPublicKey(public_key);
	Bob.GrantPublicKey(public_key);
	Charlie.GrantPublicKey(public_key);

	Alice.GrantPrivateKey(rsa.DistributePrikey(0));
	Bob.GrantPrivateKey(rsa.DistributePrikey(1));
	Charlie.GrantPrivateKey(rsa.DistributePrikey(2));

	// 키 분배 완료

	// Plain Text로부터 해쉬 값 생성

	MD5 md5;
	cout << endl;
	unsigned char* digest = md5.MDString((char*)plaintxt.c_str());

	cout << "\n";
	system("pause");
	
	MsgType final_msg = Alice.Transmit(Charlie, plaintxt, (char*)digest, rsa); // Alice -> Charlie
	cout << "\n";
	system("pause");

	Charlie.Receive(Alice, final_msg, rsa);
	// Bob.Receive(Alice, final_msg, rsa);
	// Alice.Receive(Bob, final_msg, rsa);

}