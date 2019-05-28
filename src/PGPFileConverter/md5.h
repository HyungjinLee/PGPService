#pragma once
/*
  PROTOTYPES should be set to one if and only if the compiler supports
  function argument prototyping.
  The following makes PROTOTYPES default to 0 if it has not already
  been defined with C compiler flags.
 */
#ifndef PROTOTYPES
#define PROTOTYPES 0
#endif

 /* POINTER defines a generic pointer type */
typedef unsigned char *POINTER;

/* UINT2 defines a two byte word */
typedef unsigned short int UINT2;

/* UINT4 defines a four byte word */
typedef unsigned long int UINT4;

/* PROTO_LIST is defined depending on how PROTOTYPES is defined above.
If using PROTOTYPES, then PROTO_LIST returns the list, otherwise it
  returns an empty list.
 */
#if PROTOTYPES
#define PROTO_LIST(list) list
#else
#define PROTO_LIST(list) ()
#endif

 /* MD5 context. */
typedef struct {
	UINT4 state[4];                                   /* state (ABCD) */
	UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;

class MD5 {

public:
	MD5();
	~MD5();
	void MD5Init ();
	void MD5Update (unsigned char *, unsigned int);
	void MD5Final (unsigned char[16], MD5_CTX *);
	void MDPrint();
	
	void MDFilter();
	void MDTimeTrial();
	unsigned char* MDString(char*);
	void MDTestSuite();
	unsigned char* GetDigest() { return digest; }

private:
	MD5_CTX* context;
	unsigned char digest[16];

};