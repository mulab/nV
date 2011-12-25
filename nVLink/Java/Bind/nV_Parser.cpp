#include "../Java/nV_Parser.h"
#include <nV/Parser.h>
#ifdef _MSC_VER
#pragma comment(lib,"nVParser")
#endif

using namespace nV;

namespace {
inline jfieldID fieidID(JNIEnv* env, jobject obj, const char* field, const char* sig) {
    jclass cls = env->GetObjectClass(obj);
    return env->GetFieldID(cls, field, sig);
}
inline Var* VarPtr(jlong x) {
    return reinterpret_cast<Var*>(x);
}
inline jlong NewRef(Var* x) {
    return reinterpret_cast<jlong>(x->copy());
}
inline jlong NewRef(const var& x) {
    return reinterpret_cast<jlong>(x.ptr->copy());
}
inline Parser* Ptr(jlong x) {
    return reinterpret_cast<Parser*>(x);
}
inline void open(Parser& p, wifstream& f, const char* s) {
    f.open(s);
    if (!f)
        return;
    try {
        p.start(f);
        p.eval();
    } catch (std::exception& e) {
        wcerr << _W("Error occurred while eval ") << s << _W(", ")
              << e.what() << _W("...") << endl;
    }
    f.close();
}
}

/*
 * Class:     nV_Parser
 * Method:    _constructor
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_nV_Parser__1constructor
(JNIEnv * env, jclass cls, jlong $k) {
    Kernel* k = reinterpret_cast<Kernel*>($k);
    return reinterpret_cast<jlong>(new Parser(*k));
}

/*
 * Class:     nV_Parser
 * Method:    _destructor
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_nV_Parser__1destructor
(JNIEnv * env, jclass cls, jlong $cptr) {
    delete Ptr($cptr);
}

/*
 * Class:     nV_Parser
 * Method:    _parse
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_nV_Parser__1parse
(JNIEnv * env, jclass cls, jlong $cptr, jstring x) {
    Parser* cptr = Ptr($cptr);
    const jchar* buf = env->GetStringChars(x, 0);
    wstringstream ss;
    ss << (wcs)buf;
	var r;
    try {
		cptr->start(ss);
		r = cptr->eval();
    } catch (std::exception& e) {
        wcerr << _W("Error occurred while eval ") << ss << _W(", ")
              << e.what() << _W("...") << endl;
    }
    env->ReleaseStringChars(x, buf);
    return NewRef(r);
}