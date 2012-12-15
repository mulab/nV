#include "common.h"
#include "objects/var.h"
#include <vector>
#include <map>
#include <iostream>
#include <cstring>

namespace maTHmU {
//////////////////////////////////////
namespace {
std::vector<DES_OP> TypeFactory(0x100);
UINT TypeHead = 1;
std::vector<USHORT> RefFactory(0x10000);
UINT RefHead = 1;
struct LESS { bool operator()(const char *a,const char *b) const { return std::strcmp(a,b) < 0; } };
std::map<const char*,var,LESS> NameFactory;
}
#define TYPE(x) ((x) & 0xFF000000)
#define REF(x) ((x) & 0x00FFFFFF)
//////////////////////////////////////
void var::incr() const { ++RefFactory[REF(id)]; }
void var::decr()
{
	UINT ref = REF(id);
	if(ref != 0 && --RefFactory[ref] == 0)
	{
		clear();
		if(ref < RefHead) RefHead = ref;
	}
}
void var::init() { ptr = 0; id = 0; }
void var::clone(VAR x) { ptr = x.ptr; id = x.id; }
void var::set(VAR x) { x.incr(); decr(); clone(x); }
void var::clear() { TypeFactory[id >> 24](ptr); ptr = 0; }
var::var() { init(); }
var::var(VAR x) { init(); set(x); }
var::var(void *p) : ptr(p), id(0) {}
var::var(void *p, UINT t) : ptr(p), id(t | RefHead)
{
	RefFactory[RefHead] = 1;
	while(++RefHead < RefFactory.size() && RefFactory[RefHead] != 0);
	if(RefHead == RefFactory.size()) RefFactory.resize(2 * RefFactory.size());
}
var& var::operator = (VAR x) { set(x); return *this; }
var::~var() { decr(); }
void SetType(var& x, UINT t) { x.id = t | REF(x.id); }
UINT GetType(VAR x) { return TYPE(x.id); }
//////////////////////////////////////
DES_OP& TypeTable(UINT i) { return TypeFactory[i >> 24]; }
UINT AddType(DES_OP p)
{
	TypeFactory[TypeHead] = p; UINT t = TypeHead << 24;
	while(++TypeHead < TypeFactory.size() && TypeFactory[TypeHead] != 0);
	if(TypeHead == TypeFactory.size()) TypeFactory.resize(2 * TypeFactory.size());
	return t;
}
void RemoveType(UINT t) { t = t >> 24; if(t < TypeHead) TypeHead = t; }
//////////////////////////////////////
var& NameTable(const char *s) { return NameFactory[s]; }
bool FindName(const char *s) { return NameFactory.count(s) != 0; }
void RemoveName(const char *s) { NameFactory.erase(s); }
//////////////////////////////////////
}
