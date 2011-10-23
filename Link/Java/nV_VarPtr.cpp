#include "Java/nV_VarPtr.h"
#include "JavaObject.h"
#include <algorithm>
#include <cassert>

namespace {
inline jfieldID fieidID(JNIEnv* env, jobject obj, const char* field, const char* sig) {
    jclass cls = env->GetObjectClass(obj);
    return env->GetFieldID(cls, field, sig);
}
inline nV::Var* VarPtr(jlong x) {
    return reinterpret_cast<nV::Var*>(x);
}
inline jlong NewRef(nV::Var* x) {
    if (x)
        return reinterpret_cast<jlong>(x->copy());
    return 0;
}
inline jlong NewRef(const nV::var& x) {
    return NewRef(x.ptr);
}
}

namespace nV {
jobject var_ptr(JNIEnv* env, const var& x) {
    jclass cls = env->FindClass("LnV/VarPtr;");
    assert(cls);
    jmethodID mid = env->GetMethodID(cls, "<init>", "(J)V");
    assert(mid);
    jobject obj = env->NewObject(cls, mid, NewRef(x));
    env->DeleteLocalRef(cls);
    return obj;
}
var invoke(JNIEnv* env, jobject obj, const wstring& methodName, const Tuple& x) {
    jclass api = env->FindClass("LnV/Interface;");
    assert(api);
    jmethodID mid = env->GetStaticMethodID(api, "_invoke", "(Ljava/lang/Object;Ljava/lang/String;[J)J");
    assert(mid);
    jstring str = env->NewString((const jchar*)methodName.c_str(), methodName.size());
    jlong* buf = new jlong[x.size - 1];
    for (uint i = 1; i < x.size; ++i)
        buf[i - 1] = NewRef(x[i]);
    jlongArray longArr = env->NewLongArray(x.size - 1);
    env->SetLongArrayRegion(longArr, 0, x.size - 1, buf);
    jlong r = env->CallStaticLongMethod(api, mid, obj, str, longArr);
    env->DeleteLocalRef(longArr);
    delete buf;
    env->DeleteLocalRef(str);
    env->DeleteLocalRef(api);
    return VarPtr(r);
}
var newInstance(JNIEnv* env, jclass cls, const Tuple& x) {
    jclass api = env->FindClass("LnV/Interface;");
    assert(api);
    jmethodID mid = env->GetStaticMethodID(api, "_newInstance", "(Ljava/lang/Class;[J)J");
    assert(mid);
    jlong* buf = new jlong[x.size - 1];
    for (uint i = 1; i < x.size; ++i)
        buf[i - 1] = NewRef(x[i]);
    jlongArray longArr = env->NewLongArray(x.size - 1);
    env->SetLongArrayRegion(longArr, 0, x.size - 1, buf);
    jlong r = env->CallStaticLongMethod(api, mid, cls, longArr);
    env->DeleteLocalRef(longArr);
    delete buf;
    env->DeleteLocalRef(api);
    return VarPtr(r);
}
var get(JNIEnv* env, jobject obj, const wstring& fieldName) {
    jclass api = env->FindClass("LnV/Interface;");
    assert(api);
    jmethodID mid = env->GetStaticMethodID(api, "_get", "(Ljava/lang/Object;Ljava/lang/String;)J");
    assert(mid);
    jstring str = env->NewString((const jchar*)fieldName.c_str(), fieldName.size());
    jlong r = env->CallStaticLongMethod(api, mid, obj, str);
    env->DeleteLocalRef(str);
    env->DeleteLocalRef(api);
    return VarPtr(r);
}
bool set(JNIEnv* env, jobject obj, const wstring& fieldName, const var& x) {
    jclass api = env->FindClass("LnV/Interface;");
    assert(api);
    jmethodID mid = env->GetStaticMethodID(api, "_set", "(Ljava/lang/Object;Ljava/lang/String;J)Z");
    assert(mid);
    jstring str = env->NewString((const jchar*)fieldName.c_str(), fieldName.size());
    jboolean r = env->CallStaticBooleanMethod(api, mid, obj, str, NewRef(x));
    env->DeleteLocalRef(str);
    env->DeleteLocalRef(api);
    return r != 0;
}
var cast(JNIEnv* env, jobject obj) {
    jclass api = env->FindClass("LnV/Interface;");
    assert(api);
    jmethodID mid = env->GetStaticMethodID(api, "_cast", "(Ljava/lang/Object;)J");
    assert(mid);
    jlong r = env->CallStaticLongMethod(api, mid, obj);
    env->DeleteLocalRef(api);
    return VarPtr(r);
}
jobject cast(JNIEnv* env, double x, jclass cls) {
    jclass api = env->FindClass("LnV/Interface;");
    assert(api);
    jmethodID mid = env->GetStaticMethodID(api, "cast", "(DLjava/lang/Class;)Ljava/lang/Number;");
    assert(mid);
    jobject r = env->CallStaticObjectMethod(api, mid, (jdouble)x, cls);
    env->DeleteLocalRef(api);
    return r;
}
jobject cast(JNIEnv* env, const	var& x, jclass cls) {
    jclass api = env->FindClass("LnV/Interface;");
    assert(api);
    jmethodID mid = env->GetStaticMethodID(api, "_cast", "(JLjava/lang/Class;)Ljava/lang/Object;");
    assert(mid);
    jobject r = env->CallStaticObjectMethod(api, mid, NewRef(x), cls);
    env->DeleteLocalRef(api);
    return r;
}
}

using namespace nV;

/*
 * Class:     nV_VarPtr
 * Method:    _copy
 * Signature: (J)J
 *
extern "C" JNIEXPORT jlong JNICALL Java_nV_VarPtr__1copy
  (JNIEnv * env, jclass cls, jlong $ptr) {
	  Var* ptr = VarPtr($ptr);
	  if (ptr)
		  ptr->copy();
	  return reinterpret_cast<jlong>(ptr);
}
*/

/*
 * Class:     nV_VarPtr
 * Method:    _destroy
 * Signature: (J)V
 */
extern "C" JNIEXPORT void JNICALL Java_nV_VarPtr__1destroy
(JNIEnv * env, jclass cls, jlong $ptr) {
    Var* ptr = VarPtr($ptr);
    if (ptr)
        ptr->destroy();
}

/*
 * Class:     nV_VarPtr
 * Method:    _newKey
 * Signature: (Ljava/lang/String;)J
 */
extern "C" JNIEXPORT jlong JNICALL Java_nV_VarPtr__1newKey__Ljava_lang_String_2
(JNIEnv * env, jclass cls, jstring x) {
    const jchar* buf = env->GetStringChars(x, 0);
    Key* r = key(wstr((wcs)buf));
    env->ReleaseStringChars(x, buf);
    return NewRef(r);
}

/*
 * Class:     nV_VarPtr
 * Method:    _newKey
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_nV_VarPtr__1newKey__J
(JNIEnv * env, jclass cls, jlong x) {
    return NewRef(key((uint)x));
}

/*
 * Class:     nV_VarPtr
 * Method:    _newSymbol
 * Signature: (Ljava/lang/String;)J
 */
extern "C" JNIEXPORT jlong JNICALL Java_nV_VarPtr__1newSymbol
(JNIEnv * env, jclass cls, jstring x) {
    const jchar* buf = env->GetStringChars(x, 0);
    sym r = root;
    wcs s = (wcs)buf;
    wcs p = s;
    while (*p)
        if (*p == '`') {
            r = r->symbol(wstr(wstring(s, p)));
            s = ++p;
        } else
            ++p;
    r = r->symbol(wstr(s));
    env->ReleaseStringChars(x, buf);
    return NewRef(r);
}

/*
 * Class:     nV_VarPtr
 * Method:    _newSymbol
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_nV_VarPtr__1newSymbol__JLjava_lang_String_2
(JNIEnv * env, jclass cls, jlong $c, jstring x) {
    const jchar* buf = env->GetStringChars(x, 0);
    sym c = (sym)VarPtr($c);
    sym r = c->symbol(wstr((wcs)buf));
    env->ReleaseStringChars(x, buf);
    return NewRef(r);
}

/*
 * Class:     nV_VarPtr
 * Method:    _newTuple
 * Signature: ([J)J
 */
JNIEXPORT jlong JNICALL Java_nV_VarPtr__1newTuple
(JNIEnv * env, jclass cls, jlongArray x) {
    jsize len = env->GetArrayLength(x);
    jlong* buf = env->GetLongArrayElements(x, 0);
    Tuple* r = tuple(len);
    for (jsize i = 0; i < len; ++i)
        r->tuple[i] = VarPtr(buf[i]);
    return NewRef(r);
}


/*
 * Class:     nV_VarPtr
 * Method:    _newInteger
 * Signature: (J)J
 */
extern "C" JNIEXPORT jlong JNICALL Java_nV_VarPtr__1newInteger
(JNIEnv * env, jclass cls, jlong x) {
    return NewRef(new Integer((sint)x));
}

/*
 * Class:     nV_VarPtr
 * Method:    _newInteger
 * Signature: (Ljava/lang/String;)J
 */
extern "C" JNIEXPORT jlong JNICALL Java_nV_VarPtr__1newInteger__Ljava_lang_String_2
(JNIEnv * env, jclass cls, jstring x) {
    const jchar* buf = env->GetStringChars(x, 0);
    jsize len = wcslen((wcs)buf);
    Integer* r = new Integer(string(buf, buf + len).c_str(), 10);
    env->ReleaseStringChars(x, buf);
    return NewRef(r);
}

/*
 * Class:     nV_VarPtr
 * Method:    _newRational
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_nV_VarPtr__1newRational__JJ
(JNIEnv * env, jclass cls, jlong x, jlong y) {
    return NewRef(new Rational((sint)x, (uint)y));
}

/*
 * Class:     nV_VarPtr
 * Method:    _newRational
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_nV_VarPtr__1newRational__Ljava_lang_String_2
(JNIEnv * env, jclass cls, jstring x) {
    const jchar* buf = env->GetStringChars(x, 0);
    jsize len = wcslen((wcs)buf);
    Rational* r = new Rational(string(buf, buf + len).c_str(), 10);
    env->ReleaseStringChars(x, buf);
    return NewRef(r);
}


/*
 * Class:     nV_VarPtr
 * Method:    _newReal
 * Signature: (D)J
 */
extern "C" JNIEXPORT jlong JNICALL Java_nV_VarPtr__1newReal
(JNIEnv * env, jclass cls, jdouble x) {
    return NewRef(new Real((double)x, 0));
}

/*
 * Class:     nV_VarPtr
 * Method:    _newReal
 * Signature: (Ljava/lang/String;)J
 */
extern "C" JNIEXPORT jlong JNICALL Java_nV_VarPtr__1newReal__Ljava_lang_String_2
(JNIEnv * env, jclass cls, jstring x) {
    const jchar* buf = env->GetStringChars(x, 0);
    jsize len = wcslen((wcs)buf);
    Real* r = new Real(string(buf, buf + len).c_str(), 10, len);
    env->ReleaseStringChars(x, buf);
    return NewRef(r);
}

/*
 * Class:     nV_VarPtr
 * Method:    _newString
 * Signature: (Ljava/lang/String;)J
 */
extern "C" JNIEXPORT jlong JNICALL Java_nV_VarPtr__1newString
(JNIEnv * env, jclass cls, jstring x) {
    const jchar* buf = env->GetStringChars(x, 0);
    String* r = new String((wcs)buf);
    env->ReleaseStringChars(x, buf);
    return NewRef(r);
}

/*
 * Class:     nV_VarPtr
 * Method:    _newJavaObject
 * Signature: (Ljava/lang/Object;)J
 */
extern "C" JNIEXPORT jlong JNICALL Java_nV_VarPtr__1newJavaObject
(JNIEnv * env, jclass cls, jobject x) {
    return NewRef(new JavaObject(x));
}

/*
 * Class:     nV_VarPtr
 * Method:    _type
 * Signature: (J)I
 */
extern "C" JNIEXPORT jint JNICALL Java_nV_VarPtr__1type
(JNIEnv * env, jclass cls, jlong $ptr) {
    Var* ptr = VarPtr($ptr);
    if (ptr) {
        Primary::Type t = ptr->primary();
        if (t == Primary::Object) {
            if (ptr->cast<Object>().type == $.Integer)
                return 4;
            if (ptr->cast<Object>().type == $.Rational)
                return 5;
            if (ptr->cast<Object>().type == $.Real)
                return 6;
            if (ptr->cast<Object>().type == $.String)
                return 7;
            if (ptr->cast<Object>().type == JavaObject::$JavaObject)
                return 8;
        }
        return t;
    }
    return Primary::Null;
}

/*
 * Class:     nV_VarPtr
 * Method:    _getSymbol
 * Signature: (I)Ljava/lang/String;
 */
extern "C" JNIEXPORT jstring JNICALL Java_nV_VarPtr__1getSymbol
(JNIEnv * env, jclass cls, jlong $ptr) {
    sym ptr = (sym)VarPtr($ptr);
    wstring buf = ptr->toS();
    return env->NewString((const jchar*)buf.c_str(), buf.size());
}

/*
 * Class:     nV_VarPtr
 * Method:    _getBigInteger
 * Signature: (I)Ljava/math/BigInteger;
 */
extern "C" JNIEXPORT jobject JNICALL Java_nV_VarPtr__1getBigInteger
(JNIEnv * env, jclass, jlong $ptr) {
    Integer* ptr = (Integer*)VarPtr($ptr);
    /*size_t len = mpz_sizeinbase(ptr->mpz, 2) / 8 + 1;
    jbyte* buf = new jbyte[len];
    jbyte* src = (jbyte*)ptr->mpz->_mp_d;
    std::reverse_copy(src, src + len, buf);
    jbyteArray byteArr = env->NewByteArray(len);
    env->SetByteArrayRegion(byteArr, 0, len, buf);
    delete buf;
    jclass cls = env->FindClass("Ljava/math/BigInteger;");
    jmethodID mid = env->GetMethodID(cls, "<init>","(I[B)V");
    jobject obj = env->NewObject(cls, mid, (jint)mpz_sgn(ptr->mpz), byteArr);
    env->DeleteLocalRef(byteArr);
    env->DeleteLocalRef(cls);
    */
    wostringstream oss;
    var(ptr).print(oss);
    jstring str = env->NewString((const jchar*)oss.str().c_str(), oss.str().size());
    jclass cls = env->FindClass("Ljava/math/BigInteger;");
    jmethodID mid = env->GetMethodID(cls, "<init>", "(Ljava/lang/String;)V");
    jobject obj = env->NewObject(cls, mid, str);
    env->DeleteLocalRef(str);
    env->DeleteLocalRef(cls);
    return obj;
}

/*
 * Class:     nV_VarPtr
 * Method:    _getBigDecimal
 * Signature: (I)Ljava/math/BigDecimal;
 */
JNIEXPORT jobject JNICALL Java_nV_VarPtr__1getBigDecimal
(JNIEnv * env, jclass, jlong $ptr) {
    Real* ptr = (Real*)VarPtr($ptr);
    /*mpz_t mpz;
    mpz->_mp_alloc = ptr->mpf->_mp_prec;
    mpz->_mp_d = ptr->mpf->_mp_d;
    mpz->_mp_size = ptr->mpf->_mp_size;
    size_t len = mpz_sizeinbase(mpz, 2) / 8 + 1;
    jbyte* buf = new jbyte[len];
    jbyte* src = (jbyte*)mpz->_mp_d;
    std::reverse_copy(src, src + len, buf);
    jbyteArray byteArr = env->NewByteArray(len);
    env->SetByteArrayRegion(byteArr, 0, len, buf);
    delete buf;
    jclass cls = env->FindClass("Ljava/math/BigInteger;");
    jmethodID mid = env->GetMethodID(cls, "<init>","(I[B)V");
    jobject obj = env->NewObject(cls, mid, (jint)mpz_sgn(mpz), byteArr);
    env->DeleteLocalRef(byteArr);
    env->DeleteLocalRef(cls);
    cls = env->FindClass("Ljava/math/BigDecimal;");
    mid = env->GetMethodID(cls, "<init>","(Ljava/math/BigInteger;I)V");
    jint exp = ptr->mpf->_mp_exp * GMP_NUMB_BITS * LOG_10_2;
    obj = env->NewObject(cls, mid, obj, exp);
    */
    wostringstream oss;
    var(ptr).print(oss);
    jstring str = env->NewString((const jchar*)oss.str().c_str(), oss.str().size());
    jclass cls = env->FindClass("Ljava/math/BigDecimal;");
    jmethodID mid = env->GetMethodID(cls, "<init>", "(Ljava/lang/String;)V");
    jobject obj = env->NewObject(cls, mid, str);
    env->DeleteLocalRef(str);
    env->DeleteLocalRef(cls);
    return obj;
}

/*
 * Class:     nV_VarPtr
 * Method:    _getString
 * Signature: (I)Ljava/lang/String;
 */
extern "C" JNIEXPORT jstring JNICALL Java_nV_VarPtr__1getString
(JNIEnv * env, jclass cls, jlong $ptr) {
    String* ptr = (String*)VarPtr($ptr);
    return env->NewString((const jchar*)ptr->str.c_str(), ptr->str.size());
}

/*
 * Class:     nV_VarPtr
 * Method:    _getJavaObject
 * Signature: (J)Ljava/lang/Object;
 */
JNIEXPORT jobject JNICALL Java_nV_VarPtr__1getJavaObject
(JNIEnv * env, jclass cls, jlong $ptr) {
    JavaObject* ptr = (JavaObject*)VarPtr($ptr);
    return ptr->obj;
}

/*
 * Class:     nV_VarPtr
 * Method:    _tuple
 * Signature: (J)[J
 */
extern "C" JNIEXPORT jlongArray JNICALL Java_nV_VarPtr__1tuple
(JNIEnv * env, jclass cls, jlong $ptr) {
    Tuple* ptr = (Tuple*)VarPtr($ptr);
    jlong* buf = new jlong[ptr->size];
    for (uint i = 0; i < ptr->size; ++i)
        buf[i] = NewRef(ptr->tuple[i]);
    jlongArray longArr = env->NewLongArray(ptr->size);
    // env->SetLongArrayRegion(longArr, 0, ptr->size, reinterpret_cast<const jlong*>(ptr->tuple));
    env->SetLongArrayRegion(longArr, 0, ptr->size, buf);
    delete buf;
    return longArr;
}

/*
 * Class:     nV_VarPtr
 * Method:    _toString
 * Signature: (J)Ljava/lang/String;
 */
extern "C" JNIEXPORT jstring JNICALL Java_nV_VarPtr__1toString
(JNIEnv * env, jclass cls, jlong $ptr) {
    wostringstream oss;
    var x = VarPtr($ptr);
    x.print(oss);
    return env->NewString((const jchar*)oss.str().c_str(), oss.str().size());
}

/*
 * Class:     nV_VarPtr
 * Method:    _hashCode
 * Signature: (J)I
 */
extern "C" JNIEXPORT jint JNICALL Java_nV_VarPtr__1hashCode
(JNIEnv * env, jclass cls, jlong $ptr) {
    var x = VarPtr($ptr);
    return (jint)x.hash();
}

/*
 * Class:     nV_VarPtr
 * Method:    _clone
 * Signature: (J)J
 */
extern "C" JNIEXPORT jlong JNICALL Java_nV_VarPtr__1clone
(JNIEnv * env, jclass cls, jlong $ptr) {
    var x = VarPtr($ptr);
    return NewRef(x.clone());
}

/*
 * Class:     nV_VarPtr
 * Method:    _equals
 * Signature: (JJ)Z
 */
extern "C" JNIEXPORT jboolean JNICALL Java_nV_VarPtr__1equals
(JNIEnv * env, jclass cls, jlong x, jlong y) {
    return (jboolean)var(VarPtr(x)).equal(var(VarPtr(y)));
}

/*
 * Class:     nV_VarPtr
 * Method:    _compare
 * Signature: (JJ)I
 */
extern "C" JNIEXPORT jint JNICALL Java_nV_VarPtr__1compare
(JNIEnv * env, jclass cls, jlong x, jlong y) {
    return (jint)var(VarPtr(x)).compare(var(VarPtr(y)));
}

/*
 * Class:     nV_VarPtr
 * Method:    _primary
 * Signature: (J)I
 */
extern "C" JNIEXPORT jint JNICALL Java_nV_VarPtr__1primary
(JNIEnv * env, jclass cls, jlong $ptr) {
    var x = VarPtr($ptr);
    return x.primary();
}

/*
 * Class:     nV_VarPtr
 * Method:    _head
 * Signature: (J)J
 */
extern "C" JNIEXPORT jlong JNICALL Java_nV_VarPtr__1head
(JNIEnv * env, jclass cls, jlong $ptr) {
    var x = VarPtr($ptr);
    return NewRef(x.head());
}

/*
 * Class:     nV_VarPtr
 * Method:    _toDouble
 * Signature: (J)D
 */
JNIEXPORT jdouble JNICALL Java_nV_VarPtr__1toDouble
(JNIEnv * env, jclass cls, jlong $ptr) {
    var x = VarPtr($ptr);
    if (x.isObject())
        return (jdouble)toD(x.object());
    if (x.isKey(Key::Integer))
        return (jdouble)x.key().toUI();
    return 0.0;
}