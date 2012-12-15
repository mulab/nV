#pragma once

#include "objects.h"

namespace maTHmU {
namespace Modules {
namespace NumberTheory {
//////////////////////////////////////
void Init();

void Randomize();
void SeedRandom(VAR n);
void SeedRandom();
Z RandomInteger(VAR n);
Z RandomPrime(VAR n);
Z NextPrime(VAR n);
Z NextPrime(VAR n, VAR k);

int32_t IntegerLength(VAR n, uint32_t b);

Z BitAnd(VAR n, VAR m);
Z BitOr(VAR n, VAR m);
Z BitXor(VAR n, VAR m);
Z BitNot(VAR n);
Z BitSet(VAR n, uint32_t k);
Z BitClear(VAR n, uint32_t k);
int32_t BitGet(VAR n, uint32_t k);

Z BitShiftLeft(VAR n, int32_t k=1);
Z BitShiftRight(VAR n, int32_t k=1);

int32_t OddQ(VAR n);
int32_t EvenQ(VAR n);
Z Abs(VAR n);

Z Sqrt(VAR n);
int32_t SquareQ(VAR n);

Z GCD(VAR n1, VAR n2);
Z GCD(LIST n);
list ExtendedGCD(VAR n1, VAR n2);

Z LCM(VAR n1, VAR n2);
Z LCM(LIST n);

int32_t CoprimeQ(VAR n1, VAR n2);
bool CoprimeQ(LIST l);

int32_t Divisible(VAR n, VAR m);
Z Quotient(VAR m, VAR n);

Z LegendreSymbol(VAR n, VAR m);
Z JacobiSymbol(VAR n, VAR m);
Z KroneckerSymbol(VAR n, VAR m);

Z MoebiusMu(VAR n);
Z EulerPhi(VAR n);
Z PrimeNu(VAR n);

Z Mod(VAR m, VAR n);
Z Power(VAR a, uint32_t b);
Z PowerMod(VAR a, VAR b, VAR m);
Z InverseMod(VAR a, VAR n);

list ContinuedFraction(VAR d, VAR P, VAR Q, uint32_t n);
list ContinuedFraction(VAR d, VAR P, VAR Q);
list Convergents(VAR d, VAR P, VAR Q, uint32_t n);

Z ChineseRemainder(LIST x, LIST m);

list FactorInteger(VAR n);
list Divisors(VAR n);

int32_t PrimeQ(VAR n);

#define MoebiusMu_0 0

Z ExactQuotient(VAR m, uint32_t n);
int32_t Divisible(VAR n, uint32_t m);

void FactorDivide(list& factors,list& remain,int32_t mode);
void FactorCrack(list& factors,list& remain,int32_t mode);

list FactorInteger(VAR N, int32_t mode);
//////////////////////////////////////
}
}
}
