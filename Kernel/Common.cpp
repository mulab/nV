#include <nV/Common.h>
#include <cstring>
#include <limits>
#ifdef _MSC_VER
void (*__gmp_free_func)(void*, size_t);
#endif

namespace nV {
var var::head() const {
	switch (primary()) {
	case Primary::Symbol:
		return $.Symbol;
	case Primary::Key:
		return $.Key;
	case Primary::Object:
		return object().type;
	case Primary::Tuple:
		return tuple()[0];
	}
	return null;
}
bool isCertain(const Tuple& x) {
    if (x[0].isSymbol() && $.Patterns.count(x[0].symbol()) > 0)
        return false;
    for (uint i = 0; i < x.size; ++i)
        if (!isCertain(x[i]))
            return false;
    return true;
}
namespace {
inline size_t abs(int x) {
    return x < 0 ? -x : x;
}
inline size_t hash(mp_limb_t* begin, mp_limb_t* end) {
    size_t r = 2166136261U;
    while (begin < end) {
        r = 16777619U * r ^ *begin;
        ++begin;
    }
    return r;
}
}
size_t mpz_hash(mpz_srcptr x) {
    return hash(x->_mp_d, x->_mp_d + abs(x->_mp_size));
}
size_t mpq_hash(mpq_srcptr x) {
    size_t r = mpz_hash(mpq_numref(x));
    r = 16777619U * r ^ mpz_hash(mpq_denref(x));
    return r;
}
size_t mpf_hash(mpf_srcptr x) {
    size_t r = hash(x->_mp_d, x->_mp_d + abs(x->_mp_size));
    r = 16777619U * r ^ x->_mp_exp;
    return r;
}
void Integer::print(wostream& o) const {
    char* buf = mpz_get_str(0, 10, mpz);
    o << buf;
    (*__gmp_free_func)(buf, strlen(buf) + 1);
}
void Rational::print(wostream& o) const {
    char* gbuf = mpq_get_str(0, 10, mpq);
    o << gbuf;
    (*__gmp_free_func)(gbuf, strlen(gbuf) + 1);
}
void Real::print(wostream& o) const {
    long exp;
    const size_t n_digits = static_cast<size_t>(LOG_10_2 * prec + 0.5);
    const size_t req_size = n_digits + 2;
    char* buf = new char[req_size];
    const char* const buf2 = mpf_get_str(buf, &exp, 10, n_digits, mpf);
    wstring t(buf2, buf2 + strlen(buf2));
    delete buf;
    wcs s = t.c_str();
    if (exp > 0) {
        if (s[0] == _W('-')) {
            if (exp + 1 <= static_cast<long>(t.size())) {
                o.write(s, exp + 1);
                o << _W('.') << s + exp + 1;
            } else {
                o << s;
                o << wstring(exp + 1 - t.size(), _W('0'));
                o << _W('.');
            }
        } else {
            if (exp <= static_cast<long>(t.size())) {
                o.write(s, exp);
                o << _W('.') << s + exp;
            } else {
                o << s;
                o << wstring(exp - t.size(), _W('0'));
                o << _W('.');
            }
        }
    } else {
        if (s[0] == _W('-'))
            o << _W("-0.") << wstring(-exp, _W('0')) << s + 1;
        else
            o << _W("0.") << wstring(-exp, _W('0')) << s;
    }
}
double NaN = std::numeric_limits<double>().quiet_NaN();
void String::print(wostream& o) const {
	o << _W('"');
    nV::print(str.c_str(), o);
	o << _W('"');
}
}
