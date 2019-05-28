#pragma once

#include <string>
#include <vector>

#include "Des.h"
#include "rsa.h"
#include "md5.h"
using namespace std;

#define USERNUM 3
// cipherHash : �۽��ڿ� ���� ����� �ؽ� �� H(M)�� �������� private key�� ���� ��ȣȭ�� ��
// binary_cipher_msg : ���� Ű�� ���� ��ȣȭ�� �޽��� ��
// cipherSession : �������� ����Ű�� ��ȣȭ�� ���� Ű ��
struct MsgType {

	string cipherHash;
	string binary_cipher_msg;
	int cipherSession[10]; // ���� Ű
};

class UserNode
{
public:
	UserNode(int _name);
	~UserNode();

	int GetName() { return name; }
	void AddMsg(string* _msg) {
		msg.push_back(_msg);
	}

	MsgType Transmit(UserNode& Dest, string msg,  char* digest, Rsa& rsa);
	void Receive(UserNode & Src, MsgType msg, Rsa& rsa);
	void Authentication(UserNode& Source);
	void GrantPublicKey(long int _public_key[])
	{
		for (int i = 0; i < USERNUM; i++)
			m_public_key[i] = _public_key[i];
	}

	void GrantPrivateKey(long int _private_key)
	{
		m_private_key = _private_key;
	}

	long int GetPublicKey(int keynum) { return m_public_key[keynum];  }
	long int GetPrivateKey() { return m_private_key;  }

	string* GetMsg() { return msg.at(0); }

private:
	long int m_public_key[USERNUM];
	long int m_private_key;
	int name;
	vector<string*> msg;

	Rsa rsa;	// Encryption, Decription�� ���Ǵ� ��ü
};

