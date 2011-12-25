#include "StdAfx.h"
#include <nV/Polynomial.h>
#include <nV/Number.h>
#include <algorithm>
/*#include <functional>
  using namespace std::placeholders;
*/

namespace nV {
var Plus(Kernel& k, const Tuple& x) {
    if (x.size == 1)//x=(Plus)
        return new Integer(0L);
    if (x.size == 2)//x=(Plus x[1])
        return x[1];
    std::vector<var> r;
    //Collect constant term,x is sorted(??)
    uint pos = 1;
    if (isNumber(x[1])) {
        var c = x[1].clone();
        for (pos = 2; pos < x.size && isNumber(x[pos]); ++pos)
            c = Number::Plus(c.object(), x[pos].object());
        if(c.isObject($.Rational))
            mpq_canonicalize(c.cast<Rational>().mpq);
        if (cmpD(c.object(), 0.0))
            r.push_back(c);
    }
    //Put terms in UMap.(If x is sorted,why a hash table)
    //typedef std::unordered_map<var, std::vector<var> > UMap;
    typedef std::map<var, std::vector<var> > UMap;
    UMap map;
    for (; pos < x.size; ++pos) {
        var b = x[pos], e;
        if (b == $.Infinity)//What is $.Infinity
            return $.Infinity;
        if (b.isTuple($.Times)) {
            const Tuple& t = b.tuple();
            if (isNumber(t[1])) {
                e = t[1];
                if (t.size == 3) {
                    b = t[2];//3*x==>e=3 b=x
                    if (b == $.Infinity)//What is this
                        return x[pos];
                } else {
                    Tuple* c = tuple(t.size - 1);
                    c->tuple[0] = $.Times;
                    for(uint i = 2; i < t.size; ++i)
                        c->tuple[i - 1] = t[i];
                    b = c;//3*x^2*y^2==>e=3,b=x^2*y^2
                }
            } else
                e = new Integer(1L);//x*y==>e=1,b=x*y
        } else
            e = new Integer(1L);//x^2=>e=1,b=x^2
        map[b].push_back(e);
    }
    //collect similar terms
    UMap::const_iterator iter = map.begin();
    while (iter != map.end()) {
        var b = iter->first;
        const std::vector<var>& v = iter->second;
        var e = v[0];
        for (uint i = 1; i < v.size(); ++i)
            e = Number::Plus(e.object(), v[i].object());
        ++iter;
        /*std::for_each(++v.begin(), v.end(), std::bind(Number::add, e, e.object(),
          std::bind(&var::object, _1)));
        */
        double ed = toD(e.object());//what if ed=1+10^-100?
        if (ed != 0.0) {//If ed==0,we can drop this term
            if (ed != 1.0) {
                if (b.isTuple($.Times)) {
                    Tuple* c = tuple(b.tuple().size + 1);
                    c->tuple[0] = $.Times;
                    c->tuple[1] = e;
                    for (uint i = 1; i < b.tuple().size; ++i)
                        c->tuple[i + 1] = b.tuple()[i];
                    b = c;
                } else
                    b = tuple($.Times, e, b);
            }
            r.push_back(b);
        }
    }
    if (r.size() == 0)
        return new Integer(0L);
    if (r.size() == 1)
        return r[0];
    return nV::list(r.size(), r.begin(), $.Plus);
}
var Plus(Kernel& k, const var& x, const var& y) {//What is this for
    if (x == 0)
        return y;
    if (y == 0)
        return x;
    if (x.isObject() && y.isObject())
        return Number::Plus(x.object(), y.object());
    var r = tuple($.Plus, x, y);
    r = k.flatten($.Plus, r.tuple());
	std::sort(r.tuple().tuple + 1, r.tuple().tuple + r.tuple().size, lessPrimary);
    return Plus(k, r.tuple());
}
}

using namespace nV;

CAPI void CVALUE(System, Plus)(Kernel& k, var& r, Tuple& x) {
	if (!x[0].isSymbol())
		return;
	r = Plus(k, x);
}
