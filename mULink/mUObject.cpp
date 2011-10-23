#include "StdAfx.h"
#include <nV/mUObject.h>
#ifdef _MSC_VER
#pragma comment(lib,"nVParser")
#endif


extern "C"
// #ifdef _WIN32
#ifdef _MSC_VER
__declspec(dllimport)
#endif
int mU_main(int argc, char *argv[]);
extern "C"
#ifdef _MSC_VER
__declspec(dllimport)
#endif
int nV_main(int argc, char *argv[], nV::Kernel*);
namespace nV {
sym mUObject::$mU, mUObject::$mUObject;
bool mUObject::Inited = false;
void mUObject::Init(Kernel& k) {
    if (Inited)
        return;
    $mU = SYM(root, mU);
    $mUObject = SYM($mU, Object);
	mU::var& t = mU::OwnValues[mU::Sym(_W("Kernel"), mU::Ctx(_W("nV`")))];
	if (0 == t) {
		t = mU::Int((uint)&k);
		mU::Initialize();
	}
    Inited = true;
}
void mUObject::Main(int argc, char* argv[]) {
    mU_main(argc, argv);
}
void mUObject::Close() {
    Inited = false;
}
mUObject* mUObject::clone() const {
    return new mUObject(obj);
}
int mUObject::compare(const Object& x) const {
    return mU::Compare(obj, x.cast<mUObject>().obj);
}
bool mUObject::equal(const Object& x) const {
    return mU::Same(obj, x.cast<mUObject>().obj);
}
size_t mUObject::hash() const {
    return Object::hash();
}
void mUObject::print(wostream& o) const {
    if (obj)
        mU::Print(mU::Pretty(obj), o);
}
}

namespace mU {
var nvobj_t::$nV, nvobj_t::$nVObj;
nV::Kernel* nvobj_t::kernel = 0;
bool nvobj_t::Inited = false;
void nvobj_t::Init() {
    if (Inited)
        return;
    $nV = Ctx(_W("nV`"));
    $nVObj = Sym(_W("Obj"), $nV);
	if (!kernel) {
		var kernel_ptr = Eval(Sym(_W("Kernel"), $nV));
		if (IntQ(kernel_ptr))
			kernel = reinterpret_cast<nV::Kernel*>(mpz_get_ui(CInt(kernel_ptr)));
		else {
			kernel = new nV::Kernel;
			kernel->parser = new nV::Parser(*kernel);
		}
	}
    Inited = true;
}
void nvobj_t::Main(int argc, char* argv[]) {
	// nV_main(argc, argv, 0); will get a recursion error!
    nV_main(argc, argv, kernel);
}
void nvobj_t::Close() {
	delete kernel;
    Inited = false;
}
var nvobj_t::tag() {
	return $nVObj;
}
var nvobj_t::normal() {
	return nV::to_mu(rep);
}
void nvobj_t::print(wostream& f) {
	if (rep != 0)
		nV::grammar.print(*kernel, f, rep);
}
}