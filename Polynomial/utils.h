#include <nV/Interface.h>

namespace nV{
	inline bool isZero(const var& x){
		return isNumber(x)&&(!cmpD(x.object(), 0.0));
	}
	inline bool isUnit(const var& x){
		return isNumber(x)&&(!cmpD(x.object(), 1.0));
	}
	Tuple* insert(const Tuple&x,uint k,const var& a);//0<=k<x.size
	Tuple* replace(const Tuple&x,uint k,const var& a);
	Tuple* erase(const Tuple&x,uint k);
	Tuple* cons(const var& a,const Tuple& x);
	var GCD(const std::vector<var>& V);//return gcd of all numbers in V
	var GCD(const std::vector<var>& V1,const std::vector<var>& V2);
	var BinomialTable(Kernel& K, uint n);
	class NChooseKEnumeator{
		uint n,k,h;
		bool end;
		std::vector<uint> S;
	public:
		inline const std::vector<uint>& vec(){return S;}
		inline uint size(){return k;}
		inline uint operator[](uint i){return S[i];}
		NChooseKEnumeator(uint _n,uint _k);
		inline bool is_end(){
			return end;
		}
		void next();
	};
	var GCD(const Object&,const Object&);
}
