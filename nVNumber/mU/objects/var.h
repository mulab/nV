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

#include <boost/cstdint.hpp>

namespace maTHmU {
//////////////////////////////////////
typedef boost::int32_t int32_t;
typedef boost::uint32_t uint32_t;
typedef boost::uint16_t uint16_t;

//////////////////////////////////////
class var;
typedef const var& VAR;
//////////////////////////////////////
class VAR_API var {
	friend VAR_API uint32_t GetType(VAR);
	friend VAR_API void SetType(var&,uint32_t);
public:
	static void start();
	var();
	var(VAR);
	var(void*);
	var(void*, uint32_t);
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
	uint32_t id;
};
typedef var (*N_OP) ();
typedef var (*UN_OP)(VAR);
typedef var (*BIN_OP)(VAR,VAR);
typedef var (*TER_OP)(VAR,VAR,VAR);
typedef var (*SC_OP)(VAR,int32_t);
typedef int32_t (*CMP_OP)(VAR,VAR);
typedef bool (*LESS_OP)(VAR,VAR);
typedef void (*DES_OP)(void*);
//////////////////////////////////////
VAR_API DES_OP& TypeTable(uint32_t);
VAR_API uint32_t AddType(DES_OP);
VAR_API void RemoveType(uint32_t);
//////////////////////////////////////
VAR_API var& NameTable(const char*);
VAR_API bool FindName(const char*);
VAR_API void RemoveName(const char*);
//////////////////////////////////////
}
