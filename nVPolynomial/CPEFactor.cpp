#include "common.h"
#include "CRE.h"
#include "mU_wrap.h"
#include <nV/utils.h>

namespace nV{
	namespace CPE{
		var pp(Kernel& kernel,const var& f){
			var d=Gcd(kernel,coeff(f,0),coeff(f,1));
			for(uint i=2;i<nnz(f);i++)
				d=Gcd(kernel,d,coeff(f,i));
			return DivExact(kernel,f,d);
		}
		bool is_sqf(Kernel& kernel,const var& P);
		class BezoutQ{
			uint n;
			Kernel& kernel;
			Tuple* a;//polynomial list
			std::vector<var> bezout;
			var x;
			sint upperdegree;
		public:
			operator var(){
				return bezout[0];
			}
			uint size(){
				return n;
			}
			BezoutQ(Kernel& _kernel,const Tuple& _a,sint _upperdegree=-1):kernel(_kernel),a(_a.clone()),upperdegree(_upperdegree){
				n=a->size-1;//assume n>=2,a are pairwise relative prime
				x=variable(a->tuple[1]);//assume all are univariate polynomial
				if(upperdegree==-1){
					upperdegree=0;
					for(uint i=1;i<=n;i++)
						upperdegree+=degree(a->tuple[i]);
				}
				Tuple* s=list(n);
				Tuple* q=list(n-1);
				Tuple* beta=list(n-1);
				q->tuple[n-1]=a->tuple[n];
				for(uint i=n-2;i>=1;i--){
					q->tuple[i]=CPE::Mul(kernel,a->tuple[i+1],q->tuple[i+1]);
				}
				var temp=GcdQExt(kernel,a->tuple[1],q->tuple[1]);
				beta->tuple[1]=temp.tuple()[2];
				s->tuple[1]=temp.tuple()[3];
				for(uint j=2;j<=n-1;j++){
					temp=GcdQExt(kernel,a->tuple[j],q->tuple[j]);
					beta->tuple[j]=temp.tuple()[2];
					s->tuple[j]=temp.tuple()[3];
					beta->tuple[j]=CPE::Mul(kernel,beta->tuple[j],beta->tuple[j-1]);
					s->tuple[j]=CPE::Mul(kernel,s->tuple[j],beta->tuple[j-1]);
					temp=Div(kernel,s->tuple[j],a->tuple[j]);
					var qq=temp.tuple()[1];
					var rr=temp.tuple()[2];
					s->tuple[j]=rr;
					beta->tuple[j]=CPE::Add(kernel,beta->tuple[j],CPE::Mul(kernel,qq,q->tuple[j]));
				}
				s->tuple[n]=beta->tuple[n-1];
				bezout.push_back(s);
			}
			var operator[](uint i){
				if(i>=bezout.size())
			 		bezout.resize(i+1);
				if(bezout[i].isNull()){
					bezout[i]=list(n);
					var h=newCPE(x,i);
					if(i<upperdegree){
						for(uint j=1;j<=n;j++){
							var temp=CPE::Mul(kernel,bezout[0].tuple()[j],h);
							temp=Div(kernel,temp,a->tuple[j]);
							bezout[i].tuple()[j]=temp.tuple()[2];
						}
					}
					else{
						var q=new Integer(0L);
						for(uint j=1;j<=n-1;j++){
							var temp=CPE::Mul(kernel,bezout[0].tuple()[j],h);
							temp=Div(kernel,temp,a->tuple[j]);
							bezout[i].tuple()[j]=temp.tuple()[2];
							q=CPE::Add(kernel,q,temp.tuple()[1]);
						}
						bezout[i].tuple()[n]=CPE::Add(kernel,CPE::Mul(kernel,bezout[0].tuple()[n],h),CPE::Mul(kernel,q,a->tuple[n]));
					}
				}
				return bezout[i];
			}
		};
		class EZ{
			Kernel& kernel;
			const var& f;
			std::vector<var> I;
			std::vector<var> A;
			var f2;//f after shift
			var x;
			uint maxdegree;
		public:
			EZ(Kernel& _kernel,const var& _f):kernel(_kernel),f(_f){}
			void shift(){
				f2=f;
				for(uint i=0;i<I.size();i++)
					f2=Eval(kernel,f2,I[i],CPE::Add(kernel,newCPE(I[i]),A[i]));
			}
			Tuple* shift_back(const Tuple& a){
				uint n=a.size-1;
				Tuple* r=a.clone();
				for(uint i=0;i<I.size();i++)
					for(uint j=1;j<=n;j++)
						r->tuple[j]=Eval(kernel,r->tuple[j],I[i],CPE::Sub(kernel,newCPE(I[i]),A[i]));
				return r;
			}
			Tuple* combine(const Tuple& a){
				std::vector<var> r;
				uint n=a.size-1;
				std::vector<uint> T;
				for(uint i=1;i<=n;i++)
					T.push_back(i);
				uint s=1;
				var h=f;
				var hh=CPE::Mul(kernel,h,lc(h));
				while(2*s<=n){
					NChooseKEnumeator it(n,s);
					bool findone=false;
					while(!it.is_end()){
						var g=a[T[it[0]]];
						for(uint i=1;i<it.size();i++)
							g=ModI(kernel,CPE::Mul(kernel,g,a[T[it[i]]]),I,maxdegree);
						var gg=ModI(kernel,CPE::Mul(kernel,DivExact(kernel,lc(h),lc(g)),g),I,maxdegree);
						if(DivExact(kernel,hh,gg)!=$.False){
							var factor=pp(kernel,gg);
							r.push_back(factor);
							h=DivExact(kernel,h,factor);
							hh=CPE::Mul(kernel,h,lc(h));
							std::vector<uint> temp;
							for(sint i=it.size()-1;i>=0;i--){
								T.erase(T.begin()+it[i]);
							}
							n=T.size();
							findone=true;
							it.next();
							break;
						}
						it.next();
					}
					if(!findone||it.is_end())
						s++;
				}
				r.push_back(h);
				return nV::list(r.size(),r.begin());
			}
			var replace_coeff(var e,BezoutQ& Bezout){
				//assume e's main variable is x
				uint n=Bezout.size();
				Tuple* r=list(n);
				if(variable(e)==x){
					for(uint j=1;j<=n;j++)
						r->tuple[j]=new Integer(0L);
					for(uint i=0;i<nnz(e);i++){
						for(uint j=1;j<=n;j++){
							r->tuple[j]=CPE::Add(kernel,r->tuple[j],CPE::Mul(kernel,coeff(e,i),Bezout[exponent(e,i)].tuple()[j]));
						}
					}
				}
				else
					for(uint j=1;j<=n;j++){
						r->tuple[j]=CPE::Mul(kernel,e,Bezout[0].tuple()[j]);
					}
				return r;
			}
			operator var(){
				//var r1=sqfZ(kernel,f);
				vset V=Variables(kernel,f);
				if(V.size()==1){//Use UniFacZ and wrap to CPE
					x=*V.begin();
					var temp=Polynomial::UniFacZ(kernel,ToFullCL(kernel,f));
					uint n=temp.tuple().size-2;//the coefficient must be 1
					Tuple* result=list(n);
					for(uint i=0;i<n;i++){
						result->tuple[i+1]=FromFullCL(kernel,x,temp.tuple()[i+2].tuple()[1]);
					}
					return result;
				}
				x=*V.begin();
				V.erase(V.begin());
				I.assign(V.begin(),V.end());//Choose evaluation point
				A.reserve(I.size());
				var ff=choose_evaluation_point();
				var faclist=Polynomial::UniFacZ(kernel,ToFullCL(kernel,ff));
				uint num_factor=faclist.tuple().size-2;
				if(num_factor==1)
					return list(f);
				maxdegree=degree(kernel,f,I)+1;
				Tuple* a=list(num_factor);
				a->tuple[1]=CPE::Mul(kernel,faclist.tuple()[1],FromFullCL(kernel,x,faclist.tuple()[2].tuple()[1]));
				for(uint i=2;i<=num_factor;i++)
					a->tuple[i]=FromFullCL(kernel,x,faclist.tuple()[i+1].tuple()[1]);
				BezoutQ Bezout(kernel,*a,degree(f));
				shift();
				uint k=1;
				var e=CPE::Sub(kernel,f2,ff);
				bool exact=false;
				while(k<maxdegree){
					e=ModI(kernel,e,I,k+1);
					var Delta=replace_coeff(e,Bezout);
					for(uint i=1;i<=num_factor;i++){
						a->tuple[i]=CPE::Add(kernel,a->tuple[i],Delta.tuple()[i]);
					}
					k++;//Now,Factor is exact mod I^k
					var temp=a->tuple[1];
					for(uint i=2;i<=num_factor;i++)
						temp=CPE::Mul(kernel,temp,a->tuple[i]);
					e=CPE::Sub(kernel,f2,temp);
					if(isZero(e)){
						exact=true;
						break;
					}
				}
				a=shift_back(*a);
				if(exact)
					return a;
				return combine(*a);
			}
			var choose_evaluation_point(){
				//evaluation points stored in A,return the image of mod
				var r=f;
				uint deg=degree(f);
				for(uint i=0;i<I.size();i++){
					sint j=0;
					var j_var;
					var temp;
					while(true){
						j_var=new Integer(j);
						temp=Eval(kernel,r,I[i],j_var);
						if(degree(kernel,temp,x)==deg&&is_sqf(kernel,temp))
							break;
						if(j<=0)
							j=1-j;
						else
							j=-j;
					}
					r=temp;
					A.push_back(j_var);
				}
				return r;
			}
		};
		class EEZ{
			Kernel& kernel;
			const var& f;
			var x;
		public:
			EEZ(Kernel& _kernel,const var& _f):kernel(_kernel),f(_f){}
			operator var(){
				vset V=Variables(kernel,f);
				if(V.size()==1){//Use UniFacZ and wrap to CPE
					var x=*V.begin();
					var temp=Polynomial::UniFacZ(kernel,ToFullCL(kernel,f));
					uint n=temp.tuple().size-2;//the coefficient must be 1
					Tuple* result=list(n);
					for(uint i=0;i<n;i++){
						result->tuple[i+1]=FromFullCL(kernel,x,temp.tuple()[i+2].tuple()[1]);
					}
					return result;
				}
				x=*V.begin();
				// std::vector<var> I(V.begin(),V.end());//Choose evaluation point
				// std::vector<var> A;
				// A.reserve(I.size());
				// var ff=choose_evaluation_point(I,A);
				// var faclist=Polynomial::UniFacZ(kernel,ToFullCL(kernel,ff));
				// uint num_factor=faclist.tuple().size-2;
				// if(num_factor==1)
				// 	return list(f);
				// return list(f);
			}
		};
		bool is_sqf(Kernel& kernel,const var& a){
			//P is not number,P is pp,test sqf in main variable
			var b=Derivate(kernel,a);
			var c=GcdZ(kernel,a,b);
			return isNumber(c);
		}
		var FactorListZ_sqf(Kernel& kernel,const var& a){
			// Note a is assumed as pp
			std::vector<var> C;
			std::vector<uint> E;
			uint i=1;
			var b=Derivate(kernel,a);
			var c=GcdZ(kernel,a,b);
			var w;
			if(isNumber(c))
				w=a;
			else{
				w=DivExact(kernel,a,c);
				var y=DivExact(kernel,b,c);
				var z=CPE::Sub(kernel,y,Derivate(kernel,w));
				while(!isZero(z)){
					var g=Gcd(kernel,w,z);
					if(!isNumber(g)){
						C.push_back(g);
						E.push_back(i);
					}
					i++;
					w=DivExact(kernel,w,g);
					y=DivExact(kernel,z,g);
					z=CPE::Sub(kernel,y,Derivate(kernel,w));
				}
			}
			C.push_back(w);
			E.push_back(i);

			Tuple* r=list(C.size());
			for(uint j=0;j<C.size();j++){
				r->tuple[j+1]=list(C[j],new Integer(E[j]));
			}
			return r;
		}

		var FactorListZ_pp(Kernel& kernel,const var& f){
			var r1=FactorListZ_sqf(kernel,f);
			std::vector<var> C;
			std::vector<var> E;
			for(uint i=0;i<r1.tuple().size-1;i++){
				var exponent=r1.tuple()[i+1].tuple()[2];
				var temp=EZ(kernel,r1.tuple()[i+1].tuple()[1]);
				for(uint j=0;j<temp.tuple().size-1;j++){
					C.push_back(temp.tuple()[j+1]);
					E.push_back(exponent);
				}
			}
			Tuple* r=list(C.size());
			for(uint j=0;j<C.size();j++){
				r->tuple[j+1]=list(C[j],E[j]);
			}
			return r;
		}
		var FactorListZ(Kernel& kernel,const var& f){
			if(isNumber(f))
				//return list(f);
				return list(list(f,new Integer(1L)));
			var x=variable(f);
			if(nnz(f)==1){
				var temp=FactorListZ(kernel,coeff(f,0));
				Tuple* r=insert(temp.tuple(),2,list(newCPE(x),new Integer(exponent(f,0))));//Note index 0 is $.List
				return r;
			}
			else if(exponent(f,0)>0){//nnz(f)>1
				var temp=FactorListZ(kernel,Shift(kernel,f,-exponent(f,0)));
				Tuple* r=insert(temp.tuple(),2,list(newCPE(x),new Integer(exponent(f,0))));//Note index 0 is $.List
				return r;
			}
			var d=GcdZ(kernel,coeff(f,0),coeff(f,1));
			for(uint i=2;i<nnz(f);i++)
				d=Gcd(kernel,d,coeff(f,i));
			var r1=FactorListZ_pp(kernel,DivExact(kernel,f,d));
			var r2=FactorListZ(kernel,d);
			Tuple* r=list(r1.tuple().size+r2.tuple().size-2);//note 3
			r->tuple[1]=r2.tuple()[1];
			for(uint i=1;i<r1.tuple().size;i++)
				r->tuple[i+1]=r1.tuple()[i];
			for(uint i=2;i<r2.tuple().size;i++)
				r->tuple[r1.tuple().size-1+i]=r2.tuple()[i];
			return r;
		}
		var FactorList(Kernel& kernel,const var& f0){
			var f;
			if(!isExact(f0)){
				kernel.logging(__FUNCTIONW__) << _W("operand is not exact number,conversion is applied!") << endl;;
				f=exact(f0);
			}
			else
				f=f0;
			var lcm=LCM_of_coeff_den(f);
			var ff=CPE::Mul(kernel,lcm,f);
			var r=FactorListZ(kernel,ff);
			//r.tuple()[1]=r.tuple()[1].object()/lcm.object();
			r.tuple()[1].tuple()[1]=r.tuple()[1].tuple()[1].object()/lcm.object();
			return r;
		}
		var Factor(Kernel& kernel,const var& f){
			var r1=FactorList(kernel,f);
			uint n=r1.tuple().size-1;
			var constant=r1.tuple()[1].tuple()[1];
			if(n==1)
				return constant;
			uint offset=0;
			Tuple* r=NULL;
			if(cmpD(constant.object(),1.0)==0){
				if(n==2){
					var fi=CPE::fromCPE(kernel,r1.tuple()[2].tuple()[1]);
					var ei=r1.tuple()[2].tuple()[2];
					if(cmpD(ei.object(),1.0)==0)
						return fi;
					else
						return list(fi,ei,$.Power);
				}
				offset=1;
				r=list(n-1,$.Times);
			}
			else{
				r=list(n,$.Times);
				r->tuple[1]=constant;
			}
			for(uint i=2;i<=n;i++){
				var fi=CPE::fromCPE(kernel,r1.tuple()[i].tuple()[1]);
				var ei=r1.tuple()[i].tuple()[2];
				if(cmpD(ei.object(),1.0)==0)
					r->tuple[i-offset]=fi;
				else
					r->tuple[i-offset]=list(fi,ei,$.Power);
			}
			return r;
		}
	}
}
