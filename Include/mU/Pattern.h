#pragma once
#include "Kernel.h"

namespace mU {
//////////////////////////////////////
struct pos_t
{
	pos_t *prev;
	var *index;
	var *end;
	pos_t(pos_t *x = 0) : prev(x) {}
	pos_t(var &x, pos_t *y = 0) : prev(y),index(&x),end((&x) + 1) {}
	pos_t(Var x, pos_t *y = 0) : prev(y)
	{
		index = &CVec(x).front();
		end = &CVec(x).back() + 1;
	}
	operator bool() { return index < end; }
	var& operator *() { return *index; }
	pos_t& operator ++() { ++index; return *this; }
	pos_t& operator --() { --index; return *this; }
};

struct pat_t : obj_t
{
	var m_next;
	virtual bool match(map_t&,pos_t&) = 0;
	bool next(map_t &m, pos_t &x)
	{
		return m_next.get() ?
					dynamic_cast<pat_t*>(m_next.get())->match(m,x) : !x;
	}
};
inline pat_t *PAT(Var x) { return dynamic_cast<pat_t*>(x); }
API var Pat(pos_t&);
inline var Pat(Var x) { pos_t p((var&)x); return Pat(p); }
API var Pat(Var,pos_t&);
inline var Pat(Var x, Var y) { pos_t p(y); return Pat(x,p); }
API var PatEx(Var, Var);
inline bool MatchQ(map_t &m, Var x, Var y)
{
	pos_t p((var&)y);
	return PAT(x)->match(m,p);
}
API bool MemberQ(Var,Var);
API var Dispatch(Var);
API var Replace(Var,Var,Var);
API var Replace(Var,Var);
API var ReplaceAll(Var,Var,Var);
API var ReplaceAll(Var,Var);

namespace Pattern {

inline bool Match(map_t &m, Var x, pos_t &y) { return PAT(x)->match(m,y); }
inline var& Next(Var x) { return PAT(x)->m_next; }

class fix : public pat_t
{
	var m_value;
public:
	fix(pos_t &x) : m_value(*x) {}
	bool match(map_t &m, pos_t &x)
	{
		if(!x || !Equal(m_value, *x))
			return false;
		++x;
		if(next(m,x)) return true;
		--x;
		return false;
	}
};

class name : public pat_t
{
	var m_name;
public:
	name(Var $name) : m_name($name) {}
	bool match(map_t &m, pos_t &x)
	{
		var &r = m[m_name];
		if(r)
		{
			if(!Equal(r, m[0]))
				return false;
			return next(m,x);
		}
		else
			r = m[0];
		if(next(m,x)) return true;
		r = 0;
		return false;
	}
};

class blank : public pat_t
{
public:
	blank() {}
	bool match(map_t &m, pos_t &x)
	{
		if(!x) return false;
		m[0] = *x;
		++x;
		if(next(m,x)) return true;
		--x;
		return false;
	}
};

class blank_head : public pat_t
{
	var m_head;
public:
	blank_head(Var x) : m_head(x) {}
	bool match(map_t &m, pos_t &x)
	{
		if(!x) return false;
		if(!Equal(m_head,Tag(*x)))
			return false;
		m[0] = *x;
		++x;
		if(next(m,x)) return true;
		--x;
		return false;
	}
};

class blank_flat : public pat_t
{
	var m_head;
	size_t m_size;
public:
	blank_flat(Var x, pos_t &y) : m_head(x)
	{
		m_size = y.end - y.index - 1;
	}
	bool match(map_t &m, pos_t &x)
	{
		if(!x) return false;
		var t = Ex(m_head,Vec());
		var& r = m[0];
		var *a = x.index;
		size_t n = x.end - a - m_size;
		for(size_t i = 0; i < n; ++i)
		{
			Push(Body(t),a[i]);
			r = i ? t : a[0];
			x.index = a + i + 1;
			if(next(m,x)) return true;
		}
		x.index = a;
		return false;
	}
};

class blanks : public pat_t
{
	size_t m_size;
public:
	blanks(pos_t &y)
	{
		m_size = y.end - y.index - 1;
	}
	bool match(map_t &m, pos_t &x)
	{
		if(!x) return false;
		var t = Ex(TAG(Sequence),Vec());
		var& r = m[0];
		var *a = x.index;
		int n = x.end - a - m_size;
		for(int i = 0; i < n; ++i)
		{
			Push(Body(t),a[i]);
			r = i ? t : a[0];
			x.index = a + i + 1;
			if(next(m,x)) return true;
		}
		x.index = a;
		return false;
	}
};

class blanks_head : public pat_t
{
	var m_head;
	size_t m_size;
public:
	blanks_head(Var x, pos_t &y) : m_head(x)
	{
		m_size = y.end - y.index - 1;
	}
	bool match(map_t &m, pos_t &x)
	{
		if(!x) return false;
		var t = Ex(TAG(Sequence),Vec());
		var &r = m[0];
		var *a = x.index;
		size_t n = x.end - a - m_size;
		for(size_t i = 0; i < n; ++i)
		{
			if(!Equal(m_head,Tag(a[i])))
				return false;
			Push(Body(t),a[i]);
			r = i ? t : a[0];
			x.index = a + i + 1;
			if(next(m,x)) return true;
		}
		x.index = a;
		return false;
	}
};

class condition : public pat_t
{
	var m_test;
public:
	condition(Var x) : m_test(x) {}
	bool match(map_t &m, pos_t &x)
	{
		if (Eval(Subs(m,m_test)) == True)
			return next(m, x);
		return false;
	}
};

class patterntest : public pat_t
{
	var m_test;
public:
	patterntest(Var x) : m_test(x) {}
	bool match(map_t &m, pos_t &x)
	{
		var& r = m[0];
		//if (ExQ(r,TAG(Sequence))) {

		//}
		if (Eval(Ex(m_test,Vec(r))) == True)
			return next(m, x);
		return false;
	}
};

class head : public pat_t
{
	var m_head;
public:
	head(Var x) : m_head(Pat(x)) {}
	bool match(map_t &m, pos_t &x)
	{
		if(!x || !ExQ(*x)) return false;
		pos_t h(Head(*x));
		if(!Match(m,m_head,h)) return false;
		pos_t y((Var)Body(*x),&x);
		return next(m,y);
	}
};

class oneidentity : public pat_t
{
	var m_head;
public:
	oneidentity(Var x) : m_head(x) {}
	bool match(map_t &m, pos_t &x)
	{
		if (!x) return false;
		var t = Vec(*x);
		pos_t y((Var)t, &x);
		bool flag = next(m,y);
		return flag;
	}
};

class push : public pat_t
{
public:
	push() {}
	bool match(map_t &m, pos_t &x)
	{
		if(!x || !VecQ(*x) || Size(*x) == 0)
			return false;
		pos_t y((Var)*x,&x);
		return next(m,y);
	}
};

class pop : public pat_t
{
public:
	pop() {}
	bool match(map_t &m, pos_t &x)
	{
		if(x || !x.prev) return false;
		++*x.prev;
		if(next(m,*x.prev)) return true;
		--*x.prev;
		return false;
	}
};

class chain : public pat_t
{
	var m_head;
	var m_tail;
public:
	chain(pos_t &x)
	{
		m_head = Pat(x);
		m_tail = m_head;
		while(++x)
		{
			Next(m_tail) = Pat(x);
			m_tail = Next(m_tail);
		}
	}
	chain(Var a, Var b) : m_head(a), m_tail(b)
	{
		Next(a) = b;
	}
	chain(Var a, Var b, Var c) : m_head(a), m_tail(c)
	{
		Next(a) = b; Next(b) = c;
	}
	chain(Var a, Var b, Var c, Var d) : m_head(a), m_tail(d)
	{
		Next(a) = b; Next(b) = c; Next(c) = d;
	}
	bool match(map_t &m, pos_t &x)
	{
		Next(m_tail) = m_next;
		return Match(m,m_head,x);
	}
};

class orderless : public pat_t
{
	std::multiset<var,Before2> m_fix;
	var m_head;
	var m_tail;
public:
	orderless(pos_t &x)
	{
		while(x)
		{
			if(FixQ(*x))
				m_fix.insert(*x);
			else if(m_tail)
			{
				Next(m_tail) = Pat(x);
				m_tail = Next(m_tail);
			}
			else
			{
				m_head = Pat(x);
				m_tail = m_head;
			}
			++x;
		}
	}
	bool match(map_t &m, pos_t &x)
	{
		std::multiset<var,Before2> fix_set;
		while(x)
		{
			fix_set.insert(*x);
			++x;
		}
		std::multiset<var,Before2>::const_iterator
			iter = m_fix.begin();
		while(iter != m_fix.end())
		{
			if(fix_set.erase(*iter) == 0)
				return false;
			++iter;
		}
		std::vector<var> fixs(fix_set.begin(),fix_set.end());
		if(m_head)
		{
			if(fixs.size() == 0) return false;
			Next(m_tail) = m_next;
			do
			{
				x.index = &fixs.front();
				x.end = &fixs.back() + 1;
				if(Match(m,m_head,x))
					return true;
			}
			while(std::next_permutation(fixs.begin(),
				fixs.end(),Before));
			return false;
		}
		return fixs.size() == 0 && next(m,x);
	}
};

class alternatives : public pat_t
{
	std::vector<var> m_patterns;
public:
	alternatives(const std::vector<var>& x)
	{
		m_patterns.resize(x.size());
		for (size_t i = 0; i < x.size(); ++i)
			m_patterns[i] = Pat(x[i]);
	}
	alternatives(Var x)
	{
		m_patterns.resize(Size(x));
		for (size_t i = 0; i < Size(x); ++i)
			m_patterns[i] = Pat(At(x,i));
	}
	alternatives(Var x, Var y)
	{
		m_patterns.resize(2);
		m_patterns[0] = x;
		m_patterns[1] = y;
	}
	alternatives(Var x, Var y, Var z)
	{
		m_patterns.resize(3);
		m_patterns[0] = x;
		m_patterns[1] = y;
		m_patterns[2] = z;
	}
	bool match(map_t &m, pos_t &x)
	{
		for (size_t i = 0; i < m_patterns.size(); ++i) {
			Next(m_patterns[i]) = m_next;
			if (Match(m, m_patterns[i], x))
				return true;
		}
		return false;
	}
};

class empty : public pat_t
{
	var m_default;
public:
	empty(Var x) : m_default(x) {}
	bool match(map_t &m, pos_t &x)
	{
		m[0] = m_default;
		return next(m, x);
	}
};
}

API var catch_all_pattern;
//////////////////////////////////////
}
