#include "NTL_wrap.h"
#include <cstring>
#include <gmp.h>

#ifdef _MSC_VER
#ifdef _DEBUG
#pragma comment(lib, "NTLd")
#else
#pragma comment(lib, "NTL")
#endif
#endif

using namespace NTL;

namespace nV {
namespace {
/// memory allocation for ZZ and mpz_t
//
// ZZ contains rep of type NTL_verylong(ZZ.h:33 declaration of ZZ)
// g_lip.h:468:#define NTL_verylong _ntl_gbigint
// g_lip.h:9:typedef void *_ntl_gbigint;
// See g_lip_impl.h:87 for implementation of bigint
// So we have
// ZZ:|NTL_verylong rep|->|long|long|mp_limb_t ...|
//
// gmp.h:202 declaration of  __mpz_struct
// typedef __mpz_struct mpz_t[1];(gmp.h:216)
// mpz_t:|int _mp_alloc|int _mp_size|mp_limb_t* _mp_d|->|mp_limb_T ...|

// print the memory info of mpz_t
void print_mem(const mpz_t& x) {
    std::cout << "value is ";
    mpz_out_str(0, 10, x);
    std::cout << endl;
    std::cout << "Memory allocation :\n";
    printf("The address of limbs is %p\n", x[0]._mp_d);
    std::cout << "alloc= " << x[0]._mp_alloc << endl;
    std::cout << "size= " << x[0]._mp_size << endl;
    for(uint i = 0; i < x[0]._mp_alloc; i++) {
        printf("limb %i is : %lx\n", i, x[0]._mp_d[i]);
    }
}

// print the memory info of ZZ
void print_mem(const ZZ& x) {
    std::cout << "value is " << x << endl;
    std::cout << "Memory allocation :\n";
    long* info = (long*) x.rep;
    long alloc = (*info) >> 2;
    long continue_flag = *(info) & 2;
    long frozen_flag = *(info) & 1;
    long size = *(info + 1);
    std::cout << "alloc= " << alloc << endl;
    std::cout << "continue_flag is " << continue_flag << endl;
    std::cout << "frozen_flag is " << frozen_flag << endl;
    std::cout << "size= " << size << endl;
    mp_limb_t* limbs = (mp_limb_t*)(info + 2);
    printf("The address of limbs is %p\n", limbs);
    for(uint i = 0; i < alloc; i++) {
        printf("limb %i is : %lx\n", i, limbs[i]);
    }
}

ZZ conv(const mpz_t& x) {
    // ZZ.h:77:~ZZ() { NTL_zfree(&rep); }
    // g_lip.h:489:#define NTL_zfree _ntl_gfree
    // g_lip.h:411:    void _ntl_gfree(_ntl_gbigint *x);
    // g_lip_impl.h:437:void _ntl_gfree(_ntl_gbigint *xx)
    //NTL_zfree(&y.rep);
    uint n = mpz_size(x);
    ZZ r(INIT_SIZE, n);
    // *r.rep = realloc(r.rep, 2 * sizeof(long) + x[0]._mp_alloc * sizeof(mp_limb_t));
    // r.SetSize(n);
    long* info = (long*) r.rep;
    *info = (long) x[0]._mp_alloc;
    *info <<= 2;
    *(info + 1) = (long) x[0]._mp_size;
    memcpy(info + 2, x[0]._mp_d, n * sizeof(mp_limb_t));
    return r;
}
void conv(mpz_ptr r, const ZZ& x) {
    long* info = (long*) x.rep;
    if(info) {
        long alloc = *info >> 2;
        long size = *(info + 1);
        mpz_realloc2(r, abs(size) * 8 * sizeof(mp_limb_t));
        r->_mp_size = size;
        memcpy(r->_mp_d, info + 2, abs(size) * sizeof(mp_limb_t));// size may be negative
    } else
        mpz_set_ui(r, 0);
}
}
ZZ to_ZZ(const Integer& x) {
    return conv(x.mpz);
}
var from_ZZ(const ZZ& x) {
    Integer* r = new Integer();
    conv(r->mpz, x);
    return r;
}
//Assume the Tuple[1..] contains the coefs in mpz_t
ZZX to_ZZX(const Tuple& x) {
    uint n = x.size - 1;
    ZZX r(INIT_SIZE, n);
    r.rep.SetLength(n);
    /*for(uint i = 0; i < n; i++) {
      conv(x[i + 1].cast<Integer>().mpz, r.rep[i]);
      cout << r.rep[i] << endl;
      }
    */
    for(uint i = 0; i < n; i++)
        SetCoeff(r, i, to_ZZ(x[i + 1].cast<Integer>()));
    return r;
}
Tuple* from_ZZX(const ZZX& x) {
    uint n = x.rep.length();
    if(n>0){
      Tuple* r = list(n);
      for(uint i = 0; i < n; ++i)
        r->tuple[i + 1] = from_ZZ(x.rep[i]);
      return r;
    }
    else
      return list(new Integer(0L));
}
}
