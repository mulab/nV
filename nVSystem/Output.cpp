#include <nV/utils.h>
#include <nV/Parser.h>
#include <nV/System.h>

namespace nV {
bool Output(Kernel& k, const var& x, wcs s) {
    wofstream f(wcs2mbs(s).c_str());
    if (f) {
        grammar.print(k, f, x);
		f << endl;
        f.close();
        return true;
    }
    return false;
}
bool OutputAppend(Kernel& k, const var& x, wcs s) {
    wofstream f(wcs2mbs(s).c_str(), ios::app);
    if (f) {
		grammar.print(k, f, x);
		f << endl;
        f.close();
        return true;
    }
    return false;
}
bool Save(Kernel& k, const var& r, wcs s) {
	ofstream f((dump_path() + wcs2mbs(s)).c_str(), ios::binary | ios::trunc);
	if (!f){
		wcout << _W("open wrong") << endl;
	}
	r.save(f);
	f.close();
    return true;
}
bool ParserSave(Kernel& k, wcs s) {
	wifstream f((wcs2mbs(s) + ".txt").c_str());
    if (!f) {
        k.logging(__FUNCTIONW__) << _W("File ") << s << _W(" not found.") << endl;
        return false;
    }
	Parser p(k);
	try{
		p.start(f);
		p.save(wcs2mbs(s).c_str());
	} catch(std::exception& e){
		k.logging(__FUNCTIONW__) << _W("Error occurred while text1 ") << s << _W(", ")
                                 << e.what() << _W("...") << endl;
	}
    f.close();
    return true;
}
}

using namespace nV;

CAPI void METHOD(Output, 2)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    r = boolean(Output(k, x[1], x[2].cast<String>().toS()));
}
CAPI void METHOD(OutputAppend, 2)(Kernel& k, var& r, Tuple& x, var self, sym local) {
    r = boolean(OutputAppend(k, x[1], x[2].cast<String>().toS()));
}
CAPI void VALUE(Save)(Kernel& k, var& r, Tuple& x) {
   if (x.size == 3 && x[2].isObject($.String)) {
        if (!Save(k, x[1], x[2].cast<String>().toS()))
            r = $.Fail;
		else
			r = $.True;
    }
}
CAPI void CVALUE(Parser, Save)(Kernel& k, var& r, Tuple& x) {
   if (x.size == 2 && x[1].isObject($.String)) {
        if (!ParserSave(k, x[1].cast<String>().toS()))
            r = $.Fail;
		else
			r = $.True;
    }
}