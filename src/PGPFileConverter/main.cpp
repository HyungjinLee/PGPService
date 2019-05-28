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
	// ���ÿ��� �򼭹��� ����
	Application app;
	app.ReadFile("a.txt");
	string plaintxt = app.GetPlainTxt();

	// �׽�Ʈ�� ���� ���� 3�� ����
	UserNode Alice(0), Bob(1), Charlie(2);

	// Public, Private Ű �й�

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

	// Ű �й� �Ϸ�

	// Plain Text�κ��� �ؽ� �� ����

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