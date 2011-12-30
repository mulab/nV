#pragma once
#include "Java.h"

namespace nV {
/*
class JavaState : public Object {
public:
	JNIEnv* Env;
	JavaVM* VM;
	jclass Class;
    virtual ~JavaState() {
    }
    JavaState(int argc, char* argv[]) : Object($JavaState) {}
};
*/
class JavaObject : public Object {
public:
    API static sym $Java, $JavaObject;
    API static bool Inited;
    API static void Init(int argc, char* argv[]);
    API static void Close();
    API static JNIEnv* Env;
    API static JavaVM* VM;
    API static jclass Class;
    template<class T>
    inline static JavaObject* to(T x) {
        return new JavaObject(to_java(Env, x));
    }

    jclass cls;
    jobject obj;
    virtual ~JavaObject() {
        if (obj) {
            Env->DeleteGlobalRef(cls);
            Env->DeleteGlobalRef(obj);
        }
    }
    API virtual JavaObject* clone() const;
    API virtual int compare(const Object&) const;
    API virtual bool equal(const Object&) const;
    API virtual size_t hash() const;
    API virtual void print(wostream&) const;
    JavaObject(jobject x) : Object($JavaObject), obj(0), cls(0) {
        set(x);
    }
    JavaObject& operator=(jobject x) {
        set(x);
    }
    void set(jobject x) {
        if (!x)
            return;
        if (obj) {
            Env->DeleteGlobalRef(cls);
            Env->DeleteGlobalRef(obj);
        }
        obj = Env->NewGlobalRef(x);
        cls = (jclass)Env->NewGlobalRef(Env->GetObjectClass(obj));
    }
    var from() const {
        return from_java(Env, obj);
    }
};
}