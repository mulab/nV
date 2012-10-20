#include "common.h"
#include <nV/mUObject.h>

using namespace nV;

CAPI void Install(Kernel& k) {
	mUObject::Init(k);	
    // wcout << "#EmbedmU# Install Successfully!" << endl;
}
CAPI void Uninstall(Kernel& k) {
    mUObject::Close();
    wcout << "#EmbedmU# Uninstall Successfully!" << endl;
}

#undef ASSIGN
#define ASSIGN(f) CASSIGN(mU,f)
#undef VALUE
#define VALUE(f) CVALUE(mU,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(mU,f,sig)

CAPI void VALUE(mU)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 2 && x[0].isSymbol()) {
        r = mUObject::New(x[1]);
        return;
    }
}
CAPI void VALUE(Eval)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 2) {
		if (x[1].isObject()) {
			if (x[1].object().type == $.String) {
				r = new mUObject(mU::ToExpression(x[1].cast<String>().str));
				return;
			}
			if (x[1].object().type == mUObject::$mUObject) {
				r = new mUObject(mU::SafeEval(x[1].cast<mUObject>().obj));
				return;
			}
		}
		r = new mUObject(mU::SafeEval(to_mu(x[1])));		
    }
}

namespace {
	char mu_main_argv0[] = "mU";
}

CAPI void VALUE(Main)(Kernel& k, var& r, Tuple& x) {
    int argc = x.size;
    if (argc == 1) {
        char* argv[] = {mu_main_argv0};
        mUObject::Main(argc, argv);
    }
}
CAPI void VALUE(Object)(Kernel& k, var& r, Tuple& x) {
    if (x[0].isObject(mUObject::$mUObject)) {
        r = new mUObject(mU::SafeEval(to_mu(x)));
    }
}
CAPI void METHOD(nV, 1)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    r = x[1].cast<mUObject>().normal();
}
CAPI void METHOD(Read, 2)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    if (x[2].isSymbol()) {
        wistringstream iss(x[1].cast<String>().str);
        r = new mUObject(mU::Read(iss, to_mu(x[2].symbol())));
    }
}

#undef ASSIGN
#define ASSIGN(f) CASSIGN(mU_Object,f)
#undef VALUE
#define VALUE(f) CVALUE(mU_Object,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(mU_Object,f,sig)

CAPI void METHOD(eval, 0)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    r = new mUObject(mU::SafeEval(self.cast<mUObject>().obj));
}
CAPI void METHOD(nV, 0)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    r = self.cast<mUObject>().normal();
}
