#include "UserNode.h"
#include <iostream>
#include <bitset>
using namespace std;


UserNode::UserNode(int _name)
{
	m_private_key = -1;
	name = _name;
}

UserNode::~UserNode()
{
}

// 다른 유저에게 정보 송신
MsgType UserNode::Transmit(UserNode& Dest, string msg, char* digest, Rsa& rsa)
{


	string sessionkey = "";	// session key 생성

	int key[10];

	key[0] = rand() % 2;
	key[1] = rand() % 2;
	key[2] = rand() % 2;
	key[3] = rand() % 2;
	key[4] = rand() % 2;
	key[5] = rand() % 2;
	key[6] = rand() % 2;
	key[7] = rand() % 2;
	key[8] = rand() % 2;
	key[9] = rand() % 2;

	rsa.Init();
	rsa.SetMsg(digest); // 해쉬값 입력

	// 암호화하기 전 해쉬값을 저장
	string pure_hash = digest;

	cout << "해쉬 암호화 후 " << endl;

	int* cipherHash = (int*) rsa.encrypt(this->m_private_key);	// 해당 사용자의 private 키로 해쉬값 암호화
	
	cout << "암호화된 해쉬값 출력";
	for (int i = 0; cipherHash[i] != -1; i++)
 		printf("%2x", cipherHash[i]);
	cout << endl;

	string cipherHash_str;

	for (int i = 0; cipherHash[i] != -1; i++)
		cipherHash_str += std::to_string(cipherHash[i]); //  암호화된 해쉬값을 string 형식으로 합침
	cout << endl;

	int pt[8] = { 0, };

	cout << msg;

	Des des;
	string binary_cipher_msg = "";
	des.gen_keys(key); // Generating Keys key1 & key2

	// msg string 을 binary 형식으로 8bit씩 나누고 이를 각각 des로 암호화

	for (std::size_t i = 0; i < msg.size(); ++i)
	{
		bitset<8> binarystring(msg.c_str()[i]);
		string binary_outputinfo = binarystring.to_string();

		for (int j = 0; j < 8; j++)
			pt[j] = (int)binary_outputinfo.at(j) - 48; // binary 값 하나 씩 plaintext에 넣어줌

		des.En_De(pt, 0);	// Encryption
		
		//printf("\nCipher Text :");
		binary_cipher_msg += des.GetEncryptedTxt();	// 전송을 위해 Ciphertext를 더함
	}

	for (int i : key)
		sessionkey += std::to_string(i);

	rsa.Init();
	rsa.SetMsg(sessionkey);
	int* cipher_session_key = (int* ) rsa.encrypt(m_public_key[Dest.GetName()]);	// 목적지의 public key로 세션키를 암호화

	cout << "암호화된 session key 값 출력";
	for (int i = 0; cipher_session_key[i] != -1; i++)
		cout << cipher_session_key[i];
	cout << endl;

	static MsgType final_msg; // 보내어질 최종 메시지

	final_msg.cipherHash = pure_hash;
	final_msg.binary_cipher_msg = binary_cipher_msg;
	for (int i = 0; cipher_session_key[i] != -1; i++)
		final_msg.cipherSession[i] = cipher_session_key[i];

	return final_msg;	// 최종 메시지 리턴
}

// 다른 유저로부터 정보 수신
void UserNode::Receive(UserNode& Src, MsgType msg, Rsa& rsa)
{
	string cipherHash = msg.cipherHash;
	string binary_cipher_msg = msg.binary_cipher_msg;
	int* cipher_session = msg.cipherSession;

	string c;

	// 자신의 private key를 이용하여 session key를 먼저 복호화함
	rsa.SetEncrypt(cipher_session, 10);
	
	int* sessionkey = (int*) rsa.decrypt(this->m_private_key);	// 복호화된 sessionkey
	
	for (int i = 0; i < 10; i++)
		sessionkey[i] -= 48;

	system("Pause");

	// 얻은 세션 키로 ciphered massage 복호화 - DES
	

	Des des;
	des.gen_keys(sessionkey); // Generating Keys key1 & key2

	// msg string 을 binary 형식으로 8bit씩 나누고 이를 각각 des로 암호화
	int startidx = 0;
	int endidx = 8;
	int pt[8] = { 0, };
	string result;

	for (std::size_t i = 0; i < binary_cipher_msg.size()/8; ++i)
	{
		string chunktxt = binary_cipher_msg.substr(startidx, endidx);	// 복호화 할 cipher text	
		for (int j = 0; j < 8; j++)
			pt[j] = (int)chunktxt.at(j) - 48; // binary 값 하나 씩 plaintext에 넣어줌

		
		des.En_De(pt, 1);	// Decryption
		printf("\nPlain Text (After Decrypting):");

		printf("%c\n", des.GetDecryptedTxt());
		result += des.GetDecryptedTxt();

		startidx += 8;
		endidx += 8;
	}

	cout << "\nDecrypted Message : " <<  result;
	system("Pause");

	// Authentication
	// 해쉬값이 누구에 의해서 서명되었는지 검사

	rsa.SetMsg(cipherHash);

	int* authentication_part = (int*) rsa.encrypt(Src.GetPrivateKey()); // Authentication Part
	int partlength=0;

	for (int i = 0; authentication_part[i] != -1; i++)
		partlength++;

	rsa.SetEncrypt(authentication_part, partlength);

	// 수신자의 public key를 이용하여 Authentication Part를 복호화
	int* content = (int*) rsa.decrypt(m_public_key[Src.GetName()]);
	
	cout << endl << endl << "Hash1 : " << endl;
	for (int i = 0; content[i] != -1; i++)
	{
		printf("%2x", content[i]);
	}

	system("Pause");

	/*
	// 비교를 위해 같은 char* 타입으로 바꿈
	for (int i : content)
		sessionkey += std::to_string(i);

		c1[i] = (char)content[i];
		*/

	MD5 md5;
	cout << endl;
	unsigned char* c2 = md5.MDString((char*)result.c_str()); // 복호화한 메시지로 해쉬 값을 구함
	cout << "Hash2 : " << endl;
	for(int i=0;i<16;i++)
		printf("%2x", c2[i]);
	
	//strcmp(c1, c2);

	system("Pause");
	cout << endl;

}
