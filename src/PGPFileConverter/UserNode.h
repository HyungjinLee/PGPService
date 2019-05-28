#pragma once

#include <string>
#include <vector>

#include "Des.h"
#include "rsa.h"
#include "md5.h"
using namespace std;

#define USERNUM 3
// cipherHash : 송신자에 의해 서명된 해쉬 값 H(M)과 수신자의 private key에 의해 암호화된 값
// binary_cipher_msg : 세션 키에 의해 암호화된 메시지 값
// cipherSession : 수신자의 공개키로 암호화된 세션 키 값
struct MsgType {

	string cipherHash;
	string binary_cipher_msg;
	int cipherSession[10]; // 세션 키
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

	Rsa rsa;	// Encryption, Decription에 사용되는 객체
};

