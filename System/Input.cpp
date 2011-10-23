#include <nV/System.h>
#include <nV/Parser.h>
#ifdef _MSC_VER
#pragma comment(lib,"nVParser")
#endif
#include <nV/utils.h>

namespace nV {
bool Input(Kernel& k, var& r, wcs s) {
    wifstream f(wcs2mbs(s).c_str());
    if (!f) {
        k.logging(__FUNCTIONW__) << _W("File ") << s << _W(" not found.") << endl;
        return false;
    }
    Parser p(k);
    try {
        p.start(f);
        r = p.eval();
    } catch (std::exception& e) {
        k.logging(__FUNCTIONW__) << _W("Error occurred while eval ") << s << _W(", ")
                                 << e.what() << _W("...") << endl;
    }
    f.close();
    return true;
}
bool Load(Kernel& k, var& r, wcs s) {
	ifstream f((dump_path() + wcs2mbs(s)).c_str(), ios::binary);
    if (!f) {
        k.logging(__FUNCTIONW__) << _W("File ") << s << _W(" not found.") << endl;
        return false;
    }
	try{
		r = load(f);
	} catch(std::exception& e){
		k.logging(__FUNCTIONW__) << _W("Error occurred while load ") << s << _W(", ")
                                 << e.what() << _W("...") << endl;
	}
    f.close();
    return true;
}
bool ParserLoad(Kernel& k, var& r, wcs s) {
	try{
		Parser p(k);
		int root_index = p.load(wcs2mbs(s).c_str());
		r = p.gen(root_index);
	} catch(std::exception& e){
		k.logging(__FUNCTIONW__) << _W("Error occurred while load1 ") << s << _W(", ")
                                 << e.what() << _W("...") << endl;
	}
    return true;
}
bool Parse(Kernel& k, var& r, wcs s) {
	wifstream f(wcs2mbs(s).c_str());
    if (!f) {
        k.logging(__FUNCTIONW__) << _W("File ") << s << _W(" not found.") << endl;
        return false;
    }
	try{
		Parser p(k);
		p.start(f);
		r = p.parseall();
	} catch(std::exception& e){
		k.logging(__FUNCTIONW__) << _W("Error occurred while load ") << s << _W(", ")
                                 << e.what() << _W("...") << endl;
	}
    f.close();
    return true;
}
}

using namespace nV;

CAPI void VALUE(Input)(Kernel& k, var& r, Tuple& x) {
    if (x.size == 1) {
        wstring buf;
        std::getline(wcin, buf);
        wistringstream iss(buf);
        Parser p(k);
        p.start(iss);
        r = p.eval();
        return;
    }
    if (x.size == 2 && x[1].isObject($.String)) {
        if (!Input(k, r, x[1].cast<String>().toS()))
            r = $.Fail;
    }
}


CAPI void VALUE(Load)(Kernel& k, var& r, Tuple& x) {
   if (x.size == 2 && x[1].isObject($.String)) {
        if (!Load(k, r, x[1].cast<String>().toS()))
            r = $.Fail;
    }
}

CAPI void VALUE(Parse)(Kernel& k, var& r, Tuple& x) {
   if (x.size == 2 && x[1].isObject($.String)) {
        if (!Parse(k, r, x[1].cast<String>().toS()))
            r = $.Fail;
    }
}
CAPI void CVALUE(Parser, Load)(Kernel& k, var& r, Tuple& x) {
	if (x.size == 2 && x[1].isObject($.String)) {
		if (!ParserLoad(k, r, x[1].cast<String>().toS()))
			r = $.Fail;
	}
}