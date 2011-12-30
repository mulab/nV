#include "common.h"
#include "CRE.h"
namespace nV{
	namespace CRE{
		var normalize(Kernel& k,const var& R){
			var a=num(R),b=den(R);
			if(isZero(a))
				return a;
			if(isNumber(b)){
				return CPE::DivExact(k,a,b);
			}
			if(cmpD(CPE::lc_deep(b).object(), 0.0)<0)
				return newCRE(CPE::Neg(k,a),CPE::Neg(k,b));
		}
		var Add(Kernel& k, const var& x, const var& y){
			if(CPE::isCPE(x)){
				if(CPE::isCPE(y))
					return CPE::Add(k,x,y);
				else
					return newCRE(CPE::Add(k,CPE::Mul(k,x,den(y)),num(y)),den(y));
			}
			else{
				if(CPE::isCPE(y))
					return newCRE(CPE::Add(k,CPE::Mul(k,y,den(x)),num(x)),den(x));
				else{
					var a=num(x),b=den(x),c=num(y),d=den(y);
					if(b==d){//This is for where b,d are not exact
						var aa=CPE::Add(k,a,c);
						if(isZero(aa))
							return aa;
						return newCRE(aa,b);
					}
					if(!CPE::isExact(b)||!CPE::isExact(d)){
						var aa=CPE::Add(k,CPE::Mul(k,a,d),CPE::Mul(k,b,c));
						if(isZero(aa))
							return aa;
						var bb=CPE::Mul(k,b,d);//bb is not Exact ??
						return newCRE(aa,bb);
					}
					var temp=CPE::Gcd(k,b,d);//lc_deep(temp)>0 ??
					var b_=CPE::DivExact(k,b,temp);
					var d_=CPE::DivExact(k,d,temp);
					var bb=CPE::Mul(k,d,b_);//lc_deep(bb)>0
					var aa=CPE::Add(k,CPE::Mul(k,a,d_),CPE::Mul(k,c,b_));
					if(isZero(aa))
						return aa;
					if(!CPE::isExact(aa))
						return newCRE(aa,bb);
					temp=CPE::Gcd(k,aa,temp);//d(instead of bb) is enough
					aa=CPE::DivExact(k,aa,temp);
					bb=CPE::DivExact(k,bb,temp);//lc_deep(bb)>0
					if(isNumber(bb))
						return CPE::Div(k,aa,bb);
					return newCRE(aa,bb);
				}
			}
		}
		var Neg(Kernel& k, const var& x){
			return CRE::Mul(k,new Integer(-1L),x);
		}
		var Sub(Kernel& k, const var& x, const var& y){//x-y
			return CRE::Add(k,x,CRE::Neg(k,y));
		}
		var Mul(Kernel& k, const var& x, const var& y) {
			if(x==$.Infinity)
				if (isZero(y))
					return $.NaN;
				else
					return $.Infinity;
			if(y==$.Infinity)
				if (isZero(x))
					return $.NaN;
				else
					return $.Infinity;

			if(CPE::isCPE(x)){
				if(CPE::isCPE(y))
					return CPE::Mul(k,x,y);
				else{
					if(!CPE::isExact(x)||!CPE::isExact(den(y)))
						return newCRE(CPE::Mul(k,x,num(y)),den(y));
					var d=CPE::Gcd(k,x,den(y));
					var xx=CPE::DivExact(k,x,d);
					var bb=CPE::DivExact(k,den(y),d);//bb have non-negative lc
					var aa=CPE::Mul(k,xx,num(y));//aa/=0
					if(isNumber(bb))
						return CPE::DivExact(k,aa,bb);
					return newCRE(aa,bb);
				}
			}
			else{
				if(CPE::isCPE(y)){
					if(!CPE::isExact(y)||!CPE::isExact(den(x)))
						return newCRE(CPE::Mul(k,y,num(x)),den(x));
					var d=CPE::Gcd(k,y,den(x));
					var yy=CPE::DivExact(k,y,d);
					var bb=CPE::DivExact(k,den(x),d);//bb have non-negative lc
					var aa=CPE::Mul(k,yy,num(x));//aa/=0
					if(isNumber(bb))
						return CPE::DivExact(k,aa,bb);
					return newCRE(aa,bb);
				}
				else{
					var a=num(x),b=den(x),c=num(y),d=den(y);
					var d1,d2;
					if(CPE::isExact(a)&&CPE::isExact(d))
						d1=CPE::Gcd(k,a,d);
					else
						d1=new Integer(1L);
					if(CPE::isExact(b)&&CPE::isExact(c))
						d2=CPE::Gcd(k,b,c);
					else
						d2=new Integer(1L);
					var aa=CPE::Mul(k,CPE::DivExact(k,a,d1),CPE::DivExact(k,c,d2));//a/=0
					var bb=CPE::Mul(k,CPE::DivExact(k,d,d1),CPE::DivExact(k,b,d2));//a/=0
					if(isNumber(bb))
						return CPE::Div(k,aa,bb);
					return newCRE(aa,bb);
				}
			}
		}
		var Inv(Kernel& k, const var& x){
			if(CPE::isCPE(x)){
				if(isNumber(x)){
					if(isZero(x))
						return $.Infinity;
					return Number::Divide(x.object());
				}
				if(cmpD(CPE::lc_deep(x).object(), 0.0)>0)
					return newCRE(new Integer(1L),x);
				else
					return newCRE(new Integer(-1L),CPE::Neg(k,x));
			}
			var a=num(x),b=den(x);
			if(isNumber(a))
				return CPE::DivExact(k,b,a);
			else if(cmpD(CPE::lc_deep(a).object(), 0.0)>0)
				return newCRE(b,a);
			else
				return newCRE(CPE::Neg(k,b),CPE::Neg(k,a));
		}
		var Div(Kernel& k, const var& x, const var& y) {
			return CRE::Mul(k,x,Inv(k,y));
		}
		var Power(Kernel& k,const var& x,sint n){
			if(n==0)
				return new Integer(0L);
			if(n<0)
				return Power(k,Inv(k,x),-n);
			if(x==$.Infinity)
				return x;
			if(CPE::isCPE(x))
				return CPE::Power(k,x,n);
			var a=num(x),b=den(x);
			return newCRE(CPE::Power(k,a,n),CPE::Power(k,b,n));
		}
		var toCRE(Kernel& k, const var&x){
			if(isCRE(x))
				return x;
			if(x.isTuple()){
				if(x.tuple()[0]==$.Plus){
					var r=toCRE(k,x.tuple()[1]);
					for(uint i=2;i<x.tuple().size;i++)
						r=CRE::Add(k,r,toCRE(k,x.tuple()[i]));
					return r;
				}
				if(x.tuple()[0]==$.Times){
					var r=toCRE(k,x.tuple()[1]);
					for(uint i=2;i<x.tuple().size;i++)
						r=CRE::Mul(k,r,toCRE(k,x.tuple()[i]));
					return r;
				}
				if(x.tuple()[0]==$.Power){
					var b=x.tuple()[1],c=x.tuple()[2];//b^c
					b=toCRE(k,b);c=toCRE(k,c);
					if(c.isObject($.Integer))
						return  Power(k,b,Z(c).toSI());
					else{
						if(CPE::isCPE(b))
							return CPE::newCPE(list(b,c,$.Power));
						else{
							return newCRE(CPE::newCPE(list(num(b),c,$.Power)),
														CPE::newCPE(list(num(b),c,$.Power)));
						}
					}
				}
				//convert things in other tuple(or keep it)
				else{
					Tuple*r =tuple(x.tuple().size);
					r->tuple[0]=x.tuple()[0];
					for(uint i=1;i<x.tuple().size;i++)
						r->tuple[i]=toCRE(k,x.tuple()[i]);
					return CPE::newCPE(r);
				}
			}
			return CPE::newCPE(x);
		}
		var fromCRE(Kernel& k, const var& x){
			if(!x.isTuple())
				return x;
			else if(x.tuple()[0]==$CPE)
				return CPE::fromCPE(k,x);
			else if(x.tuple()[0]==$CRE){
				var a=CPE::fromCPE(k,num(x));
				var b=CPE::fromCPE(k,den(x));
				if(isUnit(a))
					if(b.isTuple($.Power))
						return list(b.tuple()[1],CRE::Neg(k,b.tuple()[2]),$.Power);
					else
						return list(b,new Integer(-1L),$.Power);
				else
					if(b.isTuple($.Power))
						return list(a,list(b.tuple()[1],CRE::Neg(k,b.tuple()[2]),$.Power),$.Times);
					else
						return list(a,list(b,new Integer(-1L),$.Power),$.Times);
			}
			else{
				Tuple*r =tuple(x.tuple().size);
				r->tuple[0]=x.tuple()[0];
				for(uint i=1;i<x.tuple().size;i++)
					r->tuple[i]=fromCRE(k,x.tuple()[i]);
				return r;
			}
		}
		var ExpandFromCRE(Kernel& k, const var& x){
			if(!x.isTuple())
				return x;
			else if(x.tuple()[0]==$CPE)
				return CPE::ExpandFromCPE(k,x);
			else if(x.tuple()[0]==$CRE){
				var a=CPE::ExpandFromCPE(k,num(x));
				var b=CPE::ExpandFromCPE(k,den(x));
				if(isUnit(a))
					if(b.isTuple($.Power))
						return list(b.tuple()[1],CRE::Neg(k,b.tuple()[2]),$.Power);
					else
						return list(b,new Integer(-1L),$.Power);
				else
					if(b.isTuple($.Power))
						return list(a,list(b.tuple()[1],CRE::Neg(k,b.tuple()[2]),$.Power),$.Times);
					else
						return list(a,list(b,new Integer(-1L),$.Power),$.Times);
			}
			else{
				Tuple*r =tuple(x.tuple().size);
				r->tuple[0]=x.tuple()[0];
				for(uint i=1;i<x.tuple().size;i++)
					r->tuple[i]=ExpandFromCRE(k,x.tuple()[i]);
				return r;
			}
		}
	}
}
using namespace nV;
CAPI void CVALUE(System, ToCRE)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=2)
		return;
	r=CRE::toCRE(k,x[1]);
}
CAPI void CVALUE(System, FromCRE)(Kernel& k, var& r, Tuple& x) {
	if(x.size==1)
		return;
	r=CRE::fromCRE(k,x[1]);
}
CAPI void CVALUE(System, ExpandFromCRE)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=2)
		return;
	r=CRE::ExpandFromCRE(k,x[1]);
}
CAPI void CVALUE(System, Expand)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=2)
		return;
	r=CRE::ExpandFromCRE(k,CRE::toCRE(k,x[1]));
}
