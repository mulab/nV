#include "pythonObject.h"
#include "nVinPython.h"
#include "nVutils.h"
#include "nV/utils.h"
#include <algorithm>

using namespace nV;

#undef ASSIGN
#define ASSIGN(f) CASSIGN(Python,f)
#undef VALUE
#define VALUE(f) CVALUE(Python,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(Python,f,sig)


CAPI bool ASSIGN(Object)(Kernel& k, const Tuple& x, const var& y) {
	return python_assign(k,x,y);
}

CAPI void VALUE(init)(Kernel& k, var& r, Tuple& x) {
    char* argv = "";
    PythonObject::Init(0, &argv);
    r = null;
    return;
}

CAPI void VALUE(close)(Kernel& k, var& r, Tuple& x) {

	k.destroy(PythonObject::$Python);
	k.destroy(PythonObject::$PythonObject);

    PythonObject::Close();

	r = $.True;
    return;
}


CAPI void VALUE(Object)(Kernel& k, var& r, Tuple& x) {
	python_object(k,r,x);
}

CAPI void VALUE(execute)(Kernel& k, var& r, Tuple& x) {
//	if (x[0].isObject()) {
        if (x.size == 2) {
            Key* h = toKey(x[1]);
            if (h->kind() == Key::String) {
				r = py_run(wcs2mbs(h->toS()),0);
            }
        }
//    }
    return ;
}
CAPI void VALUE(eval)(Kernel& k, var& r, Tuple& x) {
//	if (x[0].isObject()) {
        if (x.size == 2) {
            Key* h = toKey(x[1]);
            if (h->kind() == Key::String) {
				r = py_run(wcs2mbs(h->toS()),1);
            }
        }
//    }
    return ;
}
CAPI void VALUE(locals)(Kernel& k, var& r, Tuple& x) {
	PyObject *locals;

	locals = PyEval_GetLocals();
	if (!locals){
		r = $.Fail; 
		throw std::logic_error("Can't get locals\n");
	}
	r = new PythonObject(locals);
	Py_DECREF(locals);
	return ;
}
CAPI void VALUE(globals)(Kernel& k, var& r, Tuple& x) {
	PyObject *globals;
	PyObject *module = 0;

	globals = PyEval_GetGlobals();
	if (!globals) {
		module = PyImport_AddModule("__main__");
		if (!module) {
			r = $.Fail;
			throw std::logic_error("Can't import __main__ module\n");
		}
		globals = PyModule_GetDict(module);
	}

	if (!globals) {
		r = $.Fail;
		throw std::logic_error("Can't get globals\n");
	}
	 r = new PythonObject(globals);

	 Py_DECREF(globals);
	 return ;
}
CAPI void VALUE(builtins)(Kernel& k, var& r, Tuple& x) {
	PyObject *builtins;
	

	builtins = PyEval_GetBuiltins();
	if (!builtins) {
		r = $.Fail;
		throw std::logic_error("Can't get builtins\n");
		return;
	}
	r = new PythonObject(builtins);

	Py_DECREF(builtins);
	
	return;
}
CAPI void VALUE(import)(Kernel& k, var& r, Tuple& x) {
//	if (x[0].isObject()) {
        if (x.size == 2) {
            Key* h = toKey(x[1]);
            if (h->kind() == Key::String) {

				PyObject * module;
				module = PyImport_ImportModule(wcs2mbs(h->toS()).c_str());
				
				if (!module){
					r = $.Fail;
					string s = string("Can't import ") + wcs2mbs(h->toS()) + string(" module\n");
					throw std::logic_error(s.c_str());
					return;
				}
				r =  new PythonObject(module);
				Py_DECREF(module);
				return;
			}
        }
}


