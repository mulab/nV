#include "../Java/nV_Kernel.h"
#include <nV/Kernel.h>
#ifdef _MSC_VER
#pragma comment(lib,"nVKernel")
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
inline Kernel* Ptr(jlong x) {
    return reinterpret_cast<Kernel*>(x);
}
}

/*
 * Class:     nV_Kernel
 * Method:    _constructor
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_nV_Kernel__1constructor
(JNIEnv * env, jclass cls) {
    return reinterpret_cast<jlong>(new Kernel());
}

/*
 * Class:     nV_Kernel
 * Method:    _destructor
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_nV_Kernel__1destructor
(JNIEnv * env, jclass cls, jlong $cptr) {
    delete Ptr($cptr);
}

/*
 * Class:     nV_Kernel
 * Method:    _eval
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_nV_Kernel__1eval
(JNIEnv * env, jclass cls, jlong $cptr, jlong x) {
    Kernel* cptr = Ptr($cptr);
    return NewRef(cptr->eval(VarPtr(x)));
}