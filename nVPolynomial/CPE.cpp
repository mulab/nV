#include "common.h"
#include "CRE.h"
#include <algorithm>
#include <cmath>
//#ifndef _MSC_VER
//#include <google/heap-profiler.h>
//#endif
namespace nV {
	namespace CPE {
		vset Variables(Kernel& k,const var&x){
			vset r;
			if(!x.isTuple($CPE))
				return r;
			r.insert(variable(x));
			vset temp;
			for(uint i=0;i<nnz(x);i++){
				temp=Variables(k,coeff(x,i));
				r.insert(temp.begin(),temp.end());
			}
			return r;
		}
		var newCPE(const var& x,sint i,const var& Q){
			//A CPE contains Q*x^i
			if(i==0)
				return Q;
			Tuple* coeff=list(Q);
			var exponent=new Array<sint>;
			A<sint>(exponent).vec.push_back(i);
			return newCPE(x,coeff,exponent);
		}
		var insertCPE(const var& P,uint k,const var& c,sint e){//insert c*x^e at k-th term in P(x)
			var expo=new Array<sint>(exponent(P));
			std::vector<sint>::iterator pos=A<sint>(expo).vec.begin()+k;
			A<sint>(expo).vec.insert(pos,e);
			return newCPE(variable(P),insert(coeff(P).tuple(),k+1,c),expo);
		}
		var eraseCPE(const var& P,uint k){//erase k-th term in P(x)
			var expo=new Array<sint>(exponent(P));
			std::vector<sint>::iterator pos=A<sint>(expo).vec.begin()+k;
			A<sint>(expo).vec.erase(pos);
			return newCPE(variable(P),erase(coeff(P).tuple(),k+1),expo);
		}
		var ConstantTerm(Kernel& k,const var&P){
			if(isNumber(P))
				return P;
			if(exponent(P,0)!=0)
				return new Integer(0L);
			return ConstantTerm(k,coeff(P,0));
		}
		var ConstantTerm(Kernel& k,const var&P,const var& x){
			if(isNumber(P))
				return P;
			if(less_main(variable(P),x))//P=P(y,z),x=x
				return P;
			else if(variable(P)==x){
				if(exponent(P,0)!=0)
					return new Integer(0L);
				else
					return coeff(P,0);
			}
			else{
				std::vector<var> Sc;
				var expo=new Array<sint>;
				std::vector<sint>& Se=A<sint>(expo).vec;
				Sc.reserve(nnz(P));
				Se.reserve(nnz(P));
				for(uint i=0;i<nnz(P);i++){
					var temp=ConstantTerm(k,coeff(P,i),x);
					if(!isZero(temp)){
						Sc.push_back(temp);
						Se.push_back(exponent(P,i));
					}
				}
				//normalize
				if(Sc.size()==0)//Zero is good
					return new Integer(0L);
				if(Sc.size()==1&&Se[0]==0)
					return Sc[0];
				return CPE::newCPE(variable(P),nV::list(Sc.size(),Sc.begin()),expo);
			}
		}
		var Coefficient(Kernel& k, const var& P,const var& V){
			//P is a CPE,V is assumed to be the CPE of x^xi*y^yi...
			if(isNumber(V))//End of recursion
				return P;
			if(isNumber(P)){//Note V is not constant now
				return new Integer(0L);
			}
			if(more_main(variable(V),variable(P)))//Now V and P are not constants
				return new Integer(0L);
			else if(variable(V)==variable(P)){
				sint e=exponent(V,nnz(V)-1);//nnz(V) should be one,or choose the largest
				std::vector<sint>::const_iterator pos=std::lower_bound(exponent(P).begin(),exponent(P).end(),e);//binary search e
				if(pos==exponent(P).end()||*pos>e)
					return new Integer(0L);
				return Coefficient(k,coeff(P,pos-exponent(P).begin()),coeff(V,nnz(V)-1));
			}
			else{//variable(V) is less main than variable(P) Think about Coefficient[(z^3+z)+(2*z^3+z^2)*y,z]
				std::vector<var> Sc;
				var expo=new Array<sint>;
				std::vector<sint>& Se=A<sint>(expo).vec;
				Sc.reserve(nnz(P));
				Se.reserve(nnz(P));
				for(uint i=0;i<nnz(P);i++){
					var temp=Coefficient(k,coeff(P,i),V);
					if(!isZero(temp)){
						Sc.push_back(temp);
						Se.push_back(exponent(P,i));
					}
				}
				//normalize
				if(Sc.size()==0)//Zero is good
					return new Integer(0L);
				if(Sc.size()==1&&Se[0]==0)
					return Sc[0];
				return CPE::newCPE(variable(P),nV::list(Sc.size(),Sc.begin()),expo);
			}
		}
		var Coefficient(Kernel& k, const var& P,const var& x,uint i){
			//coefficient of x^i in P
			if(i!=0)
				return Coefficient(k,P,newCPE(x,list(new Integer(1L)),new Array<sint>(std::vector<sint>(1,i))));
			else
				return ConstantTerm(k,P,x);
		}
		var Square(Kernel& k, const var& x){
			if(!isCPE(x))
				return list(x,new Integer(2L),$.Power);
			if(isZero(x))
				return new Integer(0L);
			var r;
			if(isNumber(x))//Assume Constant means Number
				return Number::Times(x.object(),x.object());
			else
			{// x is not constants
				var expo=new Array<sint>(exponent(x));
				std::vector<sint>& Ve=A<sint>(expo).vec;
				Tuple* C=list(nnz(x));
				for(uint i=0;i<nnz(x);i++)
				{
					C->tuple[i+1]=CPE::Square(k,coeff(x,i));
					Ve[i]=Ve[i]*2;
				}
				r=CPE::newCPE(variable(x),C,expo);

				for(uint i=0;i<nnz(x)-1;i++)
				{
					C=list(nnz(x)-i-1);
					var c_i=coeff(x,i);
					sint e_i=exponent(x,i);
					var expo_i=new Array<sint>();
					std::vector<sint>& Ve_i=A<sint>(expo_i).vec;
					Ve_i.reserve(nnz(x)-i-1);
					for(uint j=0;j<nnz(x)-i-1;j++){
						C->tuple[j+1]=CPE::Mul(k,coeff(x,j+i+1),c_i);
						Ve_i.push_back(exponent(x,j+i+1)+e_i);
					}
					r=Add(k,r,CPE::Mul(k,new Integer(2L),CPE::newCPE(variable(x),C,expo_i)));
					/* y=replaceCPE(y,i,new Integer(0L));
						 item=CPE::Mul(k,y,coeff(x,i),exponent(x,i));
						 r=CPE::Add(k,r,item);
						 r=CPE::Add(k,r,item);*/
				}
				// r=CPE::Mul(k,new Integer(2L),r);
				return r;
			}
		}
		var PowerToN(Kernel& k,const var& x,const var& BT){
			uint n=BT.tuple().size-2;//Power to n
			Tuple* r=list(n+1);
			if(isNumber(x)){
				r->tuple[0+1]=BT.tuple()[0+1].tuple()[0+1];//It is one
				for(uint i=1;i<=n;i++)
					r->tuple[i+1] = Number::Times(r->tuple[i-1+1].object(),x.object());
			}
			else if(nnz(x)==1){//exponent(x,0) cannot be zero
				var temp=PowerToN(k,coeff(x,0),BT);
				r->tuple[0+1]=BT.tuple()[0+1].tuple()[0+1];
				for(uint i=1;i<=n;i++){
					var expo=new Array<sint>(exponent(x));
					std::vector<sint>& Ve=A<sint>(expo).vec;
					Ve[0]*=i;
					r->tuple[i+1]=newCPE(variable(x),list(temp.tuple()[i+1]),expo);
				}
			}
			else{
				std::pair<var,var> S=CPE::split(x);
				var x1=S.first;
				var x2=S.second;
				var Zero=new Integer(0L);
				uint start=3;//start>=3
				r->tuple[0+1]=BT.tuple()[0+1].tuple()[0+1];//one
				if(n>=1)
					r->tuple[1+1]=x;
				if(n>=2)
					r->tuple[2+1]=Square(k,x);
				if (CPE::Variables(k,x).size()==1){
					for(uint e=3;e<=n;e++)
						r->tuple[e+1]=CPE::Mul(k,x,r->tuple[e]);
					return r;
				}
				for(uint i=3;i<std::min(start,n+1);i++)
					r->tuple[i+1]=CPE::Mul(k,r->tuple[i-1+1],x);
				var Pows1=CPE::PowerToN(k,x1,BT);
				var Pows2=CPE::PowerToN(k,x2,BT);
				for(uint i=start;i<=n;i++)
					r->tuple[i+1]=Zero;
				for(uint i=0;i<=n;i++)
					for(uint j=std::max(i+1,start>=i?start-i:0);j<=n-i;j++){
						var temp=CPE::Add(k,CPE::Mul(k,Pows1.tuple()[i+1],Pows2.tuple()[j+1]),
															CPE::Mul(k,Pows1.tuple()[j+1],Pows2.tuple()[i+1]));
						temp=CPE::Mul(k,BT.tuple()[i+j+1].tuple()[i+1],temp);
						r->tuple[i+j+1]=CPE::Add(k,r->tuple[i+j+1],temp);
					}
				for(uint i=ceil(start/2.0);i<=n/2;i++){
					var temp=CPE::Mul(k,Pows1.tuple()[i+1],Pows2.tuple()[i+1]);
					temp=CPE::Mul(k,BT.tuple()[i+i+1].tuple()[i+1],temp);
					r->tuple[i+i+1]=CPE::Add(k,r->tuple[i+i+1],temp);
				}
			}
			return r;
		}
		var PowerToSomeN(Kernel& k,const var& x,const std::vector<sint> & expo){
			// assume expo is sorted
			uint m=expo.size();
			if(m==0)
				return list(0);
			sint n=expo[m-1];
			const var BT=BinomialTable(k,n);
			var Zero=new Integer(0L);
			var One=new Integer(1L);
			if(isNumber(x)){
				Tuple* r=list(n+1);
				r->tuple[0+1]=One;//It is one
				for(uint i=1;i<=n;i++)
					r->tuple[i+1] = Number::Times(r->tuple[i-1+1].object(),x.object());
				Tuple* rr=list(m);
				for(uint i=0;i<m;i++)
					rr->tuple[i+1]=r->tuple[expo[i]+1];
				return rr;
			}
			else if(nnz(x)==1){//exponent(x,0) cannot be zero
				Tuple* r=list(m);
				var temp=PowerToSomeN(k,coeff(x,0),expo);
				for(uint i=0;i<m;i++){
					r->tuple[i+1]=Zero;
					var expone=new Array<sint>(exponent(x));
					std::vector<sint>& Ve=A<sint>(expone).vec;
					if(expo[i]==0)
						r->tuple[i+1]=One;
					else 	if(expo[i]==1)
						r->tuple[i+1]=x;
					else if(expo[i]==2)
						r->tuple[i+1]=Square(k,x);
					else{
						Ve[0]*=expo[i];
						r->tuple[i+1]=newCPE(variable(x),list(temp.tuple()[i+1]),expone);
					}
				}
				return r;
			}
			else if(CPE::Variables(k,x).size()==1){
				Tuple* r=list(n+1);
				r->tuple[0+1]=One;//one
				if(n>=1)
					r->tuple[1+1]=x;
				if(n>=2)
					r->tuple[2+1]=Square(k,x);
				for(uint e=3;e<=n;e++)
					r->tuple[e+1]=CPE::Mul(k,x,r->tuple[e]);
				Tuple* rr=list(m);
				for(uint i=0;i<m;i++)
					rr->tuple[i+1]=r->tuple[expo[i]+1];
				return rr;
			}
			else{
				std::pair<var,var> S=CPE::split(x);
				var x1=S.first;
				var x2=S.second;

				Tuple* r=list(m);
				var Pow1=CPE::PowerToN(k,x1,BT);
				var Pow2=CPE::PowerToN(k,x2,BT);
				for(uint i=0;i<m;i++)
					r->tuple[i+1]=Zero;
				for(uint j=0;j<m;j++){
					var r_e=new Integer(0L);
					sint e=expo[j];
					if(e==0){
						r->tuple[j+1]=One;
						continue;
					}
					if(e==1){
						r->tuple[j+1]=x;
						continue;
					}
					if(e==2){
						r->tuple[j+1]=Square(k,x);
						continue;
					}
					if(e%2==0){//e is even
						for(uint i=0;i<=e/2-1;i++){
							var temp=CPE::Add(k,CPE::Mul(k,Pow1.tuple()[i+1],Pow2.tuple()[e-i+1]),CPE::Mul(k,Pow2.tuple()[i+1],Pow1.tuple()[e-i+1]));
							temp=CPE::Mul(k,BT.tuple()[e+1].tuple()[i+1],temp);
							r_e=CPE::Add(k,r_e,temp);
						}
						var temp=CPE::Mul(k,Pow2.tuple()[e/2+1],Pow1.tuple()[e/2+1]);
						temp=CPE::Mul(k,BT.tuple()[e+1].tuple()[e/2+1],temp);
						r_e=CPE::Add(k,r_e,temp);
					}
					else{//e is not even
						for(uint i=0;i<=e/2;i++){
							var temp=CPE::Add(k,CPE::Mul(k,Pow1.tuple()[i+1],Pow2.tuple()[e-i+1]),CPE::Mul(k,Pow2.tuple()[i+1],Pow1.tuple()[e-i+1]));
							temp=CPE::Mul(k,BT.tuple()[e+1].tuple()[i+1],temp);
							r_e=CPE::Add(k,r_e,temp);
						}
					}
					r->tuple[j+1]=r_e;
				}
				return r;
			}
		}
		var Eval(Kernel& k,const var &P,const var& varia,const var& x){
			if(x==varia)
				return P;
			if(!isCPE(P)||!isCPE(x))
				return P;
			if(isNumber(P))
				return P;
			if(nnz(P)<=0)
				return new Integer(0L);
			if(more_main(varia,variable(P)))
				return P;
			if(variable(P)==varia){
				var expo=new Array<sint>(exponent(P));
				std::vector<sint>& Ve=A<sint>(expo).vec;
				var power_value=PowerToSomeN(k,x,Ve);
				var r=new Integer(0L);
				for(uint i=0;i<nnz(P);i++)
					r=Add(k,r,CPE::Mul(k,power_value.tuple()[i+1],coeff(P,i)));
				return r;
			}
			var expo=new Array<sint>(exponent(P));
			std::vector<sint>& Ve=A<sint>(expo).vec;
			var power_value=PowerToSomeN(k,toCPE(k,variable(P)),Ve);
			var r=new Integer(0L);
			for(uint i=0;i<nnz(P);i++){
				var coeff_i=coeff(P,i);
				r=Add(k,r,CPE::Mul(k,power_value.tuple()[i+1],Eval(k,coeff_i,varia,x)));
			}
			return r;
		}

		std::pair<var,var> split(const var b){
			//split at half or at first don't show much difference in benchmark
			uint n1=ceil(nnz(b)/2.0);//assume nnz(b)>1
			//uint n1=1;//assume nnz(b)>1
			Tuple* C1=list(n1);
			var expo1=new Array<sint>;
			std::vector<sint>& Ve1=A<sint>(expo1).vec;
			Ve1.reserve(n1);
			for(uint i=0;i<n1;i++){
				C1->tuple[i+1]=coeff(b,i);
				Ve1.push_back(exponent(b,i));
			}
			var b1;
			if(Ve1.size()==1&&Ve1[0]==0)//normalize
				b1=C1->tuple[0+1];
			else
				b1=newCPE(variable(b),C1,expo1);

			uint n2=nnz(b)-n1;
			Tuple* C2=list(n2);
			var expo2=new Array<sint>;
			std::vector<sint>& Ve2=A<sint>(expo2).vec;
			Ve1.reserve(n1);
			for(uint i=0;i<n2;i++){
				C2->tuple[i+1]=coeff(b,n1+i);
				Ve2.push_back(exponent(b,n1+i));
			}
			var b2=newCPE(variable(b),C2,expo2);

			return std::make_pair(b1,b2);
		}
		var Power0(Kernel& k,const var& b,uint e){
			if(e==0)
				return new Integer(1L);
			var r;
			if(e==1)
				return b;
			else{
				var r=CPE::Square(k,b);
				for(uint i=0;i<e-2;i++)//e>=2
					r= CPE::Mul(k,r,b);
				return r;
			}
		}
		var Power(Kernel& k,const var& b,uint e){
			if(!isCPE(b))
				return list(b,new Integer(e),$.Power);
			if(e==0)
				return new Integer(1L);
			if(e==1)
				return b;
			else{
				if(isNumber(b))
					return Number::Power(b.object(),*(new Integer(e)));//Should have a pow accept uint
#ifdef CPE_USE_NTL_FLINT
				UniType type=CPE::unitype(b);
				if(type==Integer_t){
					var temp= k.value(tuple(SYM(SYM(root,FLINT),ZPower),ToFullCL(k,b),new Integer(e)));
					return FromFullCL(k,variable(b),temp);
				}
				else if(type==Rational_t){
					var temp= k.value(tuple(SYM(SYM(root,FLINT),QPower),ToFullCL(k,b),new Integer(e)));
					return FromFullCL(k,variable(b),temp);
				}
				else if(type==Real_t)
					return Power0(k,b,e);
#else
				if(isUnivariate(b))
					return Power0(k,b,e);
#endif

				if(nnz(b)==1){//exponent(x,0) cannot be zero
					var expo=new Array<sint>(exponent(b));
					std::vector<sint>& Ve=A<sint>(expo).vec;
					Ve[0]*=e;
					return newCPE(variable(b),list(CPE::Power(k,coeff(b,0),e)),expo);
				}

				//nnz(b)>=2;
				std::pair<var,var> S=CPE::split(b);
				var b1=S.first;
				var b2=S.second;
				var BT=BinomialTable(k,e);
				var Pow1=CPE::PowerToN(k,b1,BT);
				var Pow2=CPE::PowerToN(k,b2,BT);
				var r=new Integer(0L);
				if(e%2==0){
					for(uint i=0;i<=e/2-1;i++){
						var temp=CPE::Add(k,CPE::Mul(k,Pow1.tuple()[i+1],Pow2.tuple()[e-i+1]),CPE::Mul(k,Pow2.tuple()[i+1],Pow1.tuple()[e-i+1]));
						temp=CPE::Mul(k,BT.tuple()[e+1].tuple()[i+1],temp);
						r=CPE::Add(k,r,temp);
					}
					var temp=CPE::Mul(k,Pow2.tuple()[e/2+1],Pow1.tuple()[e/2+1]);
					temp=CPE::Mul(k,BT.tuple()[e+1].tuple()[e/2+1],temp);
					r=CPE::Add(k,r,temp);
				}
				else{
					for(uint i=0;i<=e/2;i++){
						var temp=CPE::Add(k,CPE::Mul(k,Pow1.tuple()[i+1],Pow2.tuple()[e-i+1]),CPE::Mul(k,Pow2.tuple()[i+1],Pow1.tuple()[e-i+1]));
						temp=CPE::Mul(k,BT.tuple()[e+1].tuple()[i+1],temp);
						r=CPE::Add(k,r,temp);
					}
				}
				return r;
			}
		}

		var toCPE(Kernel& k, const var&x){
			if(isCPE(x))
				return x;
			if(x.isTuple()){
				if(x.tuple()[0]==$.Plus){
					var r=toCPE(k,x.tuple()[1]);
					for(uint i=2;i<x.tuple().size;i++)
						r=CPE::Add(k,r,toCPE(k,x.tuple()[i]));
					return r;
				}
				if(x.tuple()[0]==$.Times){
					var r=toCPE(k,x.tuple()[1]);
					for(uint i=2;i<x.tuple().size;i++)
						r=CPE::Mul(k,r,toCPE(k,x.tuple()[i]));
					return r;
				}
				if(x.tuple()[0]==$.Power){
					var b=x.tuple()[1],c=x.tuple()[2];//b^c
					b=toCPE(k,b);c=toCPE(k,c);
					if(c.isObject($.Integer)){
						sint se = Z(c).toSI();
						uint e = se > 0 ? se : -se;
						var r = CPE::Power(k,b,e);
						return se > 0 ? r : CPE::newCPE(list(r,new Integer(-1L),$.Power));
					}
					else
						return CPE::newCPE(list(b,c,$.Power));
				}
				//convert things in other tuple(or keep it)
				else{
					Tuple*r =tuple(x.tuple().size);
					r->tuple[0]=x.tuple()[0];
					for(uint i=1;i<x.tuple().size;i++)
						r->tuple[i]=toCPE(k,x.tuple()[i]);
					return CPE::newCPE(r);
				}
			}
			return CPE::newCPE(x);
		}
		var fromCPE(Kernel& k,const var&x,const var& c,const sint& e){//convert c*x^e
			// some check are not necessary since exponent are orederd
			if(c.isObject()&&! cmpD(c.object(), 1.0)){//c=1
				if(e==0)
					return new Integer(1L);
				else if(e==1)
					return x;
				else
					return list(x,new Integer(e),$.Power);
			}
			else{
				if(e==0)
					return c;
				else if(e==1)
					return k.flatten($.Times,*list(c,x,$.Times));
				else
					return k.flatten($.Times,*list(c,list(x,new Integer(e),$.Power),$.Times));
			}
		}
		var fromCPE(Kernel& k, const var& P){
			if(!P.isTuple())
				return P;
			else if(P.tuple()[0]==$CRE)
				return CRE::fromCRE(k,P);
			else if(P.tuple()[0]==$CPE){
				const var& x=fromCPE(k,variable(P));//variable should also be converted
				if(nnz(P)==1)
					return fromCPE(k,x,fromCPE(k,CPE::coeff(P,0)),CPE::exponent(P,0));
				Tuple* r=list(nnz(P),$.Plus);
				for(uint i=0;i<nnz(P);i++){
					r->tuple[i+1]=fromCPE(k,x,fromCPE(k,coeff(P,i)),exponent(P,i));
				}
				return k.eval(r);
			}
			else{
				Tuple*r =tuple(P.tuple().size);
				r->tuple[0]=P.tuple()[0];
				for(uint i=1;i<P.tuple().size;i++)
					r->tuple[i]=fromCPE(k,P.tuple()[i]);
				return r;
			}
		}
		var ExpandFromCPE(Kernel& k,const var&x,const var& c,const sint& e){//convert c*x^e
			if(c.isTuple($.Plus)){
				Tuple *r=tuple(c.tuple().size);
				r->tuple[0]=$.Plus;
				for(uint i=1;i<c.tuple().size;i++){//c should have >= 1 operand
					r->tuple[i]=fromCPE(k,x,c.tuple()[i],e);
				}
				return k.eval(r);
			}
			else
				return fromCPE(k,x,c,e);
		}
		var ExpandFromCPE(Kernel& k, const var& P){
			if(!P.isTuple())
				return P;
			else if(P.tuple()[0]==$CRE)
				return CRE::ExpandFromCRE(k,P);
			else if(P.tuple()[0]==$CPE){
				const var& x=fromCPE(k,variable(P));
				if(nnz(P)==1)
					return ExpandFromCPE(k,x,ExpandFromCPE(k,coeff(P,0)),exponent(P,0));
				Tuple* r=list(nnz(P),$.Plus);
				for(uint i=0;i<nnz(P);i++)
					r->tuple[i+1]=ExpandFromCPE(k,x,ExpandFromCPE(k,coeff(P,i)),exponent(P,i));
				return k.flatten($.Plus,*r);
			}
			else{
				Tuple*r =tuple(P.tuple().size);
				r->tuple[0]=P.tuple()[0];
				for(uint i=1;i<P.tuple().size;i++)
					r->tuple[i]=ExpandFromCPE(k,P.tuple()[i]);
				return r;
			}
		}
		var Add(Kernel& k,const var& x,const var& y){
			if((!isCPE(x)||!isCPE(y)))
				return list(x,y,$.Plus);
			if(isZero(x))
				return y;
			if(isZero(y))
				return x;

			//Handle Constant
			var xx,yy;//Used to simplify the code for different variales situation.Add yy to coeffs of xx
			if(isNumber(x)){//Assume Constant means Number
				if(isNumber(y))
					return Number::Plus(x.object(),y.object());
				else{
					xx=y;
					yy=x;
				}
			}
			else if(isNumber(y)){
				xx=x;
				yy=y;
			}
			else {// x,y are not constants
				if(variable(x)==variable(y)){
#ifdef CPE_USE_NTL_FLINT
					// Don't use flint in Add. It's not worth the conversion!!!
					// sint type1=CPE::AboutType(k,x);
					// sint type2=CPE::AboutType(k,y);
					// if(type1>0&&type2>0){
					// 	if((type1==1)&&(type2==1)){
					// 		var temp= k.value(tuple(SYM(SYM(root,NTL),Add),ToFullCL(k,x),ToFullCL(k,y)));
					// 		return FromFullCL(k,variable(x),temp);
					// 	}
					// 	else if((type1<=2)||(type2<=2)){
					// 		var temp= k.value(tuple(SYM(SYM(root,FLINT),QAdd),ToFullCL(k,x),ToFullCL(k,y)));
					// 		return FromFullCL(k,variable(x),temp);
					// 	}
					// }
#endif

					var r=SparseArray::Add(k,coeff_list(x),coeff_list(y));
					//normalize
					if(SparseArray::nnz(r)==0)//Zero is good
						return new Integer(0L);
					if(SparseArray::nnz(r)==1&&SparseArray::index(r,0)==0)
						return SparseArray::data(r,0);
					return CPE::newCPE(variable(x),r);
				}
				else if (more_main(variable(x),variable(y))){
					xx=x;
					yy=y;
				}
				else{
					xx=y;
					yy=x;
				}
			}

			//Now the situation for different variables
			if(exponent(xx,0)>0){
				return consCPE(yy,0,xx);
			}
			else{//exponent(xx,0)==0,must have higher exponent or will be normailized
				var temp=Add(k,yy,coeff(xx,0));
				if(isZero(temp))
					return eraseCPE(xx,0);
				else
					return replaceCPE(xx,0,temp);
			}
		}
		var Neg(Kernel& k, const var& x){
			return CPE::Mul(k,new Integer(-1L),x);
		}
		var Sub(Kernel& k, const var& x, const var& y){//x-y
			return CPE::Add(k,x,CPE::Neg(k,y));
		}
		var Shift(Kernel& k,const var& P,sint e){//P(x)*x^e
			//c is not Zero,P is not Constant
			if(e==0)
				return P;
			var expo=new Array<sint>(exponent(P));
			std::vector<sint>& Ve=A<sint>(expo).vec;
			for(uint i=0;i<nnz(P);i++){
				Ve[i]+=e;
			}
			return CPE::newCPE(variable(P),coeff(P),expo);
		}
		var Mul(Kernel& k,const var& P,const var& c,const sint& e){//P(x)*(c*x^e)
			//c is not Zero,P is not Constant
			var expo=new Array<sint>(exponent(P));
			std::vector<sint>& Ve=A<sint>(expo).vec;
			Tuple* C=list(nnz(P));
			for(uint i=0;i<nnz(P);i++){
				C->tuple[i+1]=CPE::Mul(k,c,coeff(P,i));
				Ve[i]+=e;
			}
			return CPE::newCPE(variable(P),C,expo);
		}
		var Mul(Kernel& k, const var& x, const var& y) {
			if((!isCPE(x)||!isCPE(y)))
				return list(x,y,$.Times);
			if(isZero(x)||isZero(y))
				return new Integer(0L);

			var xx,yy;
			if(isNumber(x)){//Assume Constant means Number
				if(isNumber(y))
					return Number::Times(x.object(),y.object());
				else
				{
					xx=y;
					yy=x;
				}
			}
			else if(isNumber(y)){
				xx=x;
				yy=y;
			}
			else {// x,y are not constants
				if(variable(x)==variable(y)){
#ifdef CPE_USE_NTL_FLINT
					UniType type1=unitype(x);
					UniType type2=unitype(y);
					if(type1>0&&type2>0){//All are univariate.See CRE.h for definition UniType
						if((type1==1)&&(type2==1)){//All are Z[x]
							var temp= k.value(tuple(SYM(SYM(root,NTL),Mul),ToFullCL(k,x),ToFullCL(k,y)));
							return FromFullCL(k,variable(x),temp);
						}
						if((type1<=2)&&(type2<=2)){//All are Z[x] or Q[x]
							var temp= k.value(tuple(SYM(SYM(root,FLINT),QMul),ToFullCL(k,x),ToFullCL(k,y)));
							return FromFullCL(k,variable(x),temp);
						}
					}
#endif
					return newCPE(variable(x),SparseArray::Convolution(k,coeff_list(x),coeff_list(y)));
				}
				else if (more_main(variable(x),variable(y))){
					xx=x;
					yy=y;
				}
				else{
					xx=y;
					yy=x;
				}
			}

			//Now the situation for different variables
			Tuple* C=list(nnz(xx));
			for(uint i=0;i<nnz(xx);i++)
				C->tuple[i+1]=CPE::Mul(k,yy,coeff(xx,i));//Result cannnot be zero
			return CPE::newCPE(variable(xx),C,exponent_as_var(xx));
		}
		var ToFullCL(Kernel& k, const var& P){
			if(!isCPE(P))
				return P;
			if(isNumber(P))
				return list(P);
			else{
				uint n=nnz(P);
				uint e=exponent(P,n-1);
				Tuple *C=list(e+1);
				uint j=0;
				uint expo=exponent(P,j);
				var Zero=new Integer(0L);
				for(uint i=0;i<=e;i++){
					if(i==expo){
						C->tuple[i+1]=coeff(P,j);
						j=j+1;
						if(j<n)
							expo=exponent(P,j);
					}
					else
						C->tuple[i+1]=Zero;
				}
				return C;
			}
		}
		var FromFullCL(Kernel& k, const var &x,const var& P){
			if(!P.isTuple())
				return P;
			else{
				Tuple &li=P.tuple();
				sint e=li.size-2;
				if (e<0)
					return new Integer(0L);
				if(e==0)
					return li[1];
				var expo=new Array<sint>;
				std::vector<sint>& Ve=A<sint>(expo).vec;
				sint j=0;
				for(sint i=0;i<=e;i++){
					var coeff_i=P.tuple()[i+1];
					if(!isZero(coeff_i)){
						Ve.push_back(i);
						j=j+1;
					}
				}
				j=j-1;
				if(j<0)
					return new Integer(0L);
				if(j==0&&Ve[0]==0)
					return P.tuple()[1];
				Tuple *C=list(j+1);

				for(uint i=0;i<=j;i++)
					C->tuple[i+1]=P.tuple()[Ve[i]+1];
				return CPE::newCPE(x,C,expo);
				//return new Integer(e);
			}
		}
		API bool isUnivariate(const var& P){
			for(uint i=0;i<nnz(P);i++)
				if(!isNumber(coeff(P,i)))
					return false;
			return true;
		}
		UniType unitype(const var& x){
			if(!x.isTuple($CPE))
				return nonCPE_t;
			if(isNumber(x))
				return number_t;
			if(!isUnivariate(x))
				return multivariate_t;
			UniType type=Integer_t;
			for(uint i=0;i<nnz(x);i++){
				var coeff_i=coeff(x,i);
				if(coeff_i.isObject($.Real))
					return Real_t;
				else if(coeff_i.isObject($.Rational))
					type=Rational_t;
			}
			return type;
		}
		sym coeff_type(const var& P){
			if(isNumber(P))
				return P.object().type;
			sym r=$.Integer;
			for(uint i=0;i<nnz(P);i++){
				sym type=coeff_type(coeff(P,i));
				if(type==$.Real)
					return $.Real;
				else if(type==$.Rational)
					r=type;
			}
			return r;
		}
		var replace_xi(Kernel& k,const var& P,const var &x ,uint i,const var& c){//replace x^i to c
			//x is in Variables(P)
			var x_power_i,coeff_i;
			if(i==0){
				x_power_i=new Integer(1L);
			}
			else{
				var expo=new Array<sint>(std::vector<sint>(1,i));
				x_power_i=newCPE(x,list(new Integer(1L)),expo);
			}
			coeff_i=Coefficient(k,P,x,i);
			if(isZero(coeff_i))
				return P;
			var cut=CPE::Mul(k,coeff_i,x_power_i);
			var P_i=CPE::Sub(k,P,cut);
			var item=CPE::Mul(k,coeff_i,c);
			P_i=CPE::Add(k,P_i,item);
			return P_i;
		}
		var ModI(Kernel& kernel, const var& P,const std::vector<var>& I,sint k,uint start){
			if(k<=0)
				return new Integer(0L);
			if(start>=I.size()||isNumber(P))
				return P;
			const var& x=variable(P);
			const var& c=*(I.begin()+start);
			std::vector<var> Sc;
			var expo=new Array<sint>;
			std::vector<sint>& Se=A<sint>(expo).vec;

			if(x==c){
				start++;
				for(uint i=0;i<nnz(P);i++){
					var temp=ModI(kernel,coeff(P,i),I,k-exponent(P,i),start);
					if(!isZero(temp)){
						Sc.push_back(temp);
						Se.push_back(exponent(P,i));
					}
				}
			}
			else if(more_main(c,x)){
				start++;
				return ModI(kernel,P,I,k,start);
			}
			else{
				for(uint i=0;i<nnz(P);i++){
					var temp=ModI(kernel,coeff(P,i),I,k,start);
					if(!isZero(temp)){
						Sc.push_back(temp);
						Se.push_back(exponent(P,i));
					}
				}
			}
			if(Sc.size()==0)//Zero is good
				return new Integer(0L);
			if(Sc.size()==1&&Se[0]==0)
				return Sc[0];
			return CPE::newCPE(variable(P),nV::list(Sc.size(),Sc.begin()),expo);
		}
		var ModP(Kernel& k, const var& x,const var& p){
			if(isNumber(x))
				return Number::Mod(Z(x),Z(p));
			std::vector<var> Sc;
			var expo=new Array<sint>;
			std::vector<sint>& Se=A<sint>(expo).vec;
			for(uint i=0;i<nnz(x);i++){
				var temp=ModP(k,coeff(x,i),p);
				if(!isZero(temp)){
					Sc.push_back(temp);
					Se.push_back(exponent(x,i));
				}
			}
			if(Sc.size()==0)//Zero is good
				return new Integer(0L);
			if(Sc.size()==1&&Se[0]==0)
				return Sc[0];
			return CPE::newCPE(variable(x),nV::list(Sc.size(),Sc.begin()),expo);
		}
		var Mod2P(Kernel& k, const var& x,const var& p){
			if(isNumber(x))
				return Number::Mod2(Z(x),Z(p));
			std::vector<var> Sc;
			var expo=new Array<sint>;
			std::vector<sint>& Se=A<sint>(expo).vec;
			for(uint i=0;i<nnz(x);i++){
				var temp=Mod2P(k,coeff(x,i),p);
				if(!isZero(temp)){
					Sc.push_back(temp);
					Se.push_back(exponent(x,i));
				}
			}
			if(Sc.size()==0)//Zero is good
				return new Integer(0L);
			if(Sc.size()==1&&Se[0]==0)
				return Sc[0];
			return CPE::newCPE(variable(x),nV::list(Sc.size(),Sc.begin()),expo);
		}
		var lc(Kernel& k,const var& P, const var& x){
			sint deg=degree(k,P,x);
			return Coefficient(k,P,x,deg);
		}
		var lc_deep(const var& P){
			if(isNumber(P))
				return P;
			return lc_deep(lc(P));
		}
		sint degree(Kernel& k,const var& P, const var &varia){
			if(isZero(P))
				return 0;
			if(isNumber(P))
				return 0;
			if(more_main(varia,variable(P)))
				return 0;
			else if(varia==variable(P))
				return exponent(P,nnz(P)-1);
			else {
				sint r=0;
				for(uint i=0;i<nnz(P);i++){
					sint item=degree(k,coeff(P,i),varia);
					if(item>r)
						r=item;
				}
				return r;
			}
		}
		var cdr(const var& P){
			uint n=nnz(P);
			if(n==1)
				return new Integer(0L);
			if(n==2&&exponent(P,0)==0)
				return coeff(P,0);
			Tuple* C=list(n-1,coeff(P).tuple().tuple+1);
			var expo=new Array<sint>;
			std::vector<sint>& Se=A<sint>(expo).vec;
			Se.assign(exponent(P).begin(),exponent(P).end()-1);
			return newCPE(variable(P),C,expo);
		}
		var DivExact(Kernel& k,const var& f,const var& g){
			if(!isCPE(f)||!isCPE(g))
				return list(f,list(g,new Integer(-1L),$.Power),$.Times);
			if(isNumber(g)){
				if(isZero(g))
					return $.Infinity;
				return CPE::Mul(k,f,Number::Divide(g.object()));
			}
			if(isNumber(f)){//g is not number
				if(isZero(f))
					return f;
				return $.False;
			}
			if(more_main(variable(g),variable(f)))//f=x+1;g=y
				return $.False;
			if(variable(f)==variable(g)){
				if(exponent(g,0)>exponent(f,0))
					return $.False;
				var x=variable(g);
				var q=new Integer(0L);
				var r=f;
				var g_cdr=cdr(g);
				while(!isNumber(r)&&variable(r)==x&&degree(r)>=degree(g)){
					uint s=degree(r)-degree(g);
					var qq=DivExact(k,lc(r),lc(g));
					if(qq==$.False)
						return qq;
					r=CPE::Sub(k,cdr(r),CPE::Mul(k,CPE::Mul(k,qq,g_cdr),newCPE(x,s)));
					q=CPE::Add(k,q,newCPE(x,s,qq));
				}
				if(!isZero(r))
					return $.False;
				else
					return q;
			}
			//variable(f) is more main f=y^2*x^2+x^3 g=x
			Tuple* C=list(nnz(f));
			for(uint i=0;i<nnz(f);i++){
				var q=DivExact(k,coeff(f,i),g);
				if(q==$.False)
					return $.False;
				C->tuple[i+1]=q;
			}
			return newCPE(variable(f),C,exponent_as_var(f));
		}
		var Div(Kernel& k,const var& f,const var& g){
			if(!isCPE(f)||!isCPE(g))
				return list(list(f,list(g,new Integer(-1L),$.Power),$.Times),new Integer(0L));
			if(isNumber(g)){
				if(isZero(g))
					return list($.Infinity,new Integer(0L));
				return list(CPE::Mul(k,f,Number::Divide(g.object())),new Integer(0L));
			}
			if(isNumber(f)){//g is not number
				if(isZero(f))
					return list(f,f);
				return list(new Integer(0L),f);
			}
			if(more_main(variable(f),variable(g)))//f=x+1;g=y
				return list(new Integer(0L),f);
			if(variable(f)==variable(g)){
				var x=variable(g);
				var q=new Integer(0L);
				var r=f;
				var g_cdr=cdr(g);
				while(!isNumber(r)&&variable(r)==x&&degree(r)>=degree(g)){
					uint s=degree(r)-degree(g);
					var temp=Div(k,lc(r),lc(g));
					var qq=temp.tuple()[1];
					var rr=temp.tuple()[2];
					if(!isZero(rr))
						break;
					r=CPE::Sub(k,cdr(r),CPE::Mul(k,CPE::Mul(k,qq,g_cdr),newCPE(x,s)));
					q=CPE::Add(k,q,newCPE(x,s,qq));
				}
				return list(q,r);
			}
			//variable(f) is more main f=y^2*x^2+x^3 g=x
			std::vector<var> Scq;
			var expoq=new Array<sint>;
			std::vector<sint>& Seq=A<sint>(expoq).vec;
			std::vector<var> Scr;
			var expor=new Array<sint>;
			std::vector<sint>& Ser=A<sint>(expor).vec;
			for(uint i=0;i<nnz(f);i++){
				var temp=Div(k,coeff(f,i),g);
				var q=temp.tuple()[1];
				var r=temp.tuple()[2];
				if(!isZero(q)){
					Scq.push_back(q);
					Seq.push_back(exponent(f,i));
				}
				if(!isZero(r)){
					Scr.push_back(r);
					Ser.push_back(exponent(f,i));
				}
			}
			var rq,rr;
			if(Scq.size()==0)//Zero is good
				rq=new Integer(0L);
			else if(Scq.size()==1&&Seq[0]==0)
				rq=Scq[0];
			else
				rq=CPE::newCPE(variable(f),nV::list(Scq.size(),Scq.begin()),expoq);
			if(Scr.size()==0)//Zero is good
				rr=new Integer(0L);
			else if(Scr.size()==1&&Ser[0]==0)
				rr=Scr[0];
			else
				rr=CPE::newCPE(variable(f),nV::list(Scr.size(),Scr.begin()),expor);
			return list(rq,rr);
		}
		void leafs(const var& P,std::vector<var>& L){
			if(isNumber(P))
				L.push_back(P);
			else{
				for(uint i=0;i<nnz(P);i++){
					L.reserve(nnz(P));
					leafs(coeff(P,i),L);
				}
			}
		}
		var exact(const var& P){
			if(isNumber(P)){
				if(P.object().type==$.Integer||P.object().type==$.Rational)//do not convert Integer to Rational
					return P;
				else
					return Number::cast(P.object(),$.Rational);
			}
			Tuple* C=list(nnz(P));
			for(uint i=0;i<nnz(P);i++)
				C->tuple[i+1]=exact(coeff(P,i));
			return newCPE(variable(P),C,exponent_as_var(P));
		}
		var LCM_of_coeff_den(const var& P){
			if(isNumber(P))
				return denominator(P.object());
			var r=LCM_of_coeff_den(coeff(P,0));
			for(uint i=1;i<nnz(P);i++){
				r=Number::LCM(r.cast<Integer>(),LCM_of_coeff_den(coeff(P,i)).cast<Integer>());
			}
			return r;
		}
		var content(const var& P){
			//P is in Q[x],get cont(P) such that P=cont(P)*pp(P) s.t cont(P)\in Q and pp(P) is pp in Z[...]
			if(isNumber(P)){
				if(isZero(P))
					return new Integer(1L);
				return P;
			}
			var r=content(coeff(P,0));
			for(uint i=1;i<nnz(P);i++){
				r=GCD(r.object(),content(coeff(P,i)).object());
			}
			return r;
		}
		var integralize(Kernel& k,const var& P){
			var lcm=LCM_of_coeff_den(P);
			return list(lcm,CPE::Mul(k,lcm,P));
		}
		var ppZdecompose(Kernel& k,const var& P){
			var cont=content(P);
			var pp=CPE::Mul(k,P,Number::Divide(cont.object()));
			return list(cont,pp);
		}
		var exchange(Kernel& k,const var& x,const var& Q,uint i){
			//get Q*x^i with main variable varibale(Q)
			if(i==0)
				return Q;
			var y=variable(Q);
			uint n=nnz(Q);
			Tuple* C=list(n);
			for(uint j=0;j<n;j++)
				C->tuple[j+1]=newCPE(x,i,coeff(Q,j));
			return newCPE(y,C,exponent_as_var(Q));
		}
		var to_main(Kernel& k,const var& P,const var& x){
			if(isNumber(P))
				return P;
			if(less_main(variable(P),x)||variable(P)==x)//x is more main
				return P;
			var r1=new Integer(0L);
			var expo2=new Array<sint>;
			std::vector<sint>& Se2=A<sint>(expo2).vec;
			std::vector<uint> remained;
			for(uint i=0;i<nnz(P);i++){
				var temp=to_main(k,coeff(P,i),x);
				if(temp.isTuple($CPE)&&variable(temp)==x)
					r1=CPE::Add(k,r1,exchange(k,variable(P),temp,exponent(P,i)));
				else
					remained.push_back(i);
			}
			if(remained.size()==nnz(P))
				return P;
			if(remained.size()==0)
				return r1;
			Tuple* C2=list(remained.size());
			for(uint i=0;i<remained.size();i++){
				C2->tuple[i+1]=coeff(P,remained[i]);
				Se2.push_back(exponent(P,remained[i]));
			}
			var r2=newCPE(variable(P),C2,expo2);
			if(exponent(r1,0)>0){
				return consCPE(r2,0,r1);
			}
			else{
				var temp=CPE::Add(k,r2,coeff(r1,0));
				if(isZero(temp))
					return eraseCPE(r1,0);
				else
					return replaceCPE(r1,0,temp);
			}
		}
		var from_main(Kernel& k,const var& P,const var& y){
			if(isNumber(P))
				return P;
			var x=variable(P);
			if(x!=y)
				return P;
			var r=new Integer(0L);
			for(uint i=0;i<nnz(P);i++)
				r=CPE::Add(k,r,CPE::Mul(k,coeff(P,i),newCPE(x,exponent(P,i))));
			return r;
		}
		var Derivate(Kernel& k,const var& P){
			if(isNumber(P))
				return new Integer(0L);
			uint start=0;
			uint n=nnz(P);
			if(exponent(P,start)==0){//we have nnz(P)>1 here
				start=1;
				n--;
			}
			if(n==1&&exponent(P,start)==1)
				return coeff(P,start);
			Tuple* C=list(n);
			var expo=new Array<sint>;
			std::vector<sint>& Se=A<sint>(expo).vec;
			Se.assign(exponent(P).begin()+start,exponent(P).end());
			for(uint i=0;i<n;i++){
				C->tuple[i+1]=CPE::Mul(k,new Integer(exponent(P,i+start)),coeff(P,i+start));
				Se[i]--;
			}
			return newCPE(variable(P),C,expo);
		}
		var normalpart(Kernel& k,const var& f){
			if(isNumber(f)){
				if(isZero(f))
					return f;
				return new Integer(1L);
			}
			else
				return DivExact(k,f,lc(f));
		}
		var unitpart(Kernel& k,const var& f){
			if(isNumber(f)){
				if(isZero(f))
					return new Integer(1L);
				return f;
			}
			else
				return lc(f);
		}
		var GcdQExt(Kernel& k,const var& a,const var& b){
			var c=normalpart(k,a),d=normalpart(k,b);
			var c1=new Integer(1L),c2=new Integer(0L),d1=c2,d2=c1;
			while(!isZero(d)){
				var temp=Div(k,c,d);
				var q=temp.tuple()[1];
				var r=temp.tuple()[2];
				var r1=CPE::Sub(k,c1,CPE::Mul(k,q,d1));
				var r2=CPE::Sub(k,c2,CPE::Mul(k,q,d2));
				c=d;
				c1=d1;
				c2=d2;
				d=r;
				d1=r1;
				d2=r2;
			}
			var g=normalpart(k,c);
			var s=CPE::DivExact(k,c1,CPE::Mul(k,unitpart(k,a),unitpart(k,c)));
			var t=CPE::DivExact(k,c2,CPE::Mul(k,unitpart(k,b),unitpart(k,c)));
			return list(g,s,t);
		}
		sint degree(Kernel& k,const var& P, const std::vector<var>& V,uint start){
			if(start>=V.size()||isNumber(P))
				return 0;
			const var& x=variable(P);
			const var& c=*(V.begin()+start);
			sint r=0;
			if(x==c)
				for(uint i=0;i<nnz(P);i++)
					r=std::max(r,exponent(P,i)+degree(k,coeff(P,i),V,start+1));
			else if(less_main(x,c))
				return degree(k,P,V,start+1);
			else//x is more main
				for(uint i=0;i<nnz(P);i++)
					r=std::max(r,degree(k,coeff(P,i),V,start));
			return r;
		}
		var Div(Kernel& k,const var&f_,const var&g_,const var& x){
			var f=f_,g=g_;
			if(!isCPE(f)||!isCPE(g))
				return list(list(f,list(g,new Integer(-1L),$.Power),$.Times),new Integer(0L));
			if(isNumber(g)){
				if(isZero(g))
					return list($.Infinity,new Integer(0L));
				return list(CPE::Mul(k,f,Number::Divide(g.object())),new Integer(0L));
			}
			if(isNumber(f)){//g is not number
				if(isZero(f))
					return list(f,f);
				return list(new Integer(0L),f);
			}
			
			f=to_main(k,f_,x);
			g=to_main(k,g_,x);
			if(variable(f)!=x){
				if(variable(g)!=x)
					return list(CRE::Div(k,f,g),new Integer(0L));
				return list(new Integer(0L),f);
			}
			else{
				if(variable(g)!=x)
					return list(newCPE(x,SparseArray::ScalarDiv(k,coeff_list(f),g)),new Integer(0L));
				var temp=SparseArray::Div(k,coeff_list(f),coeff_list(g));
				var q=temp.tuple()[1];
				var r=temp.tuple()[2];
				//normalize
				if(SparseArray::nnz(q)==0)//Zero is good
					q=new Integer(0L);
				else if(SparseArray::nnz(q)==1&&SparseArray::index(q,0)==0)
					q=SparseArray::data(q,0);
				else
					q=newCPE(x,q);
				if(SparseArray::nnz(r)==0)//Zero is good
					r=new Integer(0L);
				else if(SparseArray::nnz(r)==1&&SparseArray::index(r,0)==0)
					r=SparseArray::data(r,0);
				else
					r=newCPE(x,r);
				return list(q,r);
			}
		}
		var PseudoRemainder(Kernel& k,const var&f_,const var&g_,const var& x){
			//g_ is not Number
			var f=f_,g=g_;
			if(isNumber(g))
				return $.Fail;
			if(isNumber(f))
				return f;
			f=to_main(k,f_,x);
			g=to_main(k,g_,x);
			if(variable(f)!=x){
				if(variable(g)!=x)
					return CRE::Div(k,f,g);
				return f;
			}
			else{
				if(variable(g)!=x)
					return $.Fail;
				var r=SparseArray::PseudoRemainder(k,coeff_list(f),coeff_list(g));
				if(SparseArray::nnz(r)==0)
					return new Integer(0L);
				else if(SparseArray::nnz(r)==1&&SparseArray::index(r,0)==0)
					return SparseArray::data(r,0);
				else
					r=from_main(k,newCPE(x,r),x);
				return ppZdecompose(k,r).tuple()[2];
			}
		}
	}
}

using namespace nV;
CAPI void CVALUE(System, FromCPE)(Kernel& k, var& r, Tuple& x) {
	if(x.size==1)
		return;
	r=CPE::fromCPE(k,x[1]);
}
CAPI void CVALUE(System, ToCPE)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=2)
		return;
	r=CPE::toCPE(k,x[1]);
}
CAPI void CVALUE(System, ExpandFromCPE)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=2)
		return;
	r=CPE::ExpandFromCPE(k,x[1]);
}
#undef VALUE
#define VALUE(f) CVALUE(CPE,f)
CAPI void VALUE(Add)(Kernel& k, var& r, Tuple& x) {
	if(x.size==1)
		return;
	var c = x[1];
	for (uint i = 2; i < x.size; ++i)
		c = CPE::Add(k,c, x[i]);
	r = c;
}
CAPI void VALUE(Sub)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=3)
		return;
	r = CPE::Sub(k,x[1],x[2]);
}
CAPI void VALUE(Mul)(Kernel& k, var& r, Tuple& x) {
	if(x.size==1)
		return;
	var c = x[1];
	for (uint i = 2; i < x.size; ++i)
		c = CPE::Mul(k,c, x[i]);
	r = c;
}
CAPI void VALUE(DivExact)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=3)
		return;
	r =CPE::DivExact(k,CPE::toCPE(k,x[1]),CPE::toCPE(k,x[2])) ;
}
CAPI void VALUE(Div)(Kernel& k, var& r, Tuple& x) {
	if(x.size==3)
		r =CPE::Div(k,CPE::toCPE(k,x[1]),CPE::toCPE(k,x[2]));
	else if(x.size==4)
		r =CPE::Div(k,CPE::toCPE(k,x[1]),CPE::toCPE(k,x[2]),x[3]);
	else
		return;
}
CAPI void VALUE(PolynomialQuotientRemainder)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=4)
		return;
	r =CPE::Div(k,CPE::toCPE(k,x[1]),CPE::toCPE(k,x[2]),x[3]);
}
CAPI void VALUE(PolynomialQuotient)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=4)
		return;
	r = CPE::Div(k,CPE::toCPE(k,x[1]),CPE::toCPE(k,x[2]),x[3]);
	r = r.tuple()[1];
}
CAPI void VALUE(PolynomialRemainder)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=4)
		return;
	r = CPE::Div(k,CPE::toCPE(k,x[1]),CPE::toCPE(k,x[2]),x[3]);
	r = r.tuple()[2];
}
CAPI void VALUE(PseudoRemainder)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=4)
		return;
	r = CPE::PseudoRemainder(k,CPE::toCPE(k,x[1]),CPE::toCPE(k,x[2]),x[3]);
}

CAPI void VALUE(GcdQExt)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=3)
		return;
	r =CPE::GcdQExt(k,CPE::toCPE(k,x[1]),CPE::toCPE(k,x[2])) ;
}
CAPI void VALUE(Variables)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=2)
		return;
	CPE::vset temp=CPE::Variables(k,CPE::toCPE(k,x[1]));
	r = nV::list(temp.size(),temp.begin());
}
CAPI void VALUE(Coefficient)(Kernel& k, var& r, Tuple& x){
	if(x.size==3){
		var xx=CPE::toCPE(k,x[2]);
		if(isNumber(xx))
			r=CPE::ConstantTerm(k,CPE::toCPE(k,x[1]));
		else
			r=CPE::Coefficient(k,CPE::toCPE(k,x[1]),xx);
	}
	else if(x.size==4){
		r=CPE::Coefficient(k,CPE::toCPE(k,x[1]),x[2],Z(x[3]).toUI());
	}
	else
		return;
}
CAPI void VALUE(Square)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=2)
		return;
	r=CPE::Square(k,x[1]);
}
CAPI void VALUE(Power)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=3)
		return;
	if(!x[2].isObject($.Integer)){
		r=list(x[1],x[2],$.Power);
		return;
	}
	r=CPE::Power(k,x[1],Z(x[2]).toUI());
}
CAPI void VALUE(ModI)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=4)
		return;
	var* start=x[2].tuple().tuple;
	std::vector<var> I(start+1,start+x[2].tuple().size);
	std::sort(I.begin(),I.end());
	r=CPE::ModI(k,x[1],I,Z(x[3]).toUI());
}
CAPI void VALUE(Eval)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=4)
		return;
	r=CPE::Eval(k,x[1],x[2],x[3]);
}
CAPI void VALUE(ModP)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=3)
		return;
	r=CPE::ModP(k,x[1],x[2]);
}
CAPI void VALUE(Mod2P)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=3)
		return;
	r=CPE::Mod2P(k,x[1],x[2]);
}
CAPI void VALUE(Degree)(Kernel& k, var& r, Tuple& x) {
	if(x.size==2){
		var f=CPE::toCPE(k,x[1]);
		CPE::vset SV=CPE::Variables(k,f);
		std::vector<var> V(SV.begin(),SV.end());
		r=new Integer(CPE::degree(k,f,V));
	}
	else if(x.size==3){
		var f=CPE::toCPE(k,x[1]);
		if(x[2].isTuple($.List)){
			CPE::vset SV(x[2].tuple().tuple+1,x[2].tuple().tuple+x[2].tuple().size);
			std::vector<var> V(SV.begin(),SV.end());
			r=new Integer(CPE::degree(k,f,V));
		}
		else
			r=new Integer(CPE::degree(k,f,x[2]));
	}
	return;
}
CAPI void VALUE(ToMain)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=3)
		return;
	r=CPE::to_main(k,CPE::toCPE(k,x[1]),x[2]);
}
CAPI void VALUE(FromMain)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=3)
		return;
	r=CPE::from_main(k,CPE::toCPE(k,x[1]),x[2]);
}
CAPI void VALUE(Derivate)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=2)
		return;
	r=CPE::Derivate(k,CPE::toCPE(k,x[1]));
}
CAPI void VALUE(Collect)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=3)
		return;
	r=CPE::to_main(k,CPE::toCPE(k,x[1]),x[2]);
}
CAPI void VALUE(PPZ)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=2)
		return;
	r=CPE::ppZdecompose(k,CPE::toCPE(k,x[1]));
}
CAPI void VALUE(PolynomialGCD)(Kernel& k, var& r, Tuple& x) {
	if(x.size<3)
		return;
	r=CPE::toCPE(k,x[1]);
	for(uint i=2;i<x.size;i++)
		r=CPE::Gcd(k,r,CPE::toCPE(k,x[i]));
}
CAPI void VALUE(PolynomialLCM)(Kernel& k, var& r, Tuple& x) {
	if(x.size<3)
		return;
	r=CPE::toCPE(k,x[1]);
	for(uint i=2;i<x.size;i++)
		r=CPE::Lcm(k,r,CPE::toCPE(k,x[i]));
}
CAPI void VALUE(GroebnerBasis)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=2||!x[1].isTuple($.List))
		return;
	r=CPE::GroebnerBasis(k,CPE::variable(CPE::toCPE(k,x[1])).tuple());
}
CAPI void VALUE(CharacterSet)(Kernel& k, var& r, Tuple& x){
	if(x.size!=2||!x[1].isTuple($.List))
		return;
	r=CPE::CharacterSet(k,CPE::variable(CPE::toCPE(k,x[1])).tuple());
}
CAPI void VALUE(FactorList)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=2)
		return;
	r=CPE::FactorList(k,CPE::toCPE(k,x[1]));
}
CAPI void VALUE(FactorListZ)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=2)
		return;
	r=CPE::FactorListZ(k,CPE::toCPE(k,x[1]));
}
CAPI void VALUE(Factor)(Kernel& k, var& r, Tuple& x) {
	if(x.size!=2)
		return;
	r=CPE::Factor(k,CPE::toCPE(k,x[1]));
}
// CAPI void VALUE(SqfFactor)(Kernel& k, var& r, Tuple& x) {
// 	if(x.size!=2)
// 		return;
// 	r=CPE::FactorListZ_sqf(k,CPE::toCPE(k,x[1]));
// }
// CAPI void VALUE(BezoutQ)(Kernel& k, var& r, Tuple& x) {
// 	if(x.size!=2)
// 		return;
// 	r=CPE::BezoutQ(k,CPE::variable(CPE::toCPE(k,x[1])).tuple());
// }

#define Export(function)																						\
	CAPI void CVALUE(System,function)(Kernel& k, var& r, Tuple& x) {	\
		VALUE(function) (k,r,x);																				\
		r=CPE::fromCPE(k,r);																						\
	}
Export(Variables)
Export(Coefficient)
Export(Degree)
Export(GroebnerBasis)
Export(CharacterSet)
Export(PolynomialGCD)
Export(PolynomialLCM)
Export(FactorList)
Export(Factor)
Export(Collect)
Export(DivExact)
Export(Div)
Export(PolynomialRemainder)
Export(PolynomialQuotient)
Export(PolynomialQuotientRemainder)
Export(PseudoRemainder)
