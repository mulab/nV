#pragma once
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#ifdef _DEBUG
#include <typeinfo>
#endif

#undef CAPI
#undef API
#ifdef _MSC_VER
#pragma warning(push,1)
#pragma warning(disable:4819)
#define CAPI extern "C" __declspec(dllexport)
#ifdef KERNEL_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define CAPI extern "C"
#define API
#endif

namespace nV {
typedef signed long sint;
#ifndef _WIN32
#define uint another_uint_type
#endif
typedef unsigned long uint;
// #ifdef _MSC_VER
typedef wchar_t wchar;
typedef wint_t wint;
#define __W(x) L ## x
#define _W(x) __W(x)
using std::wcscmp;
using std::wcslen;
using std::wcin;
using std::wcout;
using std::wcerr;
using std::wclog;
/*
#else
#include <cstdint>
typedef char16_t wchar;
typedef uint_least16_t wint;
#define __W(x) u ## x
#define _W(x) __W(x)
API int wcscmp(const wchar*, const wchar*);
API size_t wcslen(const wchar*);
API extern std::basic_istream<wchar> wcin;
API extern std::basic_ostream<wchar> wcout;
API extern std::basic_ostream<wchar> wcerr;
API extern std::basic_ostream<wchar> wclog;
#endif
*/
typedef const wchar* wcs;
using std::string;
typedef std::basic_string<wchar> wstring;
using std::istream;
typedef std::basic_istream<wchar> wistream;
using std::ostream;
typedef std::basic_ostream<wchar> wostream;
using std::iostream;
typedef std::basic_iostream<wchar> wiostream;
using std::istringstream;
typedef std::basic_istringstream<wchar> wistringstream;
using std::ostringstream;
typedef std::basic_ostringstream<wchar> wostringstream;
using std::stringstream;
typedef std::basic_stringstream<wchar> wstringstream;
using std::ifstream;
typedef std::basic_ifstream<wchar> wifstream;
using std::ofstream;
typedef std::basic_ofstream<wchar> wofstream;
using std::fstream;
typedef std::basic_fstream<wchar> wfstream;
using std::cin;
using std::cout;
using std::cerr;
using std::ios;
typedef std::basic_ios<wchar> wios;
using std::endl;

class Var;
namespace Primary {
enum Type {
    Null = -1,
    Object,
    Key,
    Symbol,
    Tuple
};
const size_t TypeBits = 2;
const size_t TypeSize = 1 << TypeBits;
// const size_t TypeMask = TypeSize - 1;
API extern void(* const TypeRuin[TypeSize])(Var*);
}

class var;
class Object;
class Key;
class Symbol;
class Tuple;
class Kernel;
API Tuple* tuple(uint);
class Var {
    friend class var;
protected:
	uint m_primary : Primary::TypeBits;
	uint m_refcount : sizeof(uint) * 8 - Primary::TypeBits;
	explicit Var(Primary::Type $primary) : m_primary($primary),m_refcount(0) {}
public:
    Var* copy() {
        ++m_refcount;
        return this;
    }
    void pass(Var*& x) {
        if (this)
			++m_refcount;
        if (x)
            x->destroy();
        x = this;
    }
    void destroy() {
        --m_refcount;
        if (m_refcount == 0)
            ruin();
    }
    void discard() {
        if (m_refcount)
            --m_refcount;
		else
            ruin();
    }
	void ruin() {
#ifdef _DEBUG
		delete this;
#else
		Primary::TypeRuin[m_primary](this);
#endif
	}
    template <class T>
    T& cast() const {
        return const_cast<T&>(static_cast<const T&>(*this));
    }
	Primary::Type primary() const { return static_cast<Primary::Type>(m_primary); }
	uint refcount() const { return m_refcount; }
#ifdef _DEBUG
	API virtual ~Var() = 0;
#endif
};

inline bool islower(wchar x) {
    return x > 96 && x < 123;
}
inline bool isupper(wchar x) {
    return x > 64 && x < 91;
}
inline bool isalpha(wchar x) {
    return islower(x) || isupper(x) || x > 0x7F;
}
inline bool isdigit(wchar x) {
    return x > 47 && x < 58;
}
API void print(wchar, wostream& = wcout);
inline void print(wcs x, wostream& o = wcout) {
    if (!x)
        return;
    while(*x)
        print(*x++, o);
}
inline void print(const wstring& x, wostream& o = wcout) {
    print(x.c_str(), o);
}
class Key : public Var {
public:
    API static void ruin(Var*);
    uint key;
    enum Kind {
        Null = -1,
        String,
        Integer
    };
    explicit Key(uint $key) : Var(Primary::Key), key($key) {}
    API long compare(const Key&) const;
    Kind kind() const {
        return key ? static_cast<Kind>(key & 1) : Null;
    }
    wcs toS() const {
        return reinterpret_cast<wcs>(key);
    }
    uint toUI() const {
        return (key >> 1);
    }
    operator bool() const {
        return key != 0;
    }
    API void print(wostream& o = wcout) const;
private:
    API ~Key();
};
inline wostream& operator<<(wostream& o, const Key& x) {
    x.print(o);
    return o;
}

API extern std::unordered_set<wstring> wstrs;
inline wcs wstr(wcs x) {
    return wstrs.insert(x).first->c_str();
}
inline wcs wstr(const wstring& x) {
    return wstr(x.c_str());
}
#define WSTR(x) wstr(_W(#x))
typedef const Symbol* sym;
API extern std::unordered_map<sym, wcs> names;
API extern sym root, sys;
class Symbol : public Var {
public:
    API static void ruin(Var*);
    sym context;
    explicit Symbol(sym $context = root) : Var(Primary::Symbol), context($context) {}
    API wcs name() const;
    API sym clone(wcs = 0) const;
    API long compare(sym) const;
    API sym symbol(wcs) const;
#undef SYM
#define SYM(x,y) x->symbol(WSTR(y))
#undef SYS
#define SYS(x) SYM(sys,x)
    API var get(wcs) const;
    API bool set(wcs, const var&) const;
    API wstring toS(sym = 0) const;
    API void print(wostream& o = wcout) const;
private:
    API ~Symbol();
};
inline wostream& operator<<(wostream& o, sym x) {
    x->print(o);
    return o;
}

class var {
public:
    Var* ptr;
    var() : ptr(0) {}
    var(const Var* $ptr) : ptr($ptr ? const_cast<Var*>($ptr)->copy() : 0) {}
    var(const var& x) : ptr(x.ptr ? x.ptr->copy() : 0) {}
    var& operator =(const Var* x) {
        const_cast<Var*>(x)->pass(ptr);
        return *this;
    }
    var& operator =(const var& x) {
        x.ptr->pass(ptr);
        return *this;
    }
    ~var() {
        if (ptr) ptr->destroy();
    }
    uint refcount() const {
        return ptr ? ptr->refcount() : 0;
    }
    Primary::Type primary() const {
        return ptr ? ptr->primary() : Primary::Null;
    }
    struct ahead {
        API bool operator()(const var&, const var&) const;
    };
	bool isNull() const {
		return ptr == 0;
	}
    bool isObject() const {
        return primary() == Primary::Object;
    }
    bool isObject(sym) const;
    bool isKey() const {
        return primary() == Primary::Key;
    }
    bool isKey(Key::Kind x) const {
        return isKey() && key().kind() == x;
    }
    bool isSymbol() const {
        return primary() == Primary::Symbol;
    }
    bool isTuple() const {
        return primary() == Primary::Tuple;
    }
    bool isTuple(const var&) const;
    bool isTuple(sym) const;
	template <class T>
	T& cast() const {
		return ptr->cast<T>();
	}
#ifdef _DEBUG
	template <>
	Key& cast() const {
		if (isKey())
			return ptr->cast<Key>();
		throw std::bad_cast("Key");
	}
	template <>
	Symbol& cast() const {
		if (isSymbol())
			return ptr->cast<Symbol>();
		throw std::bad_cast("Symbol");
	}
#endif
    sym symbol() const {
        // return static_cast<sym>(ptr);
		return &cast<Symbol>();
    }
    Key& key() const {
        return cast<Key>();
    }
    Object& object() const;
    Tuple& tuple() const;
    API var head() const;
    API var clone() const;
    API long compare(const var&) const;
    API bool equal(const var&) const;
    API size_t hash() const;
    API void print(wostream& = wcout) const;
	API void save(ostream&) const;
    bool operator<(const var& $other) const {
        return this->compare($other) < 0;
    }
    bool operator<=(const var& $other) const {
        return this->compare($other) <= 0;
    }
    bool operator==(const var& $other) const {
        return equal($other);
    }
    bool operator==(sym $other) const {
        return ptr == $other;
    }
    bool operator!=(const var& $other) const {
        return !equal($other);
    }
    bool operator!=(sym $other) const {
        return ptr != $other;
    }
    bool operator>(const var& $other) const {
        return this->compare($other) > 0;
    }
    bool operator>=(const var& $other) const {
        return this->compare($other) >= 0;
    }
	// API size_t size() const;
	// API const var& operator[](uint) const;
	// API var& operator[](uint);
};
inline wostream& operator<<(wostream& o, const var& x) {
    x.print(o);
    return o;
}
API var load(istream&);
API extern const var null;
}

namespace std {
template<>
inline size_t
hash<nV::var>::operator()(
#ifdef _MSC_VER
    const nV::var&
#else
    nV::var
#endif
    x) const {
    return x.hash();
}
}

namespace nV {
API extern std::unordered_map<uint, var> keys;
API Key* key(wcs);
API Key* key(uint);
typedef std::unordered_map<uint, var> Context;
API extern std::unordered_map<sym, Context> contexts;
class Tuple : public Var {
	static const Tuple* unit;
	friend API Tuple* tuple(uint);
public:
    API static void ruin(Var*);
    uint size;
    var tuple[1];
	explicit Tuple(uint n) : Var(Primary::Tuple), size(n) {}
    API Tuple* clone() const;
    API long compare(const Tuple&) const;
    API bool equal(const Tuple&) const;
    API size_t hash() const;
    API void print(wostream& o = wcout) const;
    const var& operator[](uint i) const {
		assert(i < size);
        return tuple[i];
    }
    var& operator[](uint i) {
		assert(i < size);
        return tuple[i];
    }
private:
    API ~Tuple();
};
inline wostream& operator<<(wostream& o, const Tuple& x) {
    x.print(o);
    return o;
}
inline bool var::isTuple(const var& x) const {
    return isTuple() && tuple()[0] == x;
}
inline bool var::isTuple(sym x) const {
    return isTuple() && tuple()[0] == x;
}
inline Tuple& var::tuple() const {
#ifdef _DEBUG
	if (isTuple())
		return cast<Tuple>();
	throw std::bad_cast("Tuple");
#else
    return cast<Tuple>();
#endif
}
inline Tuple* tuple(const var& a) {
    Tuple* r = tuple(1);
    r->tuple[0] = a;
    return r;
}
inline Tuple* tuple(const var& a, const var& b) {
    Tuple* r = tuple(2);
    r->tuple[0] = a;
    r->tuple[1] = b;
    return r;
}
inline Tuple* tuple(const var& a, const var& b, const var& c) {
    Tuple* r = tuple(3);
    r->tuple[0] = a;
    r->tuple[1] = b;
    r->tuple[2] = c;
    return r;
}
inline Tuple* tuple(const var& a, const var& b, const var& c, const var& d) {
    Tuple* r = tuple(4);
    r->tuple[0] = a;
    r->tuple[1] = b;
    r->tuple[2] = c;
    r->tuple[3] = d;
    return r;
}
template <class Iter>
inline Tuple* tuple(uint size, Iter begin) {
    Tuple* r = tuple(size);
    for (uint i = 0; i < size; ++i, ++begin)
        r->tuple[i] = *begin;
    return r;
}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
