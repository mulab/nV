#include "pythonObject.h"
#include <nV/utils.h>
#include <algorithm>

namespace nV {

sym PythonObject::$Python, PythonObject::$PythonObject;
bool PythonObject::Inited = false;
void PythonObject::Init(int argc, char* argv[]) {
    if (Inited)
        return;
	$Python = SYM(root, Python);
    $PythonObject = SYM($Python, Object);

	if (!Py_IsInitialized()) {
		PyObject *nVm, *mainm, *maind;
		char *argv[] = {"<nV>", 0};
		Py_SetProgramName("<nV>");
		Py_Initialize();
		PySys_SetArgv(1, argv);

		//设置import目录
		char *prefix_path = Py_GetPrefix();
		Py_SetPythonHome(prefix_path);

		char path[100];

		strcpy(path, prefix_path);
		strcat(path, "\\Lib");
		Py_SetPythonHome(path);


		strcpy(path, prefix_path);
		strcat(path, "\\DLLs");
		Py_SetPythonHome(path);


		/* Import 'nV' automatically. */          
		nVm = PyImport_ImportModule("nV");
		if (!nVm) {
			throw std::logic_error("Can't import nV module\n");
		} else {
			mainm = PyImport_AddModule("__main__");
			if (!mainm) {
				throw std::logic_error("Can't get __main__ module\n");
			} else {
				maind = PyModule_GetDict(mainm);
				PyDict_SetItemString(maind, "nV", nVm);
				//放这个地方？？？？
				Py_DECREF(nVm);
			}
		}
	}
	Inited = true;
}

void PythonObject::Close() {

/*	 if (!$Python || $Python->refcount() > 1)
        return;
    clear($Python);
    const_cast<Symbol*>($Python)->discard();*/

	if (Py_IsInitialized()){
		Py_Finalize();
	}
    Inited = false;
}

PythonObject* PythonObject::clone() const {
	if (obj){
		Py_INCREF(obj);
		return new PythonObject(obj);
	}
	else
		return new PythonObject(0);
}
int PythonObject::compare(const Object& x) const {

	int result;
	
	if (PyObject_Cmp(obj, x.cast<PythonObject>().obj, &result) == -1){
		return 0;
		PyErr_Clear();
	}
    return result;
}
bool PythonObject::equal(const Object& $other) const {
	return (compare($other) == 1);
}
size_t PythonObject::hash() const {
	if (obj)
		return PyObject_Hash(obj);
	return 0;
}
void PythonObject::print(wostream& o) const {
    if (obj) {
		PyObject *repr = PyObject_Str(obj);
		if (!repr) {
			char buf[256];
			snprintf(buf, 256, "python object: %p", obj);
			o << mbs2wcs(buf);
			PyErr_Clear();
		} else {
			char *s;
			int len;
			//PyString_AsStringAndSize返回值为0是正常的？怎么与标准说明不一样
			if (0 != PyString_AsStringAndSize(repr, &s, &len) || 0 == s) {
				PyErr_SetString(PyExc_RuntimeError, "can not get string");
				return;
			}
			o << mbs2wcs(s);
			Py_DECREF(repr);
			free(s);
		}
		//obj->ob_type->tp_print(obj, stdout, 0);
        return;
    }
}


}