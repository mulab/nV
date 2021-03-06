(* ::Package:: *)

(************************************************************************)
(* This file was generated automatically by the Mathematica front end.  *)
(* It contains Initialization cells from a Notebook file, which         *)
(* typically will have the same name as this file except ending in      *)
(* ".nb" instead of ".m".                                               *)
(*                                                                      *)
(* This file is intended to be loaded into the Mathematica kernel using *)
(* the package loading commands Get or Needs.  Doing so is equivalent   *)
(* to using the Evaluate Initialization Cells menu command in the front *)
(* end.                                                                 *)
(*                                                                      *)
(* DO NOT EDIT THIS FILE.  This entire file is regenerated              *)
(* automatically each time the parent Notebook file is saved in the     *)
(* Mathematica front end.  Any changes you make to this file will be    *)
(* overwritten.                                                         *)
(************************************************************************)



(* ::Code:: *)
Int[(c_.*(a_.+b_.*x_)^n_.)^m_,x_Symbol] :=
  (a+b*x)*(c*(a+b*x)^n)^m*Log[a+b*x]/b /;
FreeQ[{a,b,c,m,n},x] && ZeroQ[m*n+1]


(* ::Code:: *)
Int[(c_.*(a_.+b_.*x_)^n_)^m_,x_Symbol] :=
  (a+b*x)*(c*(a+b*x)^n)^m/(b*(m*n+1)) /;
FreeQ[{a,b,c,m,n},x] && NonzeroQ[m*n+1]


(* ::Code:: *)
Int[(a_.*x_^m_.+b_.*x_^n_.)^p_,x_Symbol] :=
  Int[x^(m*p)*(a+b*x^(n-m))^p,x] /;
FreeQ[{a,b,m,n},x] && IntegerQ[p] && ZeroQ[m*p+1-n+m] && Not[IntegersQ[m,n]]


(* ::Code:: *)
Int[(a_.*x_^m_.+b_.*x_^n_.)^p_,x_Symbol] :=
  Int[(x^m*(a+b*x^(n-m)))^p,x] /;
FreeQ[{a,b,m,n},x] && FractionQ[p] && ZeroQ[m*p+1-n+m]


(* ::Code:: *)
Int[(a_.*x_^m_.+b_.*x_^n_.+c_.*x_^q_.)^p_,x_Symbol] :=
  Int[(x^m*(a+b*x^(n-m)+c*x^(q-m)))^p,x] /;
FreeQ[{a,b,c},x] && IntegersQ[m,n,q] && FractionQ[p] && m<=n<=q


(* ::Code:: *)
Int[u_.*x_^m_./(a_.*x_^n_.+b_.*x_^p_),x_Symbol] :=
  Int[u*x^(m-n)/(a+b*x^(p-n)),x] /;
FreeQ[{a,b},x] && FractionQ[{m,n,p}] && 0<n<=p


(* ::Code:: *)
Int[(u_.*x_^m_.+v_)/(a_.*x_^n_.+b_.*x_^p_),x_Symbol] :=
  Int[u*x^(m-n)/(a+b*x^(p-n)),x] + Int[v/(a*x^n+b*x^p),x] /;
FreeQ[{a,b},x] && FractionQ[{m,n,p}] && 0<n<=p && Not[PolynomialQ[v,x]]


(* ::Code:: *)
If[ShowSteps,
Int[x_^m_.*u_,x_Symbol] :=
  ShowStep["","Int[x^m*(u+v+\[CenterEllipsis]),x]","Int[x^m*u+x^m*v+\[CenterEllipsis],x]",Hold[
  Int[Map[Function[x^m*#],u],x]]] /;
SimplifyFlag && FreeQ[m,x] && SumQ[u],

Int[x_^m_.*u_,x_Symbol] :=
  Int[Map[Function[x^m*#],u],x] /;
FreeQ[m,x] && SumQ[u]]


(* ::Code:: *)
Int[1/(x_*Sqrt[a_+b_.*x_^n_.]),x_Symbol] :=
  -2*ArcTanh[Sqrt[a+b*x^n]/Rt[a,2]]/(n*Rt[a,2]) /;
FreeQ[{a,b,n},x] && PosQ[a]


(* ::Code:: *)
Int[1/(x_*Sqrt[a_+b_.*x_^n_.]),x_Symbol] :=
  2*ArcTan[Sqrt[a+b*x^n]/Rt[-a,2]]/(n*Rt[-a,2]) /;
FreeQ[{a,b,n},x] && NegQ[a]


(* ::Code:: *)
Int[(a_+b_.*x_^n_.)^p_/x_,x_Symbol] :=
  (a+b*x^n)^p/(n*p) +
  Dist[a,Int[(a+b*x^n)^(p-1)/x,x]] /;
FreeQ[{a,b,n},x] && FractionQ[p] && p>0


(* ::Code:: *)
Int[(a_+b_.*x_^n_.)^p_/x_,x_Symbol] :=
  -(a+b*x^n)^(p+1)/(a*n*(p+1)) +
  Dist[1/a,Int[(a+b*x^n)^(p+1)/x,x]] /;
FreeQ[{a,b,n},x] && FractionQ[p] && p<-1


(* ::Code:: *)
Int[1/((a_.+b_.*x_)*Sqrt[c_.+d_.*x_]),x_Symbol] :=
  -2*ArcTanh[Sqrt[c+d*x]/Rt[(b*c-a*d)/b,2]]/(b*Rt[(b*c-a*d)/b,2]) /;
FreeQ[{a,b,c,d},x] && PosQ[(b*c-a*d)/b]


(* ::Code:: *)
Int[1/((a_.+b_.*x_)*Sqrt[c_.+d_.*x_]),x_Symbol] :=
  2*ArcTan[Sqrt[c+d*x]/Rt[(a*d-b*c)/b,2]]/(b*Rt[(a*d-b*c)/b,2]) /;
FreeQ[{a,b,c,d},x] && NegQ[(b*c-a*d)/b]


(* ::Code:: *)
Int[1/(Sqrt[a_+b_.*x_]*Sqrt[c_+b_.*x_]),x_Symbol] :=
  ArcCosh[b*x/a]/b /;
FreeQ[{a,b,c},x] && ZeroQ[a+c] && PositiveQ[a]


(* ::Code:: *)
Int[1/(Sqrt[a_+b_.*x_]*Sqrt[c_+d_.*x_]),x_Symbol] :=
  ArcSin[(a-c+2*b*x)/(a+c)]/b /;
FreeQ[{a,b,c,d},x] && ZeroQ[b+d] && PositiveQ[a+c]


(* ::Code:: *)
Int[1/(Sqrt[a_.+b_.*x_]*Sqrt[c_.+d_.*x_]),x_Symbol] :=
  2/(Rt[b,2]*Sqrt[d])*ArcSinh[Rt[b,2]*Sqrt[c+d*x]/Sqrt[a*d-b*c]] /;
FreeQ[{a,b,c,d},x] && NonzeroQ[a*d-b*c] && PositiveQ[d/(a*d-b*c)] && Not[NegativeQ[a*d-b*c]] && PosQ[b]


(* ::Code:: *)
Int[1/(Sqrt[a_.+b_.*x_]*Sqrt[c_.+d_.*x_]),x_Symbol] :=
  2/(Rt[-b,2]*Sqrt[d])*ArcSin[Rt[-b,2]*Sqrt[c+d*x]/Sqrt[a*d-b*c]] /;
FreeQ[{a,b,c,d},x] && NonzeroQ[a*d-b*c] && PositiveQ[d/(a*d-b*c)] && Not[NegativeQ[a*d-b*c]] && NegQ[b]


(* ::Code:: *)
Int[1/(Sqrt[a_.+b_.*x_]*Sqrt[c_.+d_.*x_]),x_Symbol] :=
  2/Rt[b*d,2]*ArcTanh[Rt[b*d,2]*Sqrt[a+b*x]/(b*Sqrt[c+d*x])] /;
FreeQ[{a,b,c,d},x] && NonzeroQ[a*d-b*c] && PosQ[b*d]


(* ::Code:: *)
Int[1/(Sqrt[a_.+b_.*x_]*Sqrt[c_.+d_.*x_]),x_Symbol] :=
  2/Rt[-b*d,2]*ArcTan[Rt[-b*d,2]*Sqrt[a+b*x]/(b*Sqrt[c+d*x])] /;
FreeQ[{a,b,c,d},x] && NonzeroQ[a*d-b*c] && NegQ[b*d]


(* ::Code:: *)
Int[(a_.+b_.*x_)^m_*(c_+d_.*x_)^m_,x_Symbol] :=
  Int[(a*c+(a*d+b*c)*x+b*d*x^2)^m,x] /;
FreeQ[{a,b,c,d},x] && FractionQ[m] && ZeroQ[b+d] && PositiveQ[a+c]


(* ::Code:: *)
Int[(a_.+b_.*x_)^m_.*(c_.+d_.*x_)^n_.,x_Symbol] :=
  Dist[(d/b)^n,Int[(a+b*x)^(m+n),x]] /;
FreeQ[{a,b,c,d,m},x] && ZeroQ[b*c-a*d] && Not[IntegerQ[m]] && IntegerQ[n]


(* ::Code:: *)
Int[(a_.+b_.*x_)^m_*(c_.+d_.*x_)^n_,x_Symbol] :=
  (a+b*x)^(m+1)*(c+d*x)^n*Log[a+b*x]/b /;
FreeQ[{a,b,c,d,m,n},x] && ZeroQ[b*c-a*d] && Not[IntegerQ[m]] && Not[IntegerQ[n]] && ZeroQ[m+n+1] && 
(LeafCount[a+b*x]<LeafCount[c+d*x] || LeafCount[a+b*x]==LeafCount[c+d*x] && PosQ[a])


(* ::Code:: *)
Int[(a_.+b_.*x_)^m_*(c_.+d_.*x_)^n_,x_Symbol] :=
  (a+b*x)^(m+1)*(c+d*x)^n/(b*(m+n+1)) /;
FreeQ[{a,b,c,d,m,n},x] && ZeroQ[b*c-a*d] && Not[IntegerQ[m]] && Not[IntegerQ[n]] && 
NonzeroQ[m+n+1]


(* ::Code:: *)
Int[(a_.+b_.*x_)^m_.*(c_.+d_.*x_.)^n_,x_Symbol] :=
  -(a+b*x)^(m+1)*(c+d*x)^(n+1)/((n+1)*(b*c-a*d)) /;
FreeQ[{a,b,c,d,m,n},x] && NonzeroQ[b*c-a*d] && ZeroQ[m+n+2] && NonzeroQ[n+1]


(* ::Code:: *)
Int[(c_.+d_.*x_)^n_./(a_.+b_.*x_),x_Symbol] :=
  (c+d*x)^n/(b*n) +
  Dist[(b*c-a*d)/b,Int[(c+d*x)^(n-1)/(a+b*x),x]] /;
FreeQ[{a,b,c,d},x] && NonzeroQ[b*c-a*d] && FractionQ[n] && n>0


(* ::Code:: *)
Int[(a_.+b_.*x_)^m_*(c_.+d_.*x_),x_Symbol] :=
  (a+b*x)^(m+1)*(c+d*x)/(b*(m+2)) +
  Dist[(b*c-a*d)/(b*(m+2)),Int[(a+b*x)^m,x]] /;
FreeQ[{a,b,c,d,m},x] && NonzeroQ[b*c-a*d] && Not[IntegerQ[m]]


(* ::Code:: *)
Int[(a_.+b_.*x_)^m_*(c_.+d_.*x_)^n_,x_Symbol] :=
  (a+b*x)^(m+1)*(c+d*x)^n/(b*(m+n+1)) +
  Dist[n*(b*c-a*d)/(b*(m+n+1)),Int[(a+b*x)^m*(c+d*x)^(n-1),x]] /;
FreeQ[{a,b,c,d,m},x] && NonzeroQ[b*c-a*d] && NonzeroQ[m+n+1] && RationalQ[n] && n>0 &&
Not[IntegerQ[m]] && (IntegerQ[n] || FractionQ[m] && (n<=m || -1<=m<0))


(* ::Code:: *)
Int[(a_.+b_.*x_)^m_.*(c_.+d_.*x_)^n_,x_Symbol] :=
  -(a+b*x)^(m+1)*(c+d*x)^(n+1)/((n+1)*(b*c-a*d)) +
  Dist[b*(m+n+2)/((n+1)*(b*c-a*d)),Int[(a+b*x)^m*(c+d*x)^(n+1),x]] /;
FreeQ[{a,b,c,d,m},x] && NonzeroQ[b*c-a*d] && RationalQ[n] && n<-1 && Not[IntegersQ[m,n]] &&
(Not[RationalQ[m]] || n>=m || -1<=m<0)


(* ::Code:: *)
Int[(a_.+b_.*x_)^m_.*(c_.+d_.*x_)^n_,x_Symbol] :=
  -(a+b*x)^(m+1)*(c+d*x)^(n+1)/((n+1)*(b*c-a*d)) +
  Dist[b*(m+n+2)/((n+1)*(b*c-a*d)),Int[(a+b*x)^m*(c+d*x)^(n+1),x]] /;
FreeQ[{a,b,c,d,m,n},x] && NonzeroQ[b*c-a*d] && NonzeroQ[n+1] && Not[RationalQ[n]] && 
RationalQ[m+n] && Simplify[m+n]<-1


(* ::Code:: *)
Int[(a_.+b_.*x_)*(c_.+d_.*x_)^n_,x_Symbol] :=
  (a+b*x)*(c+d*x)^(n+1)/(d*(n+1)) -
  Dist[b/(d*(n+1)),Int[(c+d*x)^(n+1),x]] /;
FreeQ[{a,b,c,d,n},x] && Not[IntegerQ[n]]


(* ::Code:: *)
Int[(a_.+b_.*x_)^m_*(c_.+d_.*x_)^n_,x_Symbol] :=
  (a+b*x)^m*(c+d*x)^(n+1)/(d*(n+1)) -
  Dist[b*m/(d*(n+1)),Int[(a+b*x)^(m-1)*(c+d*x)^(n+1),x]] /;
FreeQ[{a,b,c,d},x] && RationalQ[{m,n}] && Not[IntegersQ[m,n]] && m>0 && n<-1


(* ::Code:: *)
Int[(a_.+b_.*x_)^m_/(c_+d_.*x_),x_Symbol] :=
  Module[{p=Denominator[m]},
  Dist[p,Subst[Int[x^(m*p+p-1)/(b*c-a*d+d*x^p),x],x,(a+b*x)^(1/p)]]] /;
FreeQ[{a,b,c,d},x] && RationalQ[m] && -1<m<0


(* ::Code:: *)
Int[(a_.+b_.*x_)^m_*(c_.+d_.*x_)^n_,x_Symbol] :=
  Module[{p=Denominator[m]},
  Dist[p,Subst[Int[x^(m*p+p-1)/(b-d*x^p),x],x,(a+b*x)^(1/p)/(c+d*x)^(1/p)]]] /;
FreeQ[{a,b,c,d},x] && RationalQ[{m,n}] && -1<m<0 && m+n==-1


(* ::Code:: *)
Int[1/(x_*Sqrt[a_+b_.*x_]*Sqrt[c_+d_.*x_]),x_Symbol] :=
  -2*ArcTanh[Rt[a*c,2]*Sqrt[a+b*x]/(a*Sqrt[c+d*x])]/Rt[a*c,2] /;
FreeQ[{a,b,c,d},x] && PosQ[a*c]


(* ::Code:: *)
Int[1/(x_*Sqrt[a_+b_.*x_]*Sqrt[c_+d_.*x_]),x_Symbol] :=
  -2*ArcTan[Rt[-a*c,2]*Sqrt[a+b*x]/(a*Sqrt[c+d*x])]/Rt[-a*c,2] /;
FreeQ[{a,b,c,d},x] && NegQ[a*c]


(* ::Code:: *)
Int[(a_+b_.*x_)^n_*(c_+d_.*x_)^n_/x_,x_Symbol] :=
  (a+b*x)^n*(c+d*x)^n/(2*n) +
  Dist[a*c,Int[(a+b*x)^(n-1)*(c+d*x)^(n-1)/x,x]] /;
FreeQ[{a,b,c,d},x] && FractionQ[n] && n>0 && ZeroQ[a*d+b*c]


(* ::Code:: *)
Int[(a_+b_.*x_)^n_*(c_+d_.*x_)^n_/x_,x_Symbol] :=
  (a+b*x)^n*(c+d*x)^n/(2*n) +
  Dist[(a*d+b*c)/2,Int[(a+b*x)^(n-1)*(c+d*x)^(n-1),x]] +
  Dist[a*c,Int[(a+b*x)^(n-1)*(c+d*x)^(n-1)/x,x]] /;
FreeQ[{a,b,c,d},x] && FractionQ[n] && n>0 && NonzeroQ[a*d+b*c]


(* ::Code:: *)
Int[(a_+b_.*x_)^n_*(c_+d_.*x_)^n_/x_,x_Symbol] :=
  -(a+b*x)^(n+1)*(c+d*x)^(n+1)/(2*a*c*(n+1)) +
  Dist[1/(a*c),Int[(a+b*x)^(n+1)*(c+d*x)^(n+1)/x,x]] /;
FreeQ[{a,b,c,d},x] && FractionQ[n] && n<-1 && ZeroQ[a*d+b*c]


(* ::Code:: *)
Int[(a_+b_.*x_)^n_*(c_+d_.*x_)^n_/x_,x_Symbol] :=
  -(a+b*x)^(n+1)*(c+d*x)^(n+1)/(2*a*c*(n+1)) -
  Dist[(a*d+b*c)/(2*a*c),Int[(a+b*x)^n*(c+d*x)^n,x]] +
  Dist[1/(a*c),Int[(a+b*x)^(n+1)*(c+d*x)^(n+1)/x,x]] /;
FreeQ[{a,b,c,d},x] && FractionQ[n] && n<-1 && NonzeroQ[a*d+b*c]


(* ::Code:: *)
Int[(a_+b_.*x_)^n_*(c_+d_.*x_)^p_/x_,x_Symbol] :=
  Dist[b,Int[(a+b*x)^(n-1)*(c+d*x)^p,x]] +
  Dist[a,Int[(a+b*x)^(n-1)*(c+d*x)^p/x,x]] /;
FreeQ[{a,b,c,d},x] && FractionQ[{n,p}] && n>0 && IntegerQ[n-p]


(* ::Code:: *)
Int[(a_+b_.*x_)^n_*(c_+d_.*x_)^p_/x_,x_Symbol] :=
  Dist[1/a,Int[(a+b*x)^(n+1)*(c+d*x)^p/x,x]] -
  Dist[b/a,Int[(a+b*x)^n*(c+d*x)^p,x]] /;
FreeQ[{a,b,c,d},x] && FractionQ[{n,p}] && n<-1 && IntegerQ[n-p]


(* ::Code:: *)
Int[x_^m_*(a_.+b_.*x_)^n_*(c_.+d_.*x_)^n_,x_Symbol] :=
  x^(m-1)*(a+b*x)^(n+1)*(c+d*x)^(n+1)/(2*b*d*(n+1)) +
  Dist[1/(b*d),Int[x^(m-2)*(a+b*x)^(n+1)*(c+d*x)^(n+1),x]] /;
FreeQ[{a,b,c,d},x] && FractionQ[n] && RationalQ[m] && m+2*n+1==0 && m>1 && ZeroQ[a*d+b*c]


(* ::Code:: *)
Int[x_^m_*(a_.+b_.*x_)^n_*(c_.+d_.*x_)^n_,x_Symbol] :=
  x^(m-1)*(a+b*x)^(n+1)*(c+d*x)^(n+1)/(2*b*d*(n+1)) -
  Dist[(a*d+b*c)/(2*b*d),Int[x^(m-1)*(a+b*x)^n*(c+d*x)^n,x]] +
  Dist[1/(b*d),Int[x^(m-2)*(a+b*x)^(n+1)*(c+d*x)^(n+1),x]] /;
FreeQ[{a,b,c,d},x] && FractionQ[n] && RationalQ[m] && m+2*n+1==0 && m>1 && NonzeroQ[a*d+b*c]


(* ::Code:: *)
Int[x_*(a_.+b_.*x_)^n_*(c_.+d_.*x_)^n_,x_Symbol] :=
  (a+b*x)^(n+1)*(c+d*x)^(n+1)/(2*b*d*(n+1)) /;
FreeQ[{a,b,c,d,n},x] && NonzeroQ[n+1] && ZeroQ[a*d+b*c]


(* ::Code:: *)
Int[x_*(a_.+b_.*x_)^n_*(c_.+d_.*x_)^n_,x_Symbol] :=
  (a+b*x)^(n+1)*(c+d*x)^(n+1)/(2*b*d*(n+1)) -
  Dist[(a*d+b*c)/(2*b*d),Int[(a+b*x)^n*(c+d*x)^n,x]] /;
FreeQ[{a,b,c,d},x] && FractionQ[n]


(* ::Code:: *)
Int[x_^m_*(a_.+b_.*x_)^n_*(c_.+d_.*x_)^n_,x_Symbol] :=
  x^(m-1)*(a+b*x)^(n+1)*(c+d*x)^(n+1)/(b*d*(m+2*n+1)) -
  Dist[a*c*(m-1)/(b*d*(m+2*n+1)),Int[x^(m-2)*(a+b*x)^n*(c+d*x)^n,x]] /;
FreeQ[{a,b,c,d},x] && FractionQ[n] && RationalQ[m] && NonzeroQ[m+2*n+1] && m>1 &&
(ZeroQ[m+n] || ZeroQ[a*d+b*c])


(* ::Code:: *)
Int[x_^m_*(a_.+b_.*x_)^n_*(c_.+d_.*x_)^n_,x_Symbol] :=
  x^(m-1)*(a+b*x)^(n+1)*(c+d*x)^(n+1)/(b*d*(m+2*n+1)) -
  Dist[(m+n)*(a*d+b*c)/(b*d*(m+2*n+1)),Int[x^(m-1)*(a+b*x)^n*(c+d*x)^n,x]] -
  Dist[a*c*(m-1)/(b*d*(m+2*n+1)),Int[x^(m-2)*(a+b*x)^n*(c+d*x)^n,x]] /;
FreeQ[{a,b,c,d},x] && FractionQ[n] && RationalQ[m] && NonzeroQ[m+2*n+1] && m>1 &&
NonzeroQ[m+n] && NonzeroQ[a*d+b*c]


(* ::Code:: *)
Int[x_^m_*(a_+b_.*x_)^n_*(c_+d_.*x_)^n_,x_Symbol] :=
  x^(m+1)*(a+b*x)^(n+1)*(c+d*x)^(n+1)/(a*c*(m+1)) /;
FreeQ[{a,b,c,d,n},x] && NonzeroQ[m+1] && ZeroQ[m+2*n+3] && ZeroQ[a*d+b*c]


(* ::Code:: *)
Int[x_^m_*(a_+b_.*x_)^n_*(c_+d_.*x_)^n_,x_Symbol] :=
  x^(m+1)*(a+b*x)^(n+1)*(c+d*x)^(n+1)/(a*c*(m+1)) -
  Dist[(m+n+2)*(a*d+b*c)/(a*c*(m+1)),Int[x^(m+1)*(a+b*x)^n*(c+d*x)^n,x]] /;
FreeQ[{a,b,c,d},x] && FractionQ[n] && RationalQ[m] && m<-1 && ZeroQ[m+2*n+3]


(* ::Code:: *)
Int[x_^m_*(a_+b_.*x_)^n_*(c_+d_.*x_)^n_,x_Symbol] :=
  x^(m+1)*(a+b*x)^(n+1)*(c+d*x)^(n+1)/(a*c*(m+1)) -
  Dist[b*d*(m+2*n+3)/(a*c*(m+1)),Int[x^(m+2)*(a+b*x)^n*(c+d*x)^n,x]] /;
FreeQ[{a,b,c,d},x] && FractionQ[n] && RationalQ[m] && m<-1 && (ZeroQ[m+n+2] || ZeroQ[a*d+b*c])


(* ::Code:: *)
Int[x_^m_*(a_+b_.*x_)^n_*(c_+d_.*x_)^n_,x_Symbol] :=
  x^(m+1)*(a+b*x)^(n+1)*(c+d*x)^(n+1)/(a*c*(m+1)) -
  Dist[(m+n+2)*(a*d+b*c)/(a*c*(m+1)),Int[x^(m+1)*(a+b*x)^n*(c+d*x)^n,x]] -
  Dist[b*d*(m+2*n+3)/(a*c*(m+1)),Int[x^(m+2)*(a+b*x)^n*(c+d*x)^n,x]] /;
FreeQ[{a,b,c,d},x] && FractionQ[n] && RationalQ[m] && m<-1 && NonzeroQ[m+n+2] &&
NonzeroQ[m+2*n+3] && NonzeroQ[a*d+b*c]


(* ::Code:: *)
Int[x_^m_.*Sqrt[a_+b_.*x_]/Sqrt[c_.+d_.*x_],x_Symbol] :=
  x^m*Sqrt[a+b*x]*Sqrt[c+d*x]/(d*(m+1)) -
  Dist[a*c*m/(d*(m+1)),Int[x^(m-1)/(Sqrt[a+b*x]*Sqrt[c+d*x]),x]] +
  Dist[(a*d-b*c*(2*m+1))/(2*d*(m+1)),Int[x^m/(Sqrt[a+b*x]*Sqrt[c+d*x]),x]] /;
FreeQ[{a,b,c,d},x] && IntegerQ[m] && m>0


(* ::Code:: *)
Int[Sqrt[a_+b_.*x_]/(x_^2*Sqrt[c_.+d_.*x_]),x_Symbol] :=
  -Sqrt[a+b*x]*Sqrt[c+d*x]/(c*x) +
  Dist[(b*c-a*d)/(2*c),Int[1/(x*Sqrt[a+b*x]*Sqrt[c+d*x]),x]] /;
FreeQ[{a,b,c,d},x]


(* ::Code:: *)
Int[x_^m_.*Sqrt[a_+b_.*x_]/Sqrt[c_.+d_.*x_],x_Symbol] :=
  x^(m+1)*Sqrt[a+b*x]*Sqrt[c+d*x]/(c*(m+1)) -
  Dist[(b*c+a*d*(2*m+3))/(2*c*(m+1)),Int[x^(m+1)/(Sqrt[a+b*x]*Sqrt[c+d*x]),x]] -
  Dist[b*d*(m+2)/(c*(m+1)),Int[x^(m+2)/(Sqrt[a+b*x]*Sqrt[c+d*x]),x]] /;
FreeQ[{a,b,c,d},x] && IntegerQ[m] && m<-2


(* ::Code:: *)
Int[x_^m_.*(a_+b_.*x_)^n_*(c_.+d_.*x_)^p_.,x_Symbol] :=
  Sum[Dist[(-a)^(m-k)/b^m*Binomial[m,m-k],Int[(a+b*x)^(n+k)*(c+d*x)^p,x]],{k,0,m}] /;
FreeQ[{a,b,c,d,n,p},x] && IntegersQ[m,p-n] && m>0 && Not[IntegerQ[n]] && p-n<0 &&
(m>3 || n=!=-1/2)


(* ::Code:: *)
Int[x_^m_.*(a_+b_.*x_)^n_*(c_.+d_.*x_)^p_,x_Symbol] :=
  Sum[Dist[(-a)^(m-k)/b^m*Binomial[m,m-k],Int[(a+b*x)^(n+k)*(c+d*x)^p,x]],{k,0,p-n-1}] + 
  Sum[Dist[(-a/b)^(m-k)/(-a)^(p-n)*Binomial[m-k-1,p-n-1],Int[x^k*(a+b*x)^p*(c+d*x)^p,x]],{k,0,m-p+n}] /;
FreeQ[{a,b,c,d,n,p},x] && IntegersQ[m,p-n] && 0<p-n<=m && Not[IntegerQ[n]]


(* ::Code:: *)
Int[x_^m_.*(a_+b_.*x_)^n_*(c_.+d_.*x_)^p_,x_Symbol] :=
  Sum[Dist[a^(m-k)/(-b)^m*Binomial[k-m-1,-m-1],Int[(a+b*x)^(n+k)*(c+d*x)^p,x]],{k,0,p-n-1}] + 
  Sum[Dist[(-b/a)^k/a^(p-n)*Binomial[p-n+k-1,p-n-1],Int[x^(m+k)*(a+b*x)^p*(c+d*x)^p,x]],{k,0,-m-1}] /;
FreeQ[{a,b,c,d,n,p},x] && IntegersQ[m,p-n] && m<0 && p-n>0 && Not[IntegerQ[n]]



