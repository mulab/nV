#include "nVinPython.h"
#include "pythonObject.h"
#include "nVutils.h"

using namespace nV;

API Kernel kinpython;
Parser pinpython(kinpython);

bool PynV_Check(const PyObject* o){
	if (strcmp(o->ob_type->tp_name, "nV.custom"))
		return false;
	return true;
}

static PyObject *nVObject_New(var r);
PyObject *nVConvert(const sym x)
{
	PyObject *ret = 0;

	var r;
	try{
		r = kinpython.eval(var(x));
	}catch (std::exception& e) {
		// printf("eval\n");
		char ebuf[128];
		sprintf(ebuf, "nVError occurred while eval %s...",e.what()); 
		PyErr_SetString(PyExc_RuntimeError, ebuf);
		return 0;
    }
	if (r.isSymbol()){
		ret =  nVObject_New(r);
	}
	else {
		ret = nVConvert(r);
	}

	return ret;
}

PyObject *nVConvert(const Object& x)
{
	PyObject *ret = NULL;

	if (x.type == $.Integer){
		const mpz_t& mpz = x.cast<Integer>().mpz;

		//这里需要判断是转换到PyInt还是PyLong
		if (mpz_fits_slong_p(mpz)) {
			ret = PyInt_FromLong((long)mpz_get_si(mpz));
		}
		else {
			char* buf = mpz_get_str(0, 10, mpz);
		 
			ret = PyLong_FromString(buf,NULL,10);

			(*__gmp_free_func)(buf, strlen(buf) + 1);
		}
    }
	else if (x.type == $.Rational){
		//目前先转换为字符串
		const mpq_t& mpq = x.cast<Rational>().mpq;
		char* gbuf = mpq_get_str(0, 10, mpq);
		ret = PyString_FromString(gbuf);
		(*__gmp_free_func)(gbuf, strlen(gbuf) + 1);
	}
	else if (x.type == $.Real){
		const mpf_t& mpf = x.cast<Real>().mpf;
	    if (mpf_get_prec(mpf) == mpf_get_default_prec()) {
			ret = PyFloat_FromDouble((double)mpf_get_d(mpf));
		}
		//精度比较大，转换到字符串
		else {
			char *gbuf = mpf_get_str(0,0,10,0,mpf);
			//怎么销毁pys
			PyObject* pys = PyString_FromString(gbuf);
			ret = PyFloat_FromString(pys,0);
		}
	}
	else if (x.type == $.String){
		const string& buf = wcs2mbs(x.cast<String>().str);
		size_t len = buf.size();
		ret = PyString_FromStringAndSize(buf.c_str(), len);
	}
	else if (x.type == PythonObject::$PythonObject){
        ret = x.cast<PythonObject>().obj;
	}
	else{
		//ret = 0;
		ret = nVObject_New(var(&x));
	}

	return ret;
}

PyObject *nVConvert(const Key& x)
{
	PyObject *ret = NULL;
	int len;
	string buf;

	switch (x.kind()) {
	case  Key::Null:ret = 0; break;
	case Key::String: 
		buf = wcs2mbs(x.toS());
		len = buf.size();
		ret = PyString_FromStringAndSize(buf.c_str(), len); break;
	case Key::Integer:
		ret = PyInt_FromLong((long)x.toUI()); break;
	default: ret = 0;
	}

	return ret;
}


PyObject *nVConvert(const Tuple& x)
{
	PyObject *ret = 0;
	PyObject *item;
	if (x.tuple[0].symbol() == $.List){			//这种判断方式对吗？
		ret = PyList_New(x.size-1);
		for (uint i = 1; i < x.size; ++i){
			if (item = nVConvert(x.tuple[i])){
				PyList_SetItem(ret, i-1, item);
			}
			else {
				PyErr_SetString(PyExc_TypeError, "failed to convert item");
			}
		}
	}

	return ret;
}

PyObject *nVConvert(const var& r)
{
	PyObject *ret = NULL;

	switch (r.primary()) {
	case Primary::Object:
		ret = nVConvert(r.object()); break;
	case Primary::Symbol:
		ret = nVConvert(r.symbol()); break;
	case Primary::Key:
		ret = nVConvert(r.key()); break;
	case Primary::Tuple:
		ret = nVConvert(r.tuple()); break;
	default: ret = nVObject_New(r);
	}

	return ret;
}

	

static PyObject *nVCall(PyObject *op, PyObject *args)
{
	PyObject *ret = NULL;

	uint nargs = PyTuple_Size(args);
	Tuple *t = tuple(nargs+1);


	t->tuple[0] = ((nVObject*)op)->r;

	for (uint i = 1; i <= nargs; i++) {
		PyObject *arg = PyTuple_GetItem(args, i-1);
		if (arg == NULL) {
			PyErr_Format(PyExc_TypeError,
				"failed to convert #%d argument", i);
			return NULL;
		}
		t->tuple[i] = py_convert(arg);
	}

	var r; 

	try{
		r = kinpython.eval(var(t));
	}catch (std::exception& e) {
		// printf("eval\n");
		char ebuf[128];
		sprintf(ebuf, "nVError occurred while eval %s...",e.what()); 
		PyErr_SetString(PyExc_RuntimeError, ebuf);
		return 0;
    }

	ret = nVConvert(r);

	if (!ret) {
		PyErr_Format(PyExc_TypeError,"failed to convert zhe result");
	}

	return ret;
}

static PyObject *nVObject_New(var r)					//这个函数可能需要重写
{
	PyObject* obj = _PyObject_New(&nVObject_Type);
	obj = PyObject_Init(obj, &nVObject_Type);
	((nVObject*)obj)->r.ptr = 0;

	if (obj) {
		((nVObject*)obj)->r = r;
	}
	else {
		PyErr_Format(PyExc_TypeError,"can not get a new PyObject");
	}
	return (PyObject*) obj;
}

void nVObject_dealloc(nVObject *self)				//这个函数可能需要重写
{
	((nVObject*)self)->r.~var();
	self->ob_type->tp_free((PyObject *)self);
}

static PyObject *nVObject_getattr(PyObject *obj, PyObject *attr)
{
	PyObject *ret = NULL;

	//此处最好是PythonConvert函数，用于将PyObject转换为nV里面的数据结构
	char *s;
	Py_ssize_t len;

	//PyString_AsStringAndSize返回值为0是正常的？怎么与标准说明不一样
	if (0 != PyString_AsStringAndSize(attr, &s, &len) || 0 == s){
		PyErr_SetString(PyExc_RuntimeError, "can not get string");
		return 0;
	}

	if (((nVObject*)obj)->r.ptr == NULL){
		PyErr_SetString(PyExc_RuntimeError, "lost reference");
		free(s);
		return 0;
	}
		
	//这个地方不好用symbol函数，getattr不能添加新的属性
	var r = (((nVObject*)obj)->r).symbol()->symbol(wstr(mbs2wcs(s).c_str()));

	if (r.ptr == NULL){
		PyErr_SetString(PyExc_RuntimeError,
				"lost s reference");
		free(s);
		return 0;
	}
	//var r1 = kinpython.eval(r);

	ret = nVConvert(r);

	if (ret == 0){
		free(s);
		PyErr_Format(PyExc_TypeError,
				"can not get the attr");
	}

	free(s);
	return ret;
}

static int nVObject_setattr(PyObject *obj, PyObject *attr, PyObject *value)
{
	PyObject *ret = NULL;

	char *s;
	Py_ssize_t len;
	//PyString_AsStringAndSize返回值为0是正常的？怎么与标准说明不一样
	if (0 != PyString_AsStringAndSize(attr, &s, &len) || 0 == s) {
		PyErr_SetString(PyExc_RuntimeError, "can not get string");
		return -1;
	}
	
	if (((nVObject*)obj)->r.ptr == NULL){
		PyErr_SetString(PyExc_RuntimeError, "lost reference");
		free(s);
		return -1;
	}

	var r1 = ((nVObject*)obj)->r.symbol()->symbol(wstr(mbs2wcs(s).c_str()));
	var r2 = py_convert(value);

	kinpython.assign(r1,r2);

	free(s);
	return 0;
}

static PyObject *nVObject_str(PyObject *obj)
{
	PyObject *ret = NULL;

	var r;
	try{
		r = kinpython.value(tuple(SYS(ToString), kinpython.value((((nVObject*)obj)->r))));
	}catch (std::exception& e) {
		// printf("eval\n");
		char ebuf[128];
		sprintf(ebuf, "nVError occurred while value %s...",e.what()); 
		PyErr_SetString(PyExc_RuntimeError, ebuf);
		return 0;
    }

	if (r.ptr != 0){
		ret = PyString_FromFormat("%s", (wcs2mbs(r.cast<String>().str)).c_str());
	}
	else {
		ret = 0;
		PyErr_SetString(PyExc_RuntimeError, "can not get the value");
	}
		
	return ret;
}

static PyObject *nVObject_call(PyObject *obj, PyObject *args)
{
	return nVCall(obj, args);
}

static PyObject *nVObject_iternext(nVObject *obj)
{
	PyObject *ret = NULL;
	return ret;
}

static Py_ssize_t nVObject_length(nVObject *obj)
{
	Py_ssize_t len = 0;
	return len;
}

static PyObject *nVObject_subscript(PyObject *obj, PyObject *key)
{
	return nVObject_getattr(obj, key);
}

static Py_ssize_t nVObject_ass_subscript(PyObject *obj,
				PyObject *key, PyObject *value)
{
	return nVObject_setattr(obj, key, value);
}

static PyMappingMethods nVObject_as_mapping = {
	(inquiry)nVObject_length,	/*mp_length*/
	(binaryfunc)nVObject_subscript,/*mp_subscript*/
	(objobjargproc)nVObject_ass_subscript,/*mp_ass_subscript*/
};



PyTypeObject nVObject_Type = {
	PyObject_HEAD_INIT(NULL)
	0,								/*ob_size*/
	"nV.custom",					/*tp_name*/
	sizeof(nVObject),				/*tp_basicsize*/
	0,								/*tp_itemsize*/
	(destructor)nVObject_dealloc,	/*tp_dealloc*/
	0,								/*tp_print*/
	0,								/*tp_getattr*/
	0,								/*tp_setattr*/
	0,								/*tp_compare*/
	nVObject_str,					/*tp_repr*/
	0,								/*tp_as_number*/
	0,								/*tp_as_sequence*/
	&nVObject_as_mapping,			/*tp_as_mapping*/
	0,								/*tp_hash*/
        (ternaryfunc)nVObject_call,          /*tp_call*/
        nVObject_str,				/*tp_str*/
        nVObject_getattr,			/*tp_getattro*/
		nVObject_setattr,			/*tp_setattro*/
        0,							 /*tp_as_buffer*/
        Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
        0,           				/*tp_doc*/
        0,							 /*tp_traverse*/
        0,								/*tp_clear*/
        0,								/*tp_richcompare*/
        0,								/*tp_weaklistoffset*/
        PyObject_SelfIter,				/*tp_iter*/
        (iternextfunc)nVObject_iternext, /*tp_iternext*/
        0,       						/*tp_methods*/
        0,       						/*tp_members*/
        0,								/*tp_getset*/
        0,								/*tp_base*/
        0,								/*tp_dict*/
        0,								/*tp_descr_get*/
        0,								/*tp_descr_set*/
        0,								/*tp_dictoffset*/
        0,								/*tp_init*/
        PyType_GenericAlloc,			/*tp_alloc*/
        PyType_GenericNew,				/*tp_new*/
      	_PyObject_Del,       			/*tp_free*/
        0,								/*tp_is_gc*/
};


PyObject *nV_run(PyObject *args, int in)
{
	PyObject *ret = 0;
	char *buf = NULL;
	char *s;
	int len;
	var r;

	if (!PyArg_ParseTuple(args, "s#", &s, &len)) {
		PyErr_SetString(PyExc_RuntimeError,"the argument is wrong");
		return NULL;
	}

	 wistringstream stream(mbs2wcs(s));

	 try {
		pinpython.start(stream);
		pinpython.gen(pinpython.parse());
	} catch (std::exception& e) {
        if (!strcmp(e.what(), "more")) {
		//	printf("more\n");
            PyErr_SetString(PyExc_RuntimeError, "nVexception: can not handle more than one line!" );    
        } else if (!strcmp(e.what(), "error")) {
		//	printf("error\n");
			PyErr_SetString(PyExc_RuntimeError, "nVexception: parse error happened!" );
            wcout << std::setw(pinpython.parsing->column + in) << _W("^\n");
		}
		stream.clear();
		return 0;
     }
        
	 try {
		r = kinpython.eval(pinpython.code());
	 } catch (std::exception& e) {
		// printf("eval\n");
		char ebuf[128];
		sprintf(ebuf, "nVError occurred while eval %s...",e.what()); 
		PyErr_SetString(PyExc_RuntimeError, ebuf);
		return 0;
      }

	 ret = nVConvert(r);

	return ret;
}

PyObject *nV_execute(PyObject *self, PyObject *args)
{
	return nV_run(args, 15);
}

PyObject *nV_eval(PyObject *self, PyObject *args)
{
	return nV_run(args, 12);
}

PyObject *nV_root(PyObject *self, PyObject *args)
{
	PyObject *ret = NULL;

	if (root == NULL) {
		PyErr_SetString(PyExc_RuntimeError, "lost root reference");
		return NULL;
	}

	ret = nVObject_New(var(root));
	
	return ret;
}

PyObject *nV_system(PyObject *self, PyObject *args)
{
	PyObject *ret = NULL;


	var r = root->symbol(wstr(_W("System")));

	if (r.ptr == NULL){
		PyErr_SetString(PyExc_RuntimeError,"can not get System namespace");
		return NULL;
	}
	ret = nVObject_New(r);

	return ret;
}

PyObject *nV_global(PyObject *self, PyObject *args)
{
	PyObject *ret = NULL;


	var r = root->symbol(wstr(_W("Global")));

	if (r.ptr == NULL){
		PyErr_SetString(PyExc_RuntimeError,"can not get Global namespace");
		return NULL;
	}
	ret = nVObject_New(r);

	return ret;
}

PyObject *nV_require(PyObject *self, PyObject *args)
{
	PyObject* ret = 0;
	char *s;
	int len;
	if (!PyArg_ParseTuple(args, "s#", &s, &len)){
		PyErr_SetString(PyExc_RuntimeError,"the argument is wrong");
		return NULL;
	}

	var r = root->symbol(wstr(mbs2wcs(s)));

	if (r.ptr == NULL){
		PyErr_SetString(PyExc_RuntimeError,"lost system reference");
		return NULL;
	}
	ret = nVObject_New(r);

	return ret;
}



static PyMethodDef nV_methods[] = {
	{"execute",	nV_execute,	METH_VARARGS,		NULL},
	{"eval",	nV_eval,	METH_VARARGS,		NULL},
	{"root",	nV_root,	METH_NOARGS,		NULL},
	{"system",	nV_system,	METH_NOARGS,		NULL},
	{"globals",	nV_global,	METH_NOARGS,		NULL},
	{"require", nV_require,	METH_VARARGS,		NULL},
	{NULL,		NULL}
};


CAPI void initnV(void)
{
	PyObject *m;
	m = Py_InitModule("nV", nV_methods);
}

