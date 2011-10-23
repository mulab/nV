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

typedef struct {
	PyObject_HEAD
	var r;
} nVObject;

API extern PyTypeObject nVObject_Type;

#define nVObject_Check(op) PyObject_TypeCheck(op, &nVObject_Type)

PyObject *nVConvert(const var& r);

var py_convert( PyObject *o);

API void initnV(void);
