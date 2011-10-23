#pragma once
extern "C" {
#include <Python.h>
}
#include <nV/Interface.h>
#include <nV/Parser.h>
#include <nV/utils.h>
#include <nV/var.h>


#ifdef _MSC_VER
//#pragma comment(lib,"python27")
#endif

#define NVLINKPYTHON_EXPORTS 1

#undef API
#ifdef _MSC_VER
#if defined(nvlinkpython_EXPORTS) || defined(NVLINKPYTHON_EXPORTS)
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#endif

using namespace nV;


API extern PyTypeObject nVObject_Type;

#define nVObject_Check(op) PyObject_TypeCheck(op, &nVObject_Type)

API var py_convert( PyObject *o);

API Key* toKey(const var& x);

API var py_run(string &command, int eval);

API bool python_assign(Kernel& k, const Tuple& x, const var& y);

API void python_object(Kernel& k, var& r, Tuple& x);

