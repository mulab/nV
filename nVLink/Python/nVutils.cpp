#include "nVinPython.h"
#include "nVutils.h"
#include "PythonObject.h"

static var py_list_convert(PyObject *o)
{
	var r = tuple(((PyListObject*)o)->ob_size+1);
	r.tuple()[0] = $.List;

	for (int i = 1; i <= ((PyListObject*)o)->ob_size; ++i){
		r.tuple()[i] = py_convert(PyList_GetItem(o, i-1));
	}
	return r;
}

static var py_tuple_convert (PyObject *o)
{
	//暂时认为list和tuple相同
	var r = py_list_convert(o);
	return r;
}

var py_convert( PyObject *o)
{
	var r;

	if (o == Py_None) {
		r = $.Null;
	} else if (o == Py_True) {
		r = $.True;
	} else if (o == Py_False) {
		r = $.False;
	} else if (PyString_Check(o)) {
		char *s;
		int len;
		//PyString_AsStringAndSize返回值为0是正常的？怎么与标准说明不一样
		if (0 != PyString_AsStringAndSize(o, &s, &len) || s == NULL){
			return 0;
		}
		//这种赋值方式完全吗？
		r = new String(mbs2wcs(s));

		free(s);
		
	} else if (PyInt_Check(o)) {
		r = new Integer(PyInt_AsLong(o));
	}
	else if (PyLong_Check(o)) {
		//可以考虑用_PyLong_AsByteArray，可能比较快
		PyObject* pys = _PyLong_Format(o,10,0,0);
		char* s = PyString_AsString(pys);
		r = new Integer(s, 10);
	}
	else if (PyFloat_Check(o)) {
		char s[120];
		PyFloat_AsString(s, (PyFloatObject*)o);
		r = new Real(s,10,0);		//需要修改
	}
	else if ( PyList_Check(o) ){
		r = py_list_convert(o);
	}
	else if ( PyTuple_Check(o) ){
		r = py_tuple_convert(o);
	}
	else if (PynV_Check(o)){
		r = ((nVObject*)o)->r;
	}
	else if (o){
		r = new PythonObject(o);
	}

	return r;
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
    return key((wcs)0);
}



static bool set_python_attr(Kernel& k, PyObject* obj, const Key* att, const var& v);
static bool set_python_attr(Kernel& k, PyObject* obj, const Tuple& attr, const var& v);
static bool set_python_attr(Kernel& k, PyObject* obj, const var& attr, const var& value);

static bool set_python_attr(Kernel& k, PyObject* obj, const Tuple& attr, const var& v){
	if (attr.size == 1){
		Key *h = toKey(attr[0]);
		if (h && h->kind() == Key::String){
			return set_python_attr(k, obj, h, v);
		}
		else{
			throw std::logic_error("set_python_attr error: not a key\n");
			return false;
		}
	}

	if (attr.size != 2){
		throw std::logic_error("set_python_attr error: attr.size is not 2\n");
		return false;
	}
	if (attr[0].isTuple()){
		throw std::logic_error("set_python_attr error: attr[0] is tuple\n");
		return false;
	}

	Key *h = toKey(attr[0]);
	if(h && h->kind() == Key::String){
		PyObject *attr_name = PyString_FromString(wcs2mbs(h->toS()).c_str());
		if (!attr_name){
			PyErr_Clear();
			throw std::logic_error("get_python_attr error: PyString_FromString\n");
		}

		PyObject *value = 0;
		if (PyModule_Check(obj)) {
			value = PyObject_GetAttr(obj, attr_name);
		}
		else if (PyDict_Check(obj)){
			value = PyObject_GetItem(obj, attr_name);
		}
	
		if (!value){
			PyErr_Clear();
			throw std::logic_error("set_python_attr error: Can not get such attribute\n");
			return false;
		}

		if (PyModule_Check(value) || PyDict_Check(value)){
			Tuple *t = attr.clone();
			t->tuple[0] = new PythonObject(value);
			return python_assign(k, *t, v);
		}
		else{
			throw std::logic_error("set_python_attr error: not a namespace\n");
			return false;
		}

		//******************
		Py_DECREF(attr_name);
	}
	else {
		throw std::logic_error("python_object error: attr is not a string\n");
		return false;
	}
}

static bool set_python_attr(Kernel& k, PyObject* obj, const Key* att, const var& v){

	PyObject *attr = PyString_FromString(wcs2mbs(att->toS()).c_str());
	PyObject *value = nVConvert(v);

	bool ret = true;

	if (PyModule_Check(obj)){
		if (-1 == PyObject_SetAttr(obj, attr, value)){
			PyErr_Clear();
			throw std::logic_error("set_python_attr error: PyObject_SetAttr\n");
			return false;
		}
	}
	else if (-1 == PyObject_SetItem(obj, attr, value)){
		PyErr_Clear();
		throw std::logic_error("set_python_attr error: PyObject_SetItem\n");
		return false;
	}
	else ret = false;

	//******************
	Py_DECREF(attr);
	Py_DECREF(value);

	return ret;
}
static bool set_python_attr(Kernel& k, PyObject* obj, const var& attr, const var& value){
	if (attr.isTuple()){
		return set_python_attr(k, obj, attr.tuple(), value);
	}
	else {
		Key *h = toKey(attr);
		if(h && h->kind() == Key::String){
			return set_python_attr(k, obj, h, value);
		}
		else {
			throw std::logic_error("python_object error: can not get a key\n");
			return false;
		}
	}
}


static var python_call(Kernel& k, PyObject *fun, Tuple& r);
static var get_python_attr(Kernel& k, PyObject* p, const var &r);
static var get_python_attr(Kernel& k, PyObject* p, const Tuple& x);
static var get_python_attr(Kernel& k, PyObject* p, const Key *h);

static var python_call(Kernel& k, PyObject *fun, Tuple& r){
	var r_arg, ret;

	int nargs = r.size-1;
	
	PyObject *args = PyTuple_New(nargs);

	for (int i = 1; i <= nargs; ++i){
		r_arg = k.value(r[i]);
		PyObject *arg = nVConvert(r_arg);
		if (!arg) {
			Py_DECREF(args);
			return $.Fail;
		}
		PyTuple_SetItem(args, i-1, arg);
	}
	PyObject* value = PyObject_CallObject(fun, args);

	if (value) {
		ret = py_convert(value);
		Py_DECREF(value);
	} else {
		throw std::logic_error("can not get the call result\n");
		ret = $.Fail;
	}
	return ret;

}


static var get_python_attr(Kernel& k, PyObject* p, const Tuple& x){
	var r = get_python_attr(k, p, x[0]);
	Tuple* t = x.clone();
	t->tuple[0] = r;

	var result;
	python_object(k, result, *t);

	return result;
}
static var get_python_attr(Kernel& k, PyObject* p, const Key *h){
	PyObject *attr_name = PyString_FromString(wcs2mbs(h->toS()).c_str());
	if (!attr_name){
		PyErr_Clear();
		throw std::logic_error("get_python_attr error: PyString_FromString\n");
		return $.Fail;
	}

	PyObject *value = 0;
	if (PyModule_Check(p)) {
		value = PyObject_GetAttr(p, attr_name);
	}
	else if (PyDict_Check(p)){
		value = PyObject_GetItem(p, attr_name);
	}
	
	/*char* s;
	int len;
	PyErr_Clear();
		if (0 != PyString_AsStringAndSize(attr_name, &s, &len) || 0 == s) {
				PyErr_SetString(PyExc_RuntimeError, "can not get string");
				return $.Fail;
		}
		printf("%s\n",mbs2wcs(s).c_str());*/

	if (!value){
		throw std::logic_error("Can not get such attribute\n");
		return $.Fail;
	}
	return py_convert(value);
}

static var get_python_attr(Kernel& k, PyObject* p, const var &r){
	if (r.isTuple()){
		return get_python_attr(k, p, r.tuple());
	}
	else {
		Key *h = toKey(r);
		if(h && h->kind() == Key::String){
			return get_python_attr(k, p, h);
		}
		else {
			throw std::logic_error("python_object error: can not get a key\n");
		}
	}
}



API var py_run(string &command, int eval)
{
	const char *s;
	char *buffer = NULL;
	PyObject *m, *d, *o;
	var ret;
	int len;

	s = command.c_str();
	if (!s)
		return $.Fail;

	if (!eval) {
		len = strlen(s)+1;
		buffer = (char *) malloc(len+1);
		if (!buffer) {
			throw std::logic_error( "Failed allocating buffer for execution\n");
			return $.Fail;
		}
		strcpy(buffer, s);
		buffer[len-1] = '\n';
		buffer[len] = '\0';
		s = buffer;
	}

	m = PyImport_AddModule("__main__");
    if (!m) {
		free(buffer);
		throw std::logic_error("Can't get __main__ module\n");
		return $.Fail;
	}
    d = PyModule_GetDict(m);

    o = PyRun_StringFlags(s, eval ? Py_eval_input : Py_single_input,
			      d, d, NULL);

	free(buffer);

    if (!o) {
          PyErr_Print();
		  return $.Fail;
    }

//	ret = py_convert(o);

	Py_DECREF(o);

    if (Py_FlushLine())
		PyErr_Clear();

	return $.True;
}

API bool python_assign(Kernel& k, const Tuple& x, const var& y){
	if (x[0].isKey()){
		if (x.size != 2){
			throw std::logic_error("python_assign error: s.size is not 2\n");
			return false;
		}
		var r0 = k.value(x[0]);

		if (r0.isObject() && (r0.object().type == PythonObject::$PythonObject)){
			return set_python_attr(k,r0.cast<PythonObject>().obj,x[1],y);
		}
		else {
			throw std::logic_error("python_assign error: x0 is not a python object\n");
			return false;
		}
	}
}

API void python_object(Kernel& k, var& r, Tuple& x){
	if (x[0].isObject() && x[0].object().type == PythonObject::$PythonObject){
		if (PyCallable_Check(x[0].cast<PythonObject>().obj)){
			r = python_call(k, x[0].cast<PythonObject>().obj, x);
		}
		else if (PyModule_Check(x[0].cast<PythonObject>().obj) || PyDict_Check(x[0].cast<PythonObject>().obj)){
			if(x.size != 2){
				r = $.Fail;
				throw std::logic_error("python_object error: two x.size != 2\n");
			}
			r = get_python_attr(k, x[0].cast<PythonObject>().obj, x[1]);
		}
	}
	else{
		r = $.Fail;
		throw std::logic_error("python_object error: is not a python object\n");
	}
}
