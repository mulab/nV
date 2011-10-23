#pragma once
#include "Common.h"
#include <vector>
#include <iterator>
#include <algorithm>

namespace nV {
// 包装std::vector
// TODO: std::tuple少一次解引用，是否更好？
template <class T>
class Array : public Object {
public:
	std::vector<T> vec;
	virtual Array<T>* clone() const {
		return new Array<T>(vec);
	}
	virtual int compare(const Object& x) const {
		const std::vector<T>& xvec = x.cast<Array<T> >().vec;
		if (vec.size() > xvec.size())
			return 1;
		else if (vec.size() < xvec.size())
			return -1;
		for (size_t i = 0; i < vec.size(); ++i) {
			if (vec[i] > xvec[i])
				return 1;
			else if (vec[i] < xvec[i])
				return -1;
		}
		return 0;
	}
	virtual size_t hash() const {
		size_t r = 2166136261U;
		typename std::vector<T>::const_iterator iter = vec.begin();//See C++ FAQ 35.18 for why typename is needed
		while (iter < vec.end()) {
			r = 16777619U * r ^ std::hash<T>()(*iter);
			++iter;
		}
		return r;
	}
	virtual void print(wostream& o) const {
	  //type->print(o);
		o << _W("<");
		if (!vec.empty()) {
			std::copy(vec.begin(), vec.end() - 1, std::ostream_iterator<T, wchar_t>(o, _W(",")));
			o << vec.back();
		}
		o << _W('>');
	}
	explicit Array() : Object($.Array) {}
	explicit Array(sym $type) : Object($type) {}
	explicit Array(const std::vector<T>& x) : Object($.Array), vec(x) {}
	explicit Array(const std::vector<T>& x, sym $type) : Object($type), vec(x) {}
};
template <class T>
inline Array<T>& A(const var& x) {
    return x.cast<Array<T> >();
}
template <class T>
inline Array<T>& A(const Object& x) {
    return x.cast<Array<T> >();
}
// typedef Array<sint> IntArray;
// #define IntA A<sint>
}
