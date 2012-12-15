#include "common.h"
#include "objects/list.h"
#include "objects/interface.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdarg>

namespace maTHmU {
//////////////////////////////////////
namespace
{
typedef struct
{
	std::vector<var> A;
} list_t;
inline list_t* PTR(void *x) { return (list_t*)x; }
inline list_t* PTR(VAR x) { return PTR(x.ptr); }
inline uint32_t SIZE(VAR x) { return PTR(x)->A.size(); }
inline void RESIZE(VAR x,uint32_t i) { PTR(x)->A.resize(i); }
inline var& AT(VAR x, uint32_t i) { return PTR(x)->A[i]; }
inline void CHK(VAR x,int32_t& i) { if(i < 0) i = SIZE(x) + i; }
inline list& CAST(VAR x) { return *(list*)(&x); }
};
using namespace Objects::List;
//////////////////////////////////////
bool list::type(VAR x) { return GetType(x) == Objects::List::LIST; }
list& list::cast(VAR x) { return CAST(x); }
list list::map(UN_OP f, VAR x) { return CAST(Map(f,x)); }
list list::map(BIN_OP f, VAR x, VAR y) { return CAST(Map(f,x,y)); }
list list::map(TER_OP f, VAR x, VAR y, VAR z) { return CAST(Map(f,x,y,z)); }
list::list() : var(Zero()) {}
list::list(uint32_t n, const var *x) : var(New(n,x)) {}
list::list(VAR a, VAR b) : var(New(a,b)) {}
list::list(VAR a, VAR b, VAR c) : var(New(a,b,c)) {}
list::list(VAR a, VAR b, VAR c, VAR d) : var(New(a,b,c,d)) {}
list::list(VAR a, VAR b, VAR c, VAR d, VAR e) : var(New(a,b,c,d,e)) {}
list::list(VAR a, VAR b, VAR c, VAR d, VAR e, VAR f) : var(New(a,b,c,d,e,f)) {}
list list::mid(int32_t x, int32_t y) const { return CAST(Mid(*this,x,y)); }
uint32_t list::size() const { return SIZE(*this); }
void list::resize(uint32_t x) {	RESIZE(*this,x); }
void list::insert(VAR x, int32_t i) { Insert(*this,x,i); }
var list::erase(int32_t i) { return Erase(*this,i); }
list& list::push(VAR x) { return CAST(Push(*this,x)); }
var list::pop() { return Pop(*this); }
void list::fill(VAR x) { Fill(*this,x); }
VAR list::left() const { return Left(*this); }
var& list::left() { return Left(*this); }
VAR list::right() const { return Right(*this); }
var& list::right() { return Right(*this); }
const var* list::begin() const { return Begin(*this); }
const var* list::end() const { return End(*this); }
var* list::begin() { return Begin(*this); }
var* list::end()  { return End(*this); }
VAR list::at(int32_t i) const { CHK(*this,i); return AT(*this,i); }
var& list::at(int32_t i) { CHK(*this,i); return AT(*this,i); }
void list::print() const { Print(*this); std::cout<<std::endl; }
list list::copy() const { return CAST(New(*this)); }
void list::sort(LESS_OP f) const { Sort(*this,f); }
list list::map(UN_OP f) const { return CAST(Map(f,*this)); }
list::operator bool () const { return (bool)SIZE(*this); }
VAR list::operator [] (uint32_t i) const { return AT(*this,i); }
var& list::operator [] (uint32_t i) { return AT(*this,i); }
list& list::operator += (const list& x) { Append(*this,x); return *this; }
list operator & (const list& x, const list& y) { return CAST(Intersection(x,y)); }
list operator | (const list& x, const list& y) { return CAST(Union(x,y)); }
list operator + (const list& x, const list& y) { return CAST(Join(x,y)); }
list operator - (const list& x, const list& y) { return CAST(Complement(x,y)); }
#define LIST_DEF_CMP(op)\
	bool operator op (const list& x,const list& y) { return Cmp(x,y) op 0; }

LIST_DEF_CMP(>)
LIST_DEF_CMP(<)
LIST_DEF_CMP(!=)
LIST_DEF_CMP(>=)
LIST_DEF_CMP(<=)
LIST_DEF_CMP(==)
//////////////////////////////////////
namespace Objects {
namespace List {
//////////////////////////////////////
uint32_t LIST;

var Zero()
{
	list_t *r = new list_t;
	return var(r, LIST);
}
var New(VAR x)
{
	var r = Zero();
	*PTR(r) = *PTR(x);
	return r;
}
var New(uint32_t n, const var *x)
{
	var r = Zero();
	if(x != 0) PTR(r)->A.assign(x,x+n);
	else RESIZE(r,n);
	return r;
}
var New(VAR a, VAR b)
{
    var r = Zero();
    PTR(r)->A.push_back(a);
    PTR(r)->A.push_back(b);
    return r;
}
var New(VAR a, VAR b, VAR c)
{
    var r = Zero();
    PTR(r)->A.push_back(a);
    PTR(r)->A.push_back(b);
    PTR(r)->A.push_back(c);
    return r;
}
var New(VAR a, VAR b, VAR c, VAR d)
{
    var r = Zero();
    PTR(r)->A.push_back(a);
    PTR(r)->A.push_back(b);
    PTR(r)->A.push_back(c);
    PTR(r)->A.push_back(d);
    return r;
}
var New(VAR a, VAR b, VAR c, VAR d, VAR e)
{
    var r = Zero();
    PTR(r)->A.push_back(a);
    PTR(r)->A.push_back(b);
    PTR(r)->A.push_back(c);
    PTR(r)->A.push_back(d);
    PTR(r)->A.push_back(e);
    return r;
}
var New(VAR a, VAR b, VAR c, VAR d, VAR e, VAR f)
{
    var r = Zero();
    PTR(r)->A.push_back(a);
    PTR(r)->A.push_back(b);
    PTR(r)->A.push_back(c);
    PTR(r)->A.push_back(d);
    PTR(r)->A.push_back(e);
    PTR(r)->A.push_back(f);
    return r;
}
var Print(VAR x)
{
	if(SIZE(x) == 0)
	{
		std::cout<<"{}";
		return x;
	}
	std::cout<<'{';
	uint32_t i;
	for(i = 0; i + 1 < SIZE(x); ++i)
	{
		Interface::Print(AT(x,i))(AT(x,i));
		std::cout<<',';
	}
	Interface::Print(AT(x,i))(AT(x,i));
	std::cout<<'}';
	return x;
}
var Mid(VAR x, int32_t begin, int32_t end)
{
	var r = Zero();
	CHK(x,begin);CHK(x,end);
	if(begin >= end || begin < 0 || end > SIZE(x)) return r;
	RESIZE(r,end-begin);
	for(uint32_t i = begin; i < end; ++i) AT(r,i-begin) = AT(x,i);
	return r;
}
void Append(var &x, VAR y)
{
	uint32_t m = SIZE(x); RESIZE(x,m+SIZE(y));;
	for(uint32_t i = 0; i < SIZE(y); ++i) AT(x,i+m) = AT(y,i);
}
var Join(VAR x, VAR y)
{
	var r = Zero(); RESIZE(r,SIZE(x)+SIZE(y));
	for(uint32_t i = 0; i < SIZE(x); ++i) AT(r,i) = AT(x,i);
	for(uint32_t i = 0; i < SIZE(y); ++i) AT(r,i+SIZE(x)) = AT(y,i);
	return r;
}
namespace {
inline int32_t CMP(VAR x, VAR y)
{
	if(GetType(x) > GetType(y)) return 1;
	if(GetType(x) < GetType(y)) return -1;
	return Interface::Cmp(x)(x,y);
}
inline bool LESS(VAR x, VAR y) { return CMP(x,y) < 0; }
}
void Sort(VAR x, LESS_OP f)
{
	if(f == 0) f = LESS;
	std::sort(PTR(x)->A.begin(), PTR(x)->A.end(), f);
}
var Map(UN_OP f, VAR x)
{
	var r = Zero(); RESIZE(r,SIZE(x));
	for(uint32_t i = 0; i < SIZE(x); ++i) AT(r,i) = f(AT(x,i));
	return r;
}
var Map(BIN_OP f, VAR x, VAR y)
{
	if(SIZE(x) > SIZE(y)) return Map(f,y,x);
	var r = Zero(); RESIZE(r,SIZE(x));
	for(uint32_t i = 0; i < SIZE(x); ++i) AT(r,i) = f(AT(x,i),AT(y,i));
	return r;
}
var Map(TER_OP f, VAR x, VAR y, VAR z)
{
	if(SIZE(x) > SIZE(y)) return Map(f,y,x,z);
	if(SIZE(x) > SIZE(z)) return Map(f,z,y,x);
	var r = Zero(); RESIZE(r,SIZE(x));
	for(uint32_t i = 0; i < SIZE(x); ++i) AT(r,i) = f(AT(x,i),AT(y,i),AT(z,i));
	return r;
}
var Intersection(VAR x, VAR y)
{
	Sort(x); Sort(y);
	var r = Zero();
	RESIZE(r,SIZE(x));
	uint32_t xi = 0, yi = 0, ri = 0;
	while(xi < SIZE(x) && yi < SIZE(y))
		switch(CMP(AT(x,xi),AT(y,yi)))
		{
			case -1: ++xi; break;
			case 1: ++yi; break;
			default:
				AT(r,ri++) = AT(x,xi);
				++xi, ++yi; break;
		}
	RESIZE(r,ri); return r;
}
var Complement(VAR x, VAR y)
{
	Sort(x); Sort(y);
	var r = Zero();
	RESIZE(r,SIZE(x));
	uint32_t xi = 0, yi = 0, ri = 0;
	while(xi < SIZE(x) && yi < SIZE(y))
		switch(CMP(AT(x,xi),AT(y,yi)))
		{
			case -1: AT(r,ri++) = AT(x,xi++); break;
			case 1: ++yi; break;
			default: ++xi, ++yi; break;
		}
	while(xi < SIZE(x)) AT(r,ri++) = AT(x,xi++);
	RESIZE(r,ri); return r;
}
var Union(VAR x, VAR y)
{
	Sort(x); Sort(y);
	var r = Zero();
	RESIZE(r,SIZE(x)+SIZE(y));
	uint32_t xi = 0, yi = 0, ri = 0;
	while(xi < SIZE(x) && yi < SIZE(y))
		switch(CMP(AT(x,xi),AT(y,yi)))
		{
			case -1: AT(r,ri++) = AT(x,xi++); break;
			case 1: AT(r,ri++) = AT(y,yi++); break;
			default:
				AT(r,ri++) = AT(x,xi);
				++xi, ++yi; break;
		}
	while(xi < SIZE(x)) AT(r,ri++) = AT(x,xi++);
	while(yi < SIZE(y)) AT(r,ri++) = AT(y,yi++);
	RESIZE(r,ri); return r;
}
uint32_t Size(VAR x) { return SIZE(x); }
void Resize(VAR x, uint32_t i) { RESIZE(x,i); }
var& Left(VAR x) { return PTR(x)->A.front(); }
var& Right(VAR x) { return PTR(x)->A.back(); }
var* Begin(VAR x) { return SIZE(x) == 0 ? 0 : &PTR(x)->A.front(); }
var* End(VAR x) { return SIZE(x) == 0 ? 0 : &PTR(x)->A.back() + 1; }
var& At(VAR x, int32_t i) { CHK(x,i); return AT(x,i); }
void Insert(VAR x, VAR y, int32_t i) { CHK(x,i); PTR(x)->A.insert(PTR(x)->A.begin()+i,y); }
var Erase(VAR x, int32_t i) { CHK(x,i); var r = PTR(x)->A[i]; PTR(x)->A.erase(PTR(x)->A.begin()+i); return r; }
var& Push(VAR x, VAR y) { PTR(x)->A.push_back(y); return (var&)x; }
var Pop(VAR x) { var r = PTR(x)->A.back(); PTR(x)->A.pop_back(); return r; }
void Fill(VAR x, VAR y) { PTR(x)->A.assign(SIZE(x),y); }
int32_t Cmp(VAR x, VAR y)
{
	if(SIZE(x) > SIZE(y)) return 1;
	if(SIZE(x) < SIZE(y)) return -1;
	for(uint32_t i = 0; i < SIZE(x); ++i)
	{
		int32_t flag = CMP(AT(x,i),AT(y,i));
		if(flag != 0) return flag;
	}
	return 0;
}
void DESTROY(void *x) { delete PTR(x); }
#define MYREG(op) REG(op,LIST)
void Init()
{
	LIST = AddType(DESTROY);
	MYREG(Zero);
	MYREG(New);
	MYREG(Cmp);
	MYREG(Print);
}
//////////////////////////////////////
}
}
}
