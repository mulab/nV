#pragma once
extern "C" {
#include <Python.h>
}
#include "nV/Interface.h"
#include "nV/Parser.h"
#include "nV/utils.h"
#include "nV/var.h"

#include "nVutils.h"
#include "nVinPython.h"

namespace nV {

class PythonObject : public Object {
public:
    API static sym $Python, $PythonObject;
    API static bool Inited;
    API static void Init(int argc, char* argv[]);
    API static void Close();
//    API static JNIEnv* Env;
//    API static JavaVM* VM;
//    API static jclass Class;
    template<class T>
    inline static PythonObject* to(T x) {
        return new PythonObject(x);
    }

//    jclass cls;
//    jobject obj;
	PyObject *obj;
    virtual ~PythonObject() {
        //ÐèÌí¼Ó
		Py_DECREF(obj);
    }
    API virtual PythonObject* clone() const;
    API virtual int compare(const Object&) const;
    API virtual bool equal(const Object&) const;
    API virtual size_t hash() const;
    API virtual void print(wostream&) const;
    PythonObject(PyObject* x = 0) : Object($PythonObject), obj(x) {
		if (obj){
			Py_INCREF(obj);
		}
 //       set(x);
    }
    PythonObject& operator=(PyObject* x) {
        //set(x);
    }
    void set(PyObject* x) {
        /*if (!x)
            return;
        if (obj) {
            Env->DeleteGlobalRef(cls);
            Env->DeleteGlobalRef(obj);
        }
        obj = Env->NewGlobalRef(x);
        cls = (jclass)Env->NewGlobalRef(Env->GetObjectClass(obj));*/
    }
    var from() const {
        return py_convert(obj);
    }
};
}