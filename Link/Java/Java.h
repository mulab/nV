#pragma once
#include <nV/Interface.h>
#include <jni.h>
#ifdef _MSC_VER
#pragma comment(lib,"jvm")
#endif

#undef API
#ifdef _MSC_VER
#if defined(nvlinkjava_EXPORTS) || defined(NVLINKJAVA_EXPORTS)
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#endif

namespace nV {
API jobject var_ptr(JNIEnv*, const var&);
API var invoke(JNIEnv*, jobject, const wstring&, const Tuple&);
API var newInstance(JNIEnv*, jclass, const Tuple&);
API var get(JNIEnv*, jobject, const wstring&);
API bool set(JNIEnv*, jobject, const wstring&, const var&);
API var cast(JNIEnv*, jobject);
API jobject cast(JNIEnv*, double, jclass);
API jobject cast(JNIEnv*, const var&, jclass);

API jclass toClass(JNIEnv*, const wstring&);
API jclass toClass(JNIEnv*, const Object&);
API jclass toClass(JNIEnv*, const var&);
API Key* toKey(const Object&);
API Key* toKey(const var&);
API jobject to_java(JNIEnv*, sym);
API jobject to_java(JNIEnv*, const Key&);
API jobject to_java(JNIEnv*, const Object&);
API jobject to_java(JNIEnv*, const Tuple&);
API jobject to_java(JNIEnv*, const var&);
inline var from_java(JNIEnv* env, jboolean x) {
    return x != 0 ? $.True : $.False;
}
inline var from_java(JNIEnv* env, jchar x) {
    return new String(wstring(1, x));
}
inline var from_java(JNIEnv* env, jshort x) {
    return new Integer((sint)x);
}
inline var from_java(JNIEnv* env, jint x) {
    return new Integer((sint)x);
}
inline var from_java(JNIEnv* env, jlong x) {
    return new Integer((sint)x);
}
inline var from_java(JNIEnv* env, jfloat x) {
    return new Real(x, 0);
}
inline var from_java(JNIEnv* env, jdouble x) {
    return new Real(x, 0);
}
API var from_java(JNIEnv*, jobject);
API var from_java(JNIEnv*, jclass);
API var from_java(JNIEnv*, jstring);
/*class _jbooleanArray : public _jarray {};
class _jbyteArray : public _jarray {};
class _jcharArray : public _jarray {};
class _jshortArray : public _jarray {};
class _jintArray : public _jarray {};
class _jlongArray : public _jarray {};
class _jfloatArray : public _jarray {};
class _jdoubleArray : public _jarray {};
class _jobjectArray : public _jarray {};
*/
}
