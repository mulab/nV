#pragma once

#ifdef _WIN32
#ifdef VAR_EXPORTS
#define VAR_API __declspec(dllexport)
#else
#define VAR_API
// #define VAR_API __declspec(dllimport)
// #pragma comment(lib,"var")
#endif
#else
#define VAR_API
#endif

namespace maTHmU {
//////////////////////////////////////
typedef int INT;
typedef unsigned int UINT;
typedef unsigned short USHORT;

//////////////////////////////////////
class var;
typedef const var& VAR;
//////////////////////////////////////
class VAR_API var {
	friend VAR_API UINT GetType(VAR);
	friend VAR_API void SetType(var&,UINT);
public:
	static void start();
	var();
	var(VAR);
	var(void*);
	var(void*, UINT);
	var& operator = (VAR);
	~var();
	void* ptr;
private:
	void clone(VAR);
	void set(VAR);
	void incr() const;
	void decr();
	void init();
	void clear();
	UINT id;
};
typedef var (*N_OP) ();
typedef var (*UN_OP)(VAR);
typedef var (*BIN_OP)(VAR,VAR);
typedef var (*TER_OP)(VAR,VAR,VAR);
typedef var (*SC_OP)(VAR,INT);
typedef INT (*CMP_OP)(VAR,VAR);
typedef bool (*LESS_OP)(VAR,VAR);
typedef void (*DES_OP)(void*);
//////////////////////////////////////
VAR_API DES_OP& TypeTable(UINT);
VAR_API UINT AddType(DES_OP);
VAR_API void RemoveType(UINT);
//////////////////////////////////////
VAR_API var& NameTable(const char*);
VAR_API bool FindName(const char*);
VAR_API void RemoveName(const char*);
//////////////////////////////////////
}
