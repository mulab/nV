#pragma once
#include "mULink.h"

namespace nV {
class mUObject : public Object {
public:
    API static sym $mU, $mUObject;
    API static bool Inited;
    API static void Init(Kernel&);
    API static void Main(int argc, char* argv[]);
    API static void Close();
    template<class T>
    inline static mUObject* New(T x) {
        return new mUObject(to_mu(x));
    }

    mU::var obj;
    API virtual mUObject* clone() const;
    API virtual int compare(const Object&) const;
    API virtual bool equal(const Object&) const;
    API virtual size_t hash() const;
    API virtual void print(wostream&) const;
    mUObject() : Object($mUObject) {}
    mUObject(const mU::var& x) : Object($mUObject), obj(x) {}
    var normal() const {
        return from_mu(obj);
    }
};
}

namespace mU {
struct nvobj_t : obj_t {
	API static var $nV, $nVObj;
	API static nV::Kernel *kernel;
	API static bool Inited;
	API static void Init();
	API static void Main(int argc, char* argv[]);
	API static void Close();
	inline static nvobj_t* New(Var x) {
		return new nvobj_t(nV::from_mu(x));
	}
	API virtual var tag();
	API virtual var normal();
	API virtual void print(wostream&);
	nV::var rep;
	nvobj_t(const nV::var& x) : rep(x) {}
};
inline nV::var& nvobj(Var x) { return ((nvobj_t*)x)->rep; }
}