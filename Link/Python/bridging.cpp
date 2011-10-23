#include "bridging.h"

extern Kernel kinpython;
//var py_convert( PyObject *o);
//PyObject *nVConvert(const var& r);
PyObject *nVObject_New(var r);

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

	printf("enter in py_convert\n");
	if (o == Py_None) {
		r = $.Null;
	} else if (o == Py_True) {
		r = $.True;
	} else if (o == Py_False) {
		r = $.False;
	} else if (PyString_Check(o)) {
		char *s;
		int len;
		PyString_AsStringAndSize(o, &s, &len);
		//这种赋值方式完全吗？
		r = new String(mbs2wcs(s));
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

	printf("leave py_convert\n");
	return r;
}


PyObject *nVConvert(const sym x)
{
	PyObject *ret = NULL;

	printf("enter in nVConvert symbol\n");
	var r = kinpython.value(var(x));
		
	//这里需要重新考虑，如果符号关联的是一个字面值，那我们应该返回这个字面值，如果符号关联的是一个函数，则应该返回这个函数
	ret =  nVObject_New(r);
	printf("leave nVConvert symbol\n");
	return ret;
}

PyObject *nVConvert(const Object& x)
{
	PyObject *ret = NULL;

	printf("enter in nVConvert object\n");
	if (x.type == $.Integer){
		const mpz_t& mpz = x.cast<Integer>().mpz;

		//这里需要判断是转换到PyInt还是PyLong

/*		if (mpz_cmp_si(mpz, 0x7fffffff)>0  || mpz_cmp_si(mpz, -0x7fffffff) <0) {
		char* buf = mpz_get_str(0, 10, mpz);
		 
		ret = PyLong_FromString(buf,NULL,10);

		(*__gmp_free_func)(buf, strlen(buf) + 1);

		}
		else {
			ret = PyLong_FromLong((long)mpz_get_si(mpz));
		}*/
		if (mpz_fits_slong_p(mpz)) {
			printf("the integer: %d is converted", (long)mpz_get_si(mpz));
			ret = PyLong_FromLong((long)mpz_get_si(mpz));
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
	else ret = 0;
		
	printf("leave nVConvert object\n");
	return ret;
}

PyObject *nVConvert(const Key& x)
{
	PyObject *ret = NULL;
	int len;
	string buf;

	printf("enter in nVConvert Key\n");
	switch (x.kind()) {
	case  Key::Null:ret = 0; break;
	case Key::String: 
		buf = wcs2mbs(x.toS());
		len = buf.size();
		ret = PyString_FromStringAndSize(buf.c_str(), len); break;
	case Key::Integer:
		ret = PyInt_FromLong((long)x.toUI()); break;
	default: ret = NULL;
	}
	printf("leave nVConvert Key\n");

	return ret;
}


PyObject *nVConvert(const Tuple& x)
{
	PyObject *ret = NULL;

	printf("enter in nVConvert tuple\n");
	if (x.tuple[0].symbol() == $.List){			//这种判断方式对吗？
		ret = PyList_New(x.size-1);
		for (uint i = 1; i < x.size; ++i){
			PyList_SetItem(ret, i-1, nVConvert((x.tuple[i])));
		}
	}
	printf("leave nVConvert tuple\n");

	return ret;
}


PyObject *nVConvert(const var& r)
{
	PyObject *ret = NULL;

	printf("enter in nVConvert r\n");
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

	printf("leave in nVConvert r\n");
	return ret;
}