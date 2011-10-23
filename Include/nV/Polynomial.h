#pragma once
#include "Interface.h"

#undef API
#ifdef _MSC_VER
#ifdef POLYNOMIAL_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif
#else
#define API
#endif

#undef ASSIGN
#define ASSIGN(f) CASSIGN(System_Polynomial,f)
#undef VALUE
#define VALUE(f) CVALUE(System_Polynomial,f)
#undef METHOD
#define METHOD(f,sig) CMETHOD(System_Polynomial,f,sig)

namespace nV {
  API extern sym $Polynomial,$CPE,$CRE,$SparseArray;
	
  API var Minus(Kernel&, const var&);
  API var Divide(Kernel&, const var&);
  API var Divide(Kernel&, const var&, const var&);
  API var Plus(Kernel&, const Tuple&);//合并同类项
  API var Plus(Kernel&, const var&, const var&);
  API var Times(Kernel&, const Tuple&);//单项式乘法
  API var Times(Kernel&, const var&, const var&);
  API var Expand(Kernel&, const var&);
  API var Mul(Kernel&, const var&, const var&);

// Compare.cpp
	API int Compare(Kernel&, const var&, const var&);

	// CPE.cpp
	namespace CPE{
		API var toCPE(Kernel& k, const var& x);
		API var fromCPE(Kernel& k, const var& x);
		API var ExpandFromCPE(Kernel& k, const var& x);
	}
	namespace CRE{
		API var toCRE(Kernel& k, const var& x);
		API var fromCRE(Kernel& k, const var& x);
		API var ExpandFromCRE(Kernel& k, const var& x);
	}
}
