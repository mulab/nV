#include "JavaObject.h"

namespace nV {
sym JavaObject::$Java, JavaObject::$JavaObject;
bool JavaObject::Inited = false;
void JavaObject::Init(int argc, char* argv[]) {
    if (Inited)
        return;
    $Java = SYM(root, Java);
    $JavaObject = SYM($Java, Object);
    JavaVMInitArgs vm_args;
    JavaVMOption* options = new JavaVMOption[argc];
    vm_args.version = JNI_VERSION_1_6;
    vm_args.nOptions = argc;
    for (int i = 0; i < argc; ++i)
        options[i].optionString = argv[i];
    // options[0].optionString = "-Djava.compiler=NONE"; /* disable JIT */
    // options[1].optionString = "-Djava.class.path=" USER_CLASSPATH; /* user classes */
    // options[2].optionString = "-Djava.library.path=" USER_LIBPATH; /* set native library path */
    // options[3].optionString = "-verbose:jni"; /* print jni-related messages */
    vm_args.options = options;
    JNI_CreateJavaVM(&VM, (void **)&Env, &vm_args);
    delete options;
    Class = (jclass)Env->NewGlobalRef(Env->FindClass("Ljava/lang/Class;"));
    /*if (!cls) {
    	wcerr << _W("Java class mU.VarPtr not found!") << endl;
    	return 0;
    }
    if (!mid) {
    	wcerr << _W("Java class mU.VarPtr hasn't a constructor from long!") << endl;
    	return 0;
    }
    */
    Inited = true;
}
void JavaObject::Close() {
    Env->DeleteGlobalRef(Class);
    VM->DestroyJavaVM();
    Inited = false;
}
JNIEnv* JavaObject::Env;
JavaVM* JavaObject::VM;
jclass	JavaObject::Class;
JavaObject* JavaObject::clone() const {
    jobject r = obj;
    // jclass cls = Env->GetObjectClass(obj);
    jmethodID mid = Env->GetMethodID(cls, "clone", "()Ljava/lang/Object;");
    // Env->DeleteLocalRef(cls);
    return new JavaObject(Env->CallObjectMethod(obj, mid));
}
int JavaObject::compare(const Object& x) const {
    return Object::compare(x);
}
bool JavaObject::equal(const Object& $other) const {
    // jclass cls = Env->GetObjectClass(obj);
    jmethodID mid = Env->GetMethodID(cls, "equals", "(Ljava/lang/Object;)Z");
    // Env->DeleteLocalRef(cls);
    return Env->CallBooleanMethod(obj, mid, $other.cast<JavaObject>().obj) != 0;
    // return Env->IsSameObject(obj, $other.cast<JavaObject>().obj) != 0;
}
size_t JavaObject::hash() const {
    // jclass cls = Env->GetObjectClass(obj);
    jmethodID mid = Env->GetMethodID(cls, "hashCode", "()I");
    // Env->DeleteLocalRef(cls);
    return static_cast<size_t>(Env->CallIntMethod(obj, mid));
}
void JavaObject::print(wostream& o) const {
    if (!obj) {
        o << _W("null");
        return;
    }
    // jclass cls = Env->GetObjectClass(obj);
    jmethodID mid = Env->GetMethodID(cls, "toString", "()Ljava/lang/String;");
    // Env->DeleteLocalRef(cls);
    jstring str = reinterpret_cast<jstring>(Env->CallObjectMethod(obj, mid));
    const jchar* buf = Env->GetStringChars(str, 0);
    o << (wcs)buf;
    Env->ReleaseStringChars(str, buf);
}
}