#include "common.h"
#include <nV/mUObject.h>

using namespace mU;

CAPI void mUInstall() {
	nvobj_t::Init();
    // wcout << "#EmbednV# Install Successfully!" << endl;
}
CAPI void mUUninstall() {
	nvobj_t::Close();
    // wcout << "#EmbednV# Uninstall Successfully!" << endl;
}

CAPI CPROC_INT(nV_nV, 1) {
	return nvobj_t::New(At(x,0));
}
CAPI CPROC_INT(nV_Eval, 1) {
	if (StrQ(At(x,0))) {
		wstringstream ss;
		ss << CStr(At(x,0));
		nvobj_t::kernel->parser->start(ss);
		return new nvobj_t(nvobj_t::kernel->eval(
			nvobj_t::kernel->parser->gen(nvobj_t::kernel->parser->parse())));
	}
	if (Tag(At(x,0)) == nvobj_t::$nVObj) {
		return new nvobj_t(nvobj_t::kernel->eval(nvobj(At(x,0))));
	}
	return new nvobj_t(nvobj_t::kernel->eval(nV::from_mu(At(x,0))));
}

namespace {
	char nv_main_argv0[] = "nV";
}
CAPI CPROC(nV_Main) {
	int argc = Size(x) + 1;
	if (argc == 1) {
		char* argv[] = {nv_main_argv0};
		nvobj_t::Main(argc, argv);
	}
	return Null;
}
CAPI CPROC_INT(nV_mU, 1) {
	if (Tag(At(x,0)) == nvobj_t::$nVObj)
		return nV::to_mu(nvobj(At(x,0)));
	return 0;
}
