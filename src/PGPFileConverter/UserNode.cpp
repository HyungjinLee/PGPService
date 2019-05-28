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

// �ٸ� �������� ���� �۽�
MsgType UserNode::Transmit(UserNode& Dest, string msg, char* digest, Rsa& rsa)
{


	string sessionkey = "";	// session key ����

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
	rsa.SetMsg(digest); // �ؽ��� �Է�

	// ��ȣȭ�ϱ� �� �ؽ����� ����
	string pure_hash = digest;

	cout << "�ؽ� ��ȣȭ �� " << endl;

	int* cipherHash = (int*) rsa.encrypt(this->m_private_key);	// �ش� ������� private Ű�� �ؽ��� ��ȣȭ
	
	cout << "��ȣȭ�� �ؽ��� ���";
	for (int i = 0; cipherHash[i] != -1; i++)
 		printf("%2x", cipherHash[i]);
	cout << endl;

	string cipherHash_str;

	for (int i = 0; cipherHash[i] != -1; i++)
		cipherHash_str += std::to_string(cipherHash[i]); //  ��ȣȭ�� �ؽ����� string �������� ��ħ
	cout << endl;

	int pt[8] = { 0, };

	cout << msg;

	Des des;
	string binary_cipher_msg = "";
	des.gen_keys(key); // Generating Keys key1 & key2

	// msg string �� binary �������� 8bit�� ������ �̸� ���� des�� ��ȣȭ

	for (std::size_t i = 0; i < msg.size(); ++i)
	{
		bitset<8> binarystring(msg.c_str()[i]);
		string binary_outputinfo = binarystring.to_string();

		for (int j = 0; j < 8; j++)
			pt[j] = (int)binary_outputinfo.at(j) - 48; // binary �� �ϳ� �� plaintext�� �־���

		des.En_De(pt, 0);	// Encryption
		
		//printf("\nCipher Text :");
		binary_cipher_msg += des.GetEncryptedTxt();	// ������ ���� Ciphertext�� ����
	}

	for (int i : key)
		sessionkey += std::to_string(i);

	rsa.Init();
	rsa.SetMsg(sessionkey);
	int* cipher_session_key = (int* ) rsa.encrypt(m_public_key[Dest.GetName()]);	// �������� public key�� ����Ű�� ��ȣȭ

	cout << "��ȣȭ�� session key �� ���";
	for (int i = 0; cipher_session_key[i] != -1; i++)
		cout << cipher_session_key[i];
	cout << endl;

	static MsgType final_msg; // �������� ���� �޽���

	final_msg.cipherHash = pure_hash;
	final_msg.binary_cipher_msg = binary_cipher_msg;
	for (int i = 0; cipher_session_key[i] != -1; i++)
		final_msg.cipherSession[i] = cipher_session_key[i];

	return final_msg;	// ���� �޽��� ����
}

// �ٸ� �����κ��� ���� ����
void UserNode::Receive(UserNode& Src, MsgType msg, Rsa& rsa)
{
	string cipherHash = msg.cipherHash;
	string binary_cipher_msg = msg.binary_cipher_msg;
	int* cipher_session = msg.cipherSession;

	string c;

	// �ڽ��� private key�� �̿��Ͽ� session key�� ���� ��ȣȭ��
	rsa.SetEncrypt(cipher_session, 10);
	
	int* sessionkey = (int*) rsa.decrypt(this->m_private_key);	// ��ȣȭ�� sessionkey
	
	for (int i = 0; i < 10; i++)
		sessionkey[i] -= 48;

	system("Pause");

	// ���� ���� Ű�� ciphered massage ��ȣȭ - DES
	

	Des des;
	des.gen_keys(sessionkey); // Generating Keys key1 & key2

	// msg string �� binary �������� 8bit�� ������ �̸� ���� des�� ��ȣȭ
	int startidx = 0;
	int endidx = 8;
	int pt[8] = { 0, };
	string result;

	for (std::size_t i = 0; i < binary_cipher_msg.size()/8; ++i)
	{
		string chunktxt = binary_cipher_msg.substr(startidx, endidx);	// ��ȣȭ �� cipher text	
		for (int j = 0; j < 8; j++)
			pt[j] = (int)chunktxt.at(j) - 48; // binary �� �ϳ� �� plaintext�� �־���

		
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
	// �ؽ����� ������ ���ؼ� ����Ǿ����� �˻�

	rsa.SetMsg(cipherHash);

	int* authentication_part = (int*) rsa.encrypt(Src.GetPrivateKey()); // Authentication Part
	int partlength=0;

	for (int i = 0; authentication_part[i] != -1; i++)
		partlength++;

	rsa.SetEncrypt(authentication_part, partlength);

	// �������� public key�� �̿��Ͽ� Authentication Part�� ��ȣȭ
	int* content = (int*) rsa.decrypt(m_public_key[Src.GetName()]);
	
	cout << endl << endl << "Hash1 : " << endl;
	for (int i = 0; content[i] != -1; i++)
	{
		printf("%2x", content[i]);
	}

	system("Pause");

	/*
	// �񱳸� ���� ���� char* Ÿ������ �ٲ�
	for (int i : content)
		sessionkey += std::to_string(i);

		c1[i] = (char)content[i];
		*/

	MD5 md5;
	cout << endl;
	unsigned char* c2 = md5.MDString((char*)result.c_str()); // ��ȣȭ�� �޽����� �ؽ� ���� ����
	cout << "Hash2 : " << endl;
	for(int i=0;i<16;i++)
		printf("%2x", c2[i]);
	
	//strcmp(c1, c2);

	system("Pause");
	cout << endl;

}
