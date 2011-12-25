#include "JavaObject.h"
#include <nV/utils.h>
#include <algorithm>

namespace nV {
jclass toClass(JNIEnv* env, const wstring& x) {
    jmethodID mid = env->GetStaticMethodID(JavaObject::Class, "forName", "(Ljava/lang/String;)Ljava/lang/Class;");
    jstring str = env->NewString((const jchar*)x.c_str(), x.size());
    jclass r = (jclass)env->CallStaticObjectMethod(JavaObject::Class, mid, str);
    env->DeleteLocalRef(str);
    if (!r)
        r = env->FindClass(wcs2mbs(x).c_str());
    return r;
}
jclass toClass(JNIEnv* env, const Object& x) {
    if (x.type == $.String)
        return toClass(env, x.cast<String>().str);
    if (x.type == JavaObject::$JavaObject) {
        jobject obj = x.cast<JavaObject>().obj;
        if (env->IsInstanceOf(obj, JavaObject::Class))
            return (jclass)obj;
        return x.cast<JavaObject>().cls;
    }
    return 0;
}
jclass toClass(JNIEnv* env, const var& x) {
    if (x.isSymbol()) {
        wstring s = x.symbol()->toS();
        std::replace(s.begin(), s.end(), _W('`'), _W('.'));
        return toClass(env, s);
    }
    if (x.isObject())
        return toClass(env, x.object());
    return 0;
}
Key* toKey(const Object& x) {
    if (x.type == $.String)
        return key(wstr(x.cast<String>().str));
    return key((uint)toD(x));
}
Key* toKey(const var& x) {
    switch (x.primary()) {
    case  Primary::Key:
        return &x.key();
    case Primary::Symbol:
        return key(x.symbol()->name());
    case Primary::Object:
        return toKey(x.object());
    }
    // return Key::null;
    return key((wcs)0);
}
jobject to_java(JNIEnv* env, sym x) {
    wstring buf = x->toS();
    return env->NewString((const jchar*)buf.c_str(), buf.size());
}
jobject to_java(JNIEnv* env, const Key& x) {
    if (x.kind() == Key::String) {
        wcs buf = x.toS();
        return env->NewString((const jchar*)buf, wcslen(buf));
    }
    jclass cls = env->FindClass("Ljava/lang/Long;");
    jmethodID mid = env->GetMethodID(cls, "<init>", "(J)V");
    jobject obj = env->NewObject(cls, mid, (jlong)x.toUI());
    env->DeleteLocalRef(cls);
    return obj;
}
jobject to_java(JNIEnv* env, const Object& x) {
    if (x.type == $.Integer) {
        const mpz_t& mpz = x.cast<Integer>().mpz;
        if (mpz_fits_slong_p(mpz)) {
            jclass cls = env->FindClass("Ljava/lang/Long;");
            jmethodID mid = env->GetMethodID(cls, "<init>", "(J)V");
            jobject obj = env->NewObject(cls, mid, (jlong)mpz_get_si(mpz));
            env->DeleteLocalRef(cls);
            return obj;
        }
        return var_ptr(env, &x);
    }
    if (x.type == $.Real) {
        const mpf_t& mpf = x.cast<Real>().mpf;
        if (mpf_get_prec(mpf) == mpf_get_default_prec()) {
            jclass cls = env->FindClass("Ljava/lang/Double;");
            jmethodID mid = env->GetMethodID(cls, "<init>", "(D)V");
            jobject obj = env->NewObject(cls, mid, (jdouble)mpf_get_d(mpf));
            env->DeleteLocalRef(cls);
            return obj;
        }
        return var_ptr(env, &x);
    }
    if (x.type == $.String) {
        const wstring& buf = x.cast<String>().str;
        return env->NewString((const jchar*)buf.c_str(), buf.size());
    }
    if (x.type == JavaObject::$JavaObject)
        return x.cast<JavaObject>().obj;
    return var_ptr(env, &x);
}
jobject to_java(JNIEnv* env, const Tuple& x) {
    jclass cls = env->FindClass("Ljava/lang/Object;");
    jobjectArray r = env->NewObjectArray(x.size, cls, NULL);
    env->DeleteLocalRef(cls);
    for (uint i = 0; i < x.size; ++i)
        env->SetObjectArrayElement(r, i, to_java(env, x[i]));
    return r;
}
jobject to_java(JNIEnv* env, const var& x) {
    switch (x.primary()) {
    case Primary::Object:
        return to_java(env, x.object());
    case Primary::Key:
        return to_java(env, x.key());
    case Primary::Symbol:
        return to_java(env, x.symbol());
    case Primary::Tuple:
        return to_java(env, x.tuple());
    }
    return var_ptr(env, x);
}
var from_java(JNIEnv* env, jobject obj) {
    /*jmethodID mid = env->GetMethodID(cls, "toString","()Ljava/lang/String;");
    jstring str = (jstring)env->CallObjectMethod(obj, mid);
    const jchar* buf = env->GetStringChars(str, 0);
    String* r = new String((wcs)buf);
    env->ReleaseStringChars(str, buf);
    return r;
    */
    return cast(env, obj);
}
var from_java(JNIEnv* env, jclass x) {
    jclass cls = env->GetObjectClass(x);
    jmethodID mid = env->GetMethodID(cls, "getName", "()Ljava/lang/String;");
    env->DeleteLocalRef(cls);
    jstring t = (jstring)env->CallObjectMethod(x, mid);
    var r = from_java(env, t);
    env->DeleteLocalRef(t);
    return r;
}
var from_java(JNIEnv* env, jstring x) {
    const jchar* buf = env->GetStringChars(x, 0);
    var r = new String((wcs)buf);
    env->ReleaseStringChars(x, buf);
    return r;
}
}