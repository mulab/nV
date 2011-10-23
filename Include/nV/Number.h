#pragma once
#include "Interface.h"

#undef API
#ifdef _MSC_VER
#ifdef NUMBER_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#endif

#undef ASSIGN
#define ASSIGN(f) CASSIGN(System_Number,f)
#undef VALUE
#define VALUE(f) CVALUE(System_Number,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(System_Number,f,sig)

#include "RealI.h"

namespace nV {
// RealI.cpp
API var realI(const Object&);
API var realI(const Object&, const Object&);

// Z.cpp
API var operator-(const Integer&);
API var operator+(const Integer&, const Integer&);
API var operator+(const Integer&, uint);
API var operator-(const Integer&, const Integer&);
API var operator-(const Integer&, uint);
API var operator-(uint, const Integer&);
API var operator*(const Integer&, const Integer&);
API var operator*(const Integer&, uint);
API var operator*(const Integer&, sint);
API var operator/(const Integer&, const Integer&);
API var operator%(const Integer&, const Integer&);
API var operator%(const Integer&, uint);
API var operator~(const Integer&);
API var operator&(const Integer&, const Integer&);
API var operator|(const Integer&, const Integer&);
API var operator^(const Integer&, const Integer&);
API var operator<<(const Integer&, int);
API var operator>>(const Integer&, int);

// Q.cpp
API var operator-(const Rational&);
API var operator+(const Rational&, const Rational&);
API var operator-(const Rational&, const Rational&);
API var operator*(const Rational&, const Rational&);
API var operator/(const Rational&, const Rational&);
API var denominator(const Rational&);
API var denominator(const Object&);
API var numerator(const Rational&);
API var numerator(const Object&);
	
// R.cpp
API var operator-(const Real&);
API var operator+(const Real&, const Real&);
API var operator+(const Real&, uint);
API var operator-(const Real&, const Real&);
API var operator-(const Real&, uint);
API var operator-(uint, const Real&);
API var operator*(const Real&, const Real&);
API var operator*(const Real&, uint);
API var operator/(const Real&, const Real&);
API var operator/(const Real&, uint);
API var operator/(uint, const Real&);

// N.cpp
API var N(Kernel&, const var&, uint = 0);
API double toD(Kernel&, const var&);

//IntervalN.cpp
API var IntervalN(Kernel&, const var&, uint = 0);

// Precision.cpp
API uint Precision(Kernel&, const var&);

namespace Number {

// Cast.cpp
API var cast(const Object&, sym);
API int cmp(const Object&, const Object&);

// Minus.cpp
API var Minus(const Object&);

// Divide.cpp
API var Divide(const Object&);
API var Divide(const Object&, const Object&);

// Plus.cpp
API var Plus(const Object&, const Object&);

// Times.cpp
API var Times(const Object&, const Object&);

// Power.cpp
API var Power(const Object&, const Object&);

// PrimeQ.cpp
API bool PrimeQ(const Integer&);
API var NextPrime(const Integer&);
API uint IntegerLength(const Integer&, uint);
API var BitSet(const Integer&, uint);
API var BitClear(const Integer&, uint);
API bool BitGet(const Integer&, uint);
API var Abs(const Integer&);
API var Sqrt(const Integer&);
API bool EvenQ(const Integer&);
API bool OddQ(const Integer&);
API bool SquareQ(const Integer&);

// RandomInteger.cpp
API void Randomize();
API void SeedRandom(const Integer&);
API void SeedRandom();
API var RandomInteger(const Integer&);
API var RandomInteger(uint);
API var RandomInteger(const Integer&, const Integer&);
API var RandomPrime(const Integer&);
API var RandomPrime(const Integer&, const Integer&);

// CoprimeQ.cpp
API bool CoprimeQ(const Integer&, const Integer&);
API bool Divisible(const Integer&, const Integer&);
API bool Divisible(const Integer&, uint);
API var DivideExact(const Integer&, const Integer&);
API var DivideExact(const Integer&, uint);
API var Quotient(const Integer&, const Integer&);
API var ExactQuotient(const Integer&, uint);
API var Mod(const Integer&, const Integer&);
API var Mod2(const Integer&, const Integer&);
API var Power(const Integer&, uint);
API var PowerMod(const Integer&, const Integer&, const Integer&);
API var InverseMod(const Integer&, const Integer&);
API var GCD(const Integer&, const Integer&);
API var LCM(const Integer&, const Integer&);
API var ExtendedGCD(const Integer&, const Integer&);

// FactorInteger.cpp
API var FactorInteger(const Integer&);

// Factorial.cpp
API var Factorial(uint);

// Fibonacci.cpp
API var Binomial(uint, uint);
API var Binomial(const Integer&, uint);
API var Fibonacci(uint);
API var Lucas(uint);

// mpfr_wrap.cpp
API void ConstPi(Real&);
API void ConstEuler(Real&);
API void ConstCatalan(Real&);
API void Log(Real&, const Real&);
API void Log2(Real&, const Real&);
API void Log10(Real&, const Real&);
API void Exp(Real&, const Real&);
API void Exp2(Real&, const Real&);
API void Exp10(Real&, const Real&);
API void Cos(Real&, const Real&);
API void Sin(Real&, const Real&);
API void Tan(Real&, const Real&);
API void Sec(Real&, const Real&);
API void Csc(Real&, const Real&);
API void Cot(Real&, const Real&);
API void ArcCos(Real&, const Real&);
API void ArcSin(Real&, const Real&);
API void ArcTan(Real&, const Real&);
API void Cosh(Real&, const Real&);
API void Sinh(Real&, const Real&);
API void Tanh(Real&, const Real&);
API void Sech(Real&, const Real&);
API void Csch(Real&, const Real&);
API void Coth(Real&, const Real&);
API void ArcCosh(Real&, const Real&);
API void ArcSinh(Real&, const Real&);
API void ArcTanh(Real&, const Real&);
API void Gamma(Real&, const Real&);
API void LogGamma(Real&, const Real&);
API void Zeta(Real&, const Real&);
API void Erf(Real&, const Real&);
API void Erfc(Real&, const Real&);
API void Sqrt(Real&, const Real&);
API void Power(Real&, const Real&, const Real&);

// RealI.cpp
API void ConstPi(RealI&);
API void ConstEuler(RealI&);
API void ConstCatalan(RealI&);
API void Log(RealI&, const RealI&);
API void Log2(RealI&, const RealI&);
API void Log10(RealI&, const RealI&);
API void Exp(RealI&, const RealI&);
API void Exp2(RealI&, const RealI&);
API void Cos(RealI&, const RealI&);
API void Sin(RealI&, const RealI&);
API void Tan(RealI&, const RealI&);
API void Sec(RealI&, const RealI&);
API void Csc(RealI&, const RealI&);
API void Cot(RealI&, const RealI&);
API void ArcCos(RealI&, const RealI&);
API void ArcSin(RealI&, const RealI&);
API void ArcTan(RealI&, const RealI&);
API void Cosh(RealI&, const RealI&);
API void Sinh(RealI&, const RealI&);
API void Tanh(RealI&, const RealI&);
API void Sech(RealI&, const RealI&);
API void Csch(RealI&, const RealI&);
API void Coth(RealI&, const RealI&);
API void ArcCosh(RealI&, const RealI&);
API void ArcSinh(RealI&, const RealI&);
API void ArcTanh(RealI&, const RealI&);
API void Sqrt(RealI&, const RealI&);
API void Power(RealI&, const RealI&, const RealI&);

// Pi.cpp
API var Pi(uint = 0);
}
// Power.cpp
inline var operator-(const Object& x) {
	return Number::Minus(x);
}
inline var operator+(const Object& x, const Object& y) {
	return Number::Plus(x, y);
}
inline var operator-(const Object& x, const Object& y) {
    var t = -y;
    if (t.isObject())
        return x + t.object();
    return tuple($.Plus, &x, t);
}
inline var operator*(const Object& x, const Object& y) {
	return Number::Times(x, y);
}
inline var operator/(const Object& x, const Object& y) {
	return Number::Divide(x, y);
}
inline bool operator>(const Object& x, const Object& y) {
	return Number::cmp(x, y) > 0;
}
inline bool operator<=(const Object& x, const Object& y) {
    return !(x > y);
}
inline bool operator<(const Object& x, const Object& y) {
	return Number::cmp(x, y) < 0;
}
inline bool operator>=(const Object& x, const Object& y) {
    return !(x < y);
}
inline bool operator==(const Object& x, const Object& y) {
	return Number::cmp(x, y) == 0;
}
inline bool operator!=(const Object& x, const Object& y) {
    return !(x == y);
}
}
