#include "StdAfx.h"
#include "CRE.h"
namespace nV{
	namespace CPE{
		class CharacterSetZ{
			typedef std::map<var,uint> VMap;
		public:
			CharacterSetZ(Kernel& k,const Tuple& _f):kernel(k),f(_f){
				Name.push_back($.Null);
			}
			operator var() {
				uint n=f.size-1;
				if(n==0)
					return list(0);
				vset V=Variables(kernel,f[1]);
				for(uint i=2;i<f.size;i++){
					vset temp=Variables(kernel,f[i]);
					for(vset::iterator it=temp.begin();it!=temp.end();it++)
						V.insert(*it);
				}
				Name.insert(Name.end(),V.rbegin(),V.rend());
				for(uint i=1;i<Name.size();i++)
					Index[Name[i]]=i;

				std::vector<uint> bs;
				std::vector<var> pi(f.tuple+1,f.tuple+f.size);
				uint oldsize;
				int count=0;
				var r;
				for(uint i=pi.size()-1;i+1>0;i--){//Note i+1>0 to avoid -1>0
					if(isNumber(pi[i])){
						if(!cmpD(pi[i].object(), 0.0))//is zero
							pi.erase(pi.begin()+i);
						else
							return list(new Integer(1L));
					}
				}
				if(pi.size()==0)
					return list(0);
				
				while(true){
					count++;
					BasicSetZ(bs,pi);
					oldsize=pi.size();
					for(uint i=0;i<oldsize;i++){
						if(std::find(bs.begin(),bs.end(),i)!=bs.end())//i is in bs
							continue;
						r=mod_ascending_set(pi[i],pi,bs);
						if(isNumber(r)){
							if(!cmpD(r.object(), 0.0))//r is zero
								continue;
							else
								return list(new Integer(1L));
						}
						bool flag=1;
						for(uint j=0;j<pi.size();j++){
							if(r==pi[j]){
								flag=0;
								break;
							}
						}
						if(flag){
							pi.push_back(r);
						}
					}
					if(pi.size()==oldsize)
						break;
				}
				Tuple* cs=list(bs.size());
				for(uint i=0;i<bs.size();i++) {
					cs->tuple[i+1]=pi[bs[i]];
				}
				return cs;
			}
		private:
			VMap Index;
			std::vector<var> Name;
			Kernel& kernel;
			const Tuple& f;

			uint cl(const var f){
				VMap::iterator it;
				it= Index.find(variable(f));
				if(it==Index.end())
					return 0;
				else
					return it->second;
			}
			sint cmp(const var& f1,const var& f2){
				uint cl1=cl(f1),cl2=cl(f2);
				if(cl1<cl2)return -1;
				if(cl1>cl2)return 1;
				sint delta=degree(f1)-degree(f2);
				if(delta>0)
					return 1;
				else if(delta==0)
					return 0;
				else
					return -1;
			}
			bool reduced(const var& f,const var& g){
				uint p=cl(g);
				return degree(kernel,f,Name[p])<degree(g);
			}
			var mod(const var& f,const var& g){
				return CPE::PseudoRemainder(kernel,f,g,variable(g));
			}
			var mod_ascending_set(const var& f,const std::vector<var>& polylist,const std::vector<uint> as){
				var r=f;
				for(sint i=as.size()-1;i>=0;i--)
					r=mod(r,polylist[as[i]]);
				return r;
			}
			uint minimUm(const std::vector<var> & polylist,const std::vector<uint> & mark){
				uint r=0;
				for(uint i=1;i<mark.size();i++)
					if(cmp(polylist[mark[r]],polylist[mark[i]])==1)
						r=i;
				return r;
			}
			void BasicSetZ(std::vector<uint> & bs, const std::vector<var> & polylist){
				//Note:ensure polylist contain no number
				bs.resize(0);
				std::vector<uint> mark;
				mark.resize(polylist.size());
				for(size_t i=0;i<mark.size();i++)
					mark[i]=i;
				uint m,index;
				index=minimUm(polylist,mark);
				m=mark[index];
				bs.push_back(m);
				if(cl(polylist[m])==0)
					return ;
				mark.erase(mark.begin()+index);//B1
				while(1)
					{
						for(size_t i=0;i<mark.size();i++)
							{
								if(!reduced(polylist[mark[i]],polylist[m]))
									{
										mark.erase(mark.begin()+i);
										i--;
									}
							}//A2
						if(mark.size()==0)break;
						index=minimUm(polylist,mark);
						m=mark[index];
						bs.push_back(m);
						mark.erase(mark.begin()+index);
					}
				return ;
			}
			var ReduceByAscendingSet(const var & f,const std::vector<var> & polylist)
			{
				var r=f;
				for(int i=polylist.size()-1;i>=0;i--){
					r=mod(r,polylist[i]);
				}
				return r;
			}
		};
		var CharacterSet(Kernel& k,const Tuple& f){
			uint n=f.size-1;
			Tuple* ff=list(n);
			for(uint i=0;i<n;i++){
				sym type=coeff_type(f[i+1]);
				if(type==$.Integer)
					ff->tuple[i+1]=f[i+1];
				else if(type==$.Rational)
					ff->tuple[i+1]=integralize(k,f[i+1]).tuple()[2];
				else{
					wcerr<<"all polynomials must be Exact\n";
					return $.Fail;
				}
			}
			return CharacterSetZ(k,*ff);
		}
	}
}
