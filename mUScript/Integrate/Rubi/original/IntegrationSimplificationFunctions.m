(* ::Package:: *)

(* ::Title:: *)
(*Integration Simplification Functions*)


(* ::Section:: *)
(*Distribution Function*)


(* Dist[u,v] returns the sum of u times each term of v, provided v is free of Int. *) 
DownValues[Dist]={};
UpValues[Dist]={};

Dist[0,v_] :=
  (Print["*** Warning ***:  Dist[0,",v,"]"]; 0);

Dist[1,v_] := v

Dist[u_,v_] := -Dist[-u,v] /;
NumericFactor[u]<0

(*
Unprotect[Plus]
(*Dist /: *)Dist[u_,v_]+Dist[w_,v_] := 
  If[ZeroQ[u+w],
    0,
  Dist[u+w,v]]

(*Dist /: *)Dist[u_,v_]-Dist[w_,v_] := 
  If[ZeroQ[u-w],
    0,
  Dist[u-w,v]]
Protect[Plus]

Unprotect[Times]
(*Dist /: *)w_*Dist[u_,v_] := Dist[w*u,v] /; w=!=-1
Protect[Times]
*)

Dist[u_,Dist[v_,w_]] := Dist[Sim[u*v],w]

Dist[u_,v_] := Map[Function[Dist[u,#]],v] /;
  SumQ[v]

Dist[u_,v_] := DistSimp[u*v] /;
  FreeQ[v,Int]

Dist[u_,v_*w_] := Dist[Sim[u*v],w] /;
  FreeQ[v,Int]

Dist[u_,v_] := DistSimp[u*v] /;
  ShowSteps=!=True


(* Basis: If n is an integer, (a+b*z)^m*(b+a/z)^n == (a+b*z)^(m+n)/z^n *)
DistSimp[u_*(a_+b_.*f_[v_])^m_*(b_+a_.*g_[v_])^n_.] :=
  u*(a+b*f[v])^(m+n)/f[v]^n /;
TrigQ[f] && TrigQ[g] && f[v]===1/g[v] && RationalQ[m] && IntegerQ[n]

DistSimp[u_] := u


(* ::Section::Closed:: *)
(*Simplification Functions*)


(* ::Subsection::Closed:: *)
(*Regularize*)


Regularize[u_,x_Symbol] :=
  If[RecognizedFormQ[u,x],
    u,
(* NormalForm[NormalForm[NormalForm[u,x],x],x]] *)
  NormalForm[NormalForm[NormalForm[Simplify[u],x],x],x]]
(* Simp[NormalForm[Simplify[u],x]]] *)


(* ::Subsection::Closed:: *)
(*RecognizedFormQ*)


(* If u (x) is a form recognized by the integrator, RecognizedFormQ[u,x] returns True, else False *)
RecognizedFormQ[u_,x_Symbol] :=
  FreeQ[u,x] ||
  MonomialQ[u,x] ||
(*  MonomialSumQ[u,x] || *)
  MatchQ[u,a_*v_ /; FreeQ[a,x] && RecognizedFormQ[v,x]] ||
  MatchQ[u,f_[a_.+b_.*x]^n_. /; FreeQ[{a,b,f},x] && IntegerQ[n]] ||
  MatchQ[u,(a_+b_.*x^n_.)^p_. /; FreeQ[{a,b,n,p},x] && IntegerQ[n] && n>0] ||
  MatchQ[u,x^m_.*(a_+b_.*x^n_.)^p_. /; FreeQ[{a,b,m,n,p},x] && IntegerQ[n] && n>0] ||
  MatchQ[u,(a_.+b_.*x)^m_.*(c_.+d_.*x)^n_. /; FreeQ[{a,b,c,d,m,n},x]] ||
  MatchQ[u,x^p_.*(a_.+b_.*x)^m_.*(c_.+d_.*x)^n_. /; FreeQ[{a,b,c,d,m,n,p},x]] ||

  MatchQ[u,x^m_.*(b_.*x+c_.*x^2)^n_ /; FreeQ[{b,c,m,n},x] && Not[IntegerQ[n]]] ||
  MatchQ[u,(a_.+b_.*x+c_.*x^2)^n_. /; FreeQ[{a,b,c,n},x]] ||
  MatchQ[u,x^m_.*(a_+b_.*x+c_.*x^2)^n_. /; FreeQ[{a,b,c,m,n},x]] ||
  MatchQ[u,(d_+e_.*x)/(a_+b_.*x+c_.*x^2) /; FreeQ[{a,b,c,d,e},x]] ||
  MatchQ[u,x^m_.*(d_+e_.*x)/(a_+b_.*x+c_.*x^2) /; FreeQ[{a,b,c,d,e},x]] ||

  MatchQ[u,v_*(a_+b_.*x) /; FreeQ[{a,b},x] && FunctionOfQ[a*x+b*x^2/2,v,x]] ||
  MatchQ[u,(a_.*v_^m_.+b_.*v_^n_.)/(c_.*v_^m_.+d_.*v_^n_.) /; FreeQ[{a,b,c,d,m,n},x]]


(* ::Subsection::Closed:: *)
(*NormalForm*)


(* NormalForm[u,x] returns u[x] in normal form. *)
DownValues[NormalForm]={};

(* Expansion rules *)

(* NormalForm[c_*(a_+b_.*x_^n_.),x_Symbol] :=
  a*c + b*c*x^n /;
FreeQ[{a,b,c,n},x] *)

(* Basis: If n is odd, -m*(-z+w)^n == m*(z-w)^n *)
NormalForm[m_*u_^n_,x_Symbol] :=
  NormalForm[-m*Map[Function[-#],u]^n,x] /;
RationalQ[m] && m<0 && OddQ[n] && SumQ[u] && NegativeCoefficientQ[u]

(* Basis: If m and n are odd, (-z+w)^m*(-r+s)^n == (z-w)^m*(r-s)^n *)
NormalForm[u_^m_*v_^n_,x_Symbol] :=
  NormalForm[Map[Function[-#],u]^m*Map[Function[-#],v]^n,x] /;
OddQ[{m,n}] && SumQ[u] && SumQ[v] && NegativeCoefficientQ[u] && NegativeCoefficientQ[v]

NormalForm[u_,x_Symbol] :=
  Coefficient[u,x,0] + Coefficient[u,x,1]*x /;
LinearQ[u,x]

(* NormalForm[u_,x_Symbol] :=
  Module[{lst=BinomialTest[u,x]},
  lst[[1]] + lst[[2]]*x^lst[[3]] /;
 Not[FalseQ[lst]]] *)

(* NormalForm[a_*(b_+u_),x_Symbol] :=
  a*b + NormalForm[a*u,x] /;
FreeQ[{a,b},x] *)

NormalForm[a_*u_,x_Symbol] :=
  NormalForm[Map[Function[a*#],u],x] /;
FreeQ[a,x] && SumQ[u]

NormalForm[b_.*x_*(c_+d_.*x_),x_Symbol] :=
  b*c*x + b*d*x^2 /;
FreeQ[{b,c,d},x]

NormalForm[a_+b_.*x_^n_.*(c_+d_.*x_^n_.),x_Symbol] :=
  a + b*c*x^n + b*d*x^(2*n) /;
FreeQ[{a,b,c,d,n},x]

NormalForm[c_+d_.*(a_+b_.*x_^n_.)^2,x_Symbol] :=
  c + a^2*d + 2*a*b*d*x^n + b^2*d*x^(2*n) /;
FreeQ[{a,b,c,d,n},x]

NormalForm[c_.+e_.*x_^n_.+d_.*(a_+b_.*x_^n_.)^2,x_Symbol] :=
  c + a^2*d + (e+2*a*b*d)*x^n + b^2*d*x^(2*n) /;
FreeQ[{a,b,c,d,e,n},x]

NormalForm[a_+b_.*f_^(e_.+g_.*x_)*(c_+d_.*f_^(e_.+g_.*x_)),x_Symbol] :=
  a + b*c*f^(e+g*x) + b*d*f^(2*e+2*g*x) /;
FreeQ[{a,b,c,d,e,f,g},x]

NormalForm[c_.*x_^m_.*(a_+b_.*x_^n_.),x_Symbol] :=
  c*(b+a*x^m) /;
FreeQ[{a,b,c,m,n},x] && ZeroQ[m+n]

NormalForm[f_[c_.*x_^m_.*(a_+b_.*x_^n_.)^p_],x_Symbol] :=
  f[c*(b+a/x^n)^p] /;
FreeQ[{a,b,c,f,m,n},x] && IntegerQ[p] && ZeroQ[m+n*p]

NormalForm[(e_.*(a_+b_.*x_)*(c_+d_.*x_))^n_,x_Symbol] :=
  (a*c*e+(a*d*e+b*c*e)*x+b*d*e*x^2)^n /;
FreeQ[{a,b,c,d,e},x] && FractionQ[n]


(* Expand quadratic polynomials *) 
(* NormalForm[c_+d_.*(a_+b_.*x_^n_)^2,x_Symbol] :=
  c + a^2*d + 2*a*b*d*x^n + b^2*d*x^(2*n) /;
FreeQ[{a,b,c,d,n},x]

NormalForm[c_+e_.*x_^n_+d_.*(a_+b_.*x_^n_)^2,x_Symbol] :=
  c + a^2*d + (e+2*a*b*d)*x^n + b^2*d*x^(2*n) /;
FreeQ[{a,b,c,d,e,n},x] *)

NormalForm[f_[u_],x_Symbol] :=
  f[ExpandPolynomial[u,x]] /;
FreeQ[f,x] && QuadraticPolynomialQ[u,x]

NormalForm[f_^u_,x_Symbol] :=
  f^ExpandPolynomial[u,x] /;
FreeQ[f,x] && QuadraticPolynomialQ[u,x]

(* NormalForm[u_^n_,x_Symbol] :=
  ExpandPolynomial[u,x]^n /;
FreeQ[n,x] && QuadraticPolynomialQ[u,x] *)

NormalForm[u_.*f_^v_*g_^w_,x_Symbol] :=
  u*E^Sum[(Log[f]*Coefficient[v,x,k]+Log[g]*Coefficient[w,x,k])*x^k,{k,0,2}] /;
FreeQ[{f,g},x] && PolynomialQ[v,x] && 1<=Exponent[v,x]<=2 && 
				  PolynomialQ[w,x] && 1<=Exponent[w,x]<=2


NormalForm[u_.*f_^(a_.+b_.*x_^n_.)*g_^(c_.+d_.*x_^n_.),x_Symbol] :=
  u*E^(a*Log[f]+c*Log[g]+(b*Log[f]+d*Log[g])*x^n) /;
FreeQ[{a,b,c,d,f,g,n},x]


QuadraticPolynomialQ[u_,x_Symbol] :=
  PolynomialQ[u,x] && Exponent[u,x]<=2 && Not[MatchQ[u,a_.*v_^2 /; FreeQ[a,x]]]


ExpandPolynomial[u_,x_Symbol] :=
  Module[{k},
  Sum[Coefficient[u,x,k]*x^k,{k,0,Exponent[u,x]}]]


(* Factoring rules *)
NormalForm[(a_.*x_^m_.+b_.*x_^n_.)^p_,x_Symbol] :=
  (x^m*(a+b*x^(n-m)))^p /;
FreeQ[{a,b},x] && RationalQ[{m,n,p}] && m<=n && Not[2*m==n==2]

NormalForm[u_.+a_.*v_+b_.*v_,x_Symbol] :=
  u+(a+b)*v /;
FreeQ[{a,b},x] && Not[FreeQ[v,x]]

NormalForm[u_^n_,x_Symbol] :=
  Module[{v=Together[u]},
  v^n /;
 Not[SumQ[v]]] /;
SumQ[u] && FractionQ[n] && Not[MonomialSumQ[u,x]]

(* NormalForm[u_*Cos[v_]^n_.*Sin[v_]^n_.,x_Symbol] :=
  u*Sin[2*v]^n/2^n /;
IntegerQ[n] && Not[FreeQ[v,x]] *)

NormalForm[Cos[2*u_]*Sec[u_]^2,x_Symbol] :=
  1-Tan[u]^2 /;
Not[FreeQ[u,x]]

(* Basis: (a^2-b^2)/(a+b)^2 == (a-b)/(a+b) *)
(* Note: Necessary because Mathematica does not simplify (1+x^2)/(1+I*x)^2 to (1-I*x)/(1+I*x) *)
NormalForm[u_. (c_+d_)/(a_+b_)^2,x_Symbol] :=
  NormalForm[u*(a-b)/(a+b),x] /;
ZeroQ[c-a^2] && ZeroQ[d+b^2]

NormalForm[u_,x_Symbol] :=
  If[AtomQ[u],
    u,
  If[MatchQ[u,a_.+b_.*x^n_. /; FreeQ[{a,b,n},x]],
    u,
  If[MatchQ[u,a_.+b_.*x^n_.+c_.*x^m_. /; FreeQ[{a,b,c,m,n},x] && 2*n===m],
    u,
  Module[{tmp},
  tmp=MonomialFactor[u,x];
  If[NonzeroQ[tmp[[1]]] (* && Not[NegativeQ[tmp[[1]]]] *),
    x^tmp[[1]]*NormalForm[tmp[[2]],x],
  Map[Function[NormalForm[#,x]],u]]]]]]


(* ::Subsection::Closed:: *)
(*SimplifyExpression*)


DownValues[SimplifyExpression]={}


(* Note: These rules are only applied if an integration rule other than partial fraction 
	expansion should be used to integrate the expression. *)


(* Derivation: Algebraic simplification *)
(* Basis: If p is an integer, (a+b*x^n)^p == x^(n*p)*(b+a*x^(-n))^p *)
SimplifyExpression[x_^m_*(a_+b_.*x_^n_)^p_.,x_Symbol] :=
  x^(m+n*p)*(b+a*x^(-n))^p /;
FreeQ[{a,b},x] && IntegersQ[m,n,p] && n<0 && (m+n*p+1==0 || p<-1 && -n>1 && -n<=m+n*p<-2*n-1)


(* Basis: a*x+b*x^m+c*x^n == x*(a+b*x^(m-1)+c*x^(n-1)) *)
SimplifyExpression[1/(a_.*x_+b_.*x_^m_+c_.*x_^n_),x_Symbol] :=
  1/(x*(a+b*x^(m-1)+c*x^(n-1))) /;
FreeQ[{a,b,c},x] && IntegersQ[m,n] && m>2 && n-1==2*(m-1)


(* Basis: (e^2-a^2*f-a^2*g*x^2)/(-a*f-a*g*x^2+e*Sqrt[f+g*x^2]) == a+e/Sqrt[f+g*x^2] *)
SimplifyExpression[(a_.+b_.*x_^2)/(c_+d_.*x_^2+e_.*Sqrt[f_+g_.*x_^2]), x_Symbol] :=
  -c/f+e/Sqrt[f+g*x^2] /;
FreeQ[{a,b,c,d,e,f,g},x] && ZeroQ[c*g-f*d] && ZeroQ[b+d^2/g] && ZeroQ[a-e^2+c^2/f]


(* Basis: 1/(x*(a/x+b+c*x)) == 1/(a+b*x+c*x^2) *) 
SimplifyExpression[1/(x_*(a_./x_+b_+c_.*x_)),x_Symbol] :=
  1/(a+b*x+c*x^2) /;
FreeQ[{a,b,c},x]

(* Basis: 1/(x^2*(a/x^2+b/x+c)) == 1/(a+b*x+c*x^2) *) 
SimplifyExpression[1/(x_^2*(a_./x_^2+b_./x_+c_)),x_Symbol] :=
  1/(a+b*x+c*x^2) /;
FreeQ[{a,b,c},x]

(* Basis: x^m/(a*x^m+b*x^(m+1)+c*x^(m+2)) == 1/(a+b*x+c*x^2) *) 
SimplifyExpression[x_^m_./(a_.*x_^m_.+b_.*x_^p_.+c_.*x_^q_.),x_Symbol] :=
  1/(a+b*x+c*x^2) /;
FreeQ[{a,b,c},x] && IntegersQ[m,p,q] && p==m+1 && q==m+2


SimplifyExpression[u_,x_Symbol] :=
  If[MatchQ[u,w_.*(a_.+b_.*v_)^m_.*(c_.+d_.*v_)^n_. /; 
		FreeQ[{a,b,c,d},x] && IntegersQ[m,n] && m<0 && n<0],
    u,
  Module[{v},
  v=Regularize[u,x];
  If[SimplerExpressionQ[v,u,x],
    v,
  v=Regularize[Cancel[u],x];
  If[SimplerExpressionQ[v,u,x],
    v,
  u]]]]


(* If u is a significantly simpler expression than v, SimplerExpressionQ[u,v,x] returns True; 
	else it returns False. *)
SimplerExpressionQ[u_,v_,x_] :=
  SmartLeafCount[u] <= 2/3*SmartLeafCount[v] ||
(*Count[u,x,Infinity] < Count[v,x,Infinity] || *)
(*RecognizedFormQ[u,x] && Not[RecognizedFormQ[v,x]] || *)
  PolynomialQ[Numerator[u],x] && PolynomialQ[Denominator[u],x] && 
    PolynomialQ[Numerator[v],x] && PolynomialQ[Denominator[v],x] && 
    Exponent[Numerator[u],x]<Exponent[Numerator[v],x] && 
    Exponent[Denominator[u],x]<Exponent[Denominator[v],x] && 
	SmartLeafCount[u] <= SmartLeafCount[v]+5


(* SmartLeafCount[u] returns the number of atoms in u and, unlike LeafCount, treats rational
	and complex numbers as atoms. *)
SmartLeafCount[u_] :=
  If[AtomQ[u],
    1,
  Apply[Plus,Map[SmartLeafCount,Apply[List,u]]]+1]


(* ::Subsection::Closed:: *)
(*TrigSimplify*)


(* Note: It may be necessary for TrigSimplify to know and use the independent variable ??? *)


(* TrigSimplify[u] returns a bottom-up trig simplification of u. *)
TrigSimplify[u_] :=
  If[AtomQ[u],
    u,
  If[Head[u]===If,
    u,
  TrigSimplifyAux[Map[TrigSimplify,u]]]]

DownValues[TrigSimplifyAux]={}


(* Basis: a*z^m+b*z^n == z^m*(a+b*z^(n-m)) *)
TrigSimplifyAux[u_.*(a_.*v_^m_.+b_.*v_^n_.)^p_] :=
  u*v^(m*p)*TrigSimplifyAux[a+b*v^(n-m)]^p /;
(TrigQ[v] || HyperbolicQ[v]) && IntegerQ[p] && RationalQ[{m,n}] && m<n


(* ::Subsubsection::Closed:: *)
(*Circular trig function simplification rules*)


(* Basis: Sin[z]^2+Cos[z]^2 == 1 *)
TrigSimplifyAux[u_.*Cos[z_]^2+v_.*Sin[z_]^2+w_.] :=
  u+w /;
u===v

(* Basis: Sec[z]^2-Tan[z]^2 == 1 *)
TrigSimplifyAux[u_.*Sec[z_]^2+v_.*Tan[z_]^2+w_.] :=
  u+w /;
u===-v

(* Basis: Csc[z]^2-Cot[z]^2 == 1 *)
TrigSimplifyAux[u_.*Csc[z_]^2+v_.*Cot[z_]^2+w_.] :=
  u+w /;
u===-v


(* Basis: 1-Sin[z]^2 == Cos[z]^2 *)
TrigSimplifyAux[u_+v_.*Sin[z_]^2+w_.] :=
  u*Cos[z]^2+w /;
u===-v

(* Basis: 1-Cos[z]^2 == Sin[z]^2 *)
TrigSimplifyAux[u_+v_.*Cos[z_]^2+w_.] :=
  u*Sin[z]^2+w /;
u===-v

(* Basis: 1+Tan[z]^2 == Sec[z]^2 *)
TrigSimplifyAux[u_+v_.*Tan[z_]^2+w_.] :=
  u*Sec[z]^2+w /;
u===v

(* Basis: 1+Cot[z]^2 == Csc[z]^2 *)
TrigSimplifyAux[u_+v_.*Cot[z_]^2+w_.] :=
  u*Csc[z]^2+w /;
u===v

(* Basis: -1+Sec[z]^2 == Tan[z]^2 *)
TrigSimplifyAux[u_+v_.*Sec[z_]^2+w_.] :=
  v*Tan[z]^2+w /;
u===-v

(* Basis: -1+Csc[z]^2 == Cot[z]^2 *)
TrigSimplifyAux[u_+v_.*Csc[z_]^2+w_.] :=
  v*Cot[z]^2+w /;
u===-v


(* Basis: If a^2-b^2==0, Sin[z]^2/(a+b*Cos[z]) == 1/a-Cos[z]/b *)
TrigSimplifyAux[u_.*Sin[v_]^2/(a_+b_.*Cos[v_])] :=
  u*(1/a - Cos[v]/b) /;
ZeroQ[a^2-b^2]

(* Basis: If a^2-b^2==0, Cos[z]^2/(a+b*Sin[z]) == 1/a-Sin[z]/b *)
TrigSimplifyAux[u_.*Cos[v_]^2/(a_+b_.*Sin[v_])] :=
  u*(1/a - Sin[v]/b) /;
ZeroQ[a^2-b^2]


(* Basis: If n is an integer, Tan[z]^n/(a+b*Tan[z]^n) == 1/(b+a*Cot[z]^n) *)
TrigSimplifyAux[u_.*Tan[v_]^n_./(a_+b_.*Tan[v_]^n_.)] :=
  u/(b+a*Cot[v]^n) /;
IntegerQ[n] && n>0 && NonsumQ[a]

(* Basis: If n is an integer, Cot[z]^n/(a+b*Cot[z]^n) == 1/(b+a*Tan[z]^n) *)
TrigSimplifyAux[u_.*Cot[v_]^n_./(a_+b_.*Cot[v_]^n_.)] :=
  u/(b+a*Tan[v]^n) /;
IntegerQ[n] && n>0 && NonsumQ[a]

(* Basis: If n is an integer, Sec[z]^n/(a+b*Sec[z]^n) == 1/(b+a*Cos[z]^n) *)
TrigSimplifyAux[u_.*Sec[v_]^n_./(a_+b_.*Sec[v_]^n_.)] :=
  u/(b+a*Cos[v]^n) /;
IntegerQ[n] && n>0 && NonsumQ[a]

(* Basis: If n is an integer, Csc[z]^n/(a+b*Csc[z]^n) == 1/(b+a*Sin[z]^n) *)
TrigSimplifyAux[u_.*Csc[v_]^n_./(a_+b_.*Csc[v_]^n_.)] :=
  u/(b+a*Sin[v]^n) /;
IntegerQ[n] && n>0 && NonsumQ[a]


(* Basis: Csc[z]+Cot[z] == Cot[z/2] *)
TrigSimplifyAux[(a_.*Csc[v_]+b_.*Cot[v_])^n_] :=
  a^n*Cot[v/2]^n /;
EvenQ[n] && ZeroQ[a-b]

(* Basis: Csc[z]-Cot[z] == Tan[z/2] *)
TrigSimplifyAux[(a_.*Csc[v_]+b_.*Cot[v_])^n_] :=
  a^n*Tan[v/2]^n /;
EvenQ[n] && ZeroQ[a+b]


(* Basis: Sin[z]*(a+b*Cot[z]) == a*Sin[z] + b*Cos[z] *)
(* TrigSimplifyAux[u_*Sin[v_]^m_.*(a_.+b_.*Cot[v_]^2)^p_.] :=
  u*(b*Cos[v]^2+a*Sin[v]^2)^p /;
IntegersQ[m,p] && m==2*p *)

(* Basis: a+b*Cot[z] == (b*Cos[z]+a*Sin[z])/Sin[z] *)
TrigSimplifyAux[u_.*Sin[v_]^m_.*(a_.+b_.*Cot[v_]^n_.)^p_.] :=
  u*Sin[v]^(m-n*p)*(b*Cos[v]^n+a*Sin[v]^n)^p /;
IntegersQ[m,n,p]

(* Basis: Cos[z]*(a+b*Tan[z]) == a*Cos[z] + b*Sin[z] *)
(* TrigSimplifyAux[u_*Cos[v_]^m_.*(a_.+b_.*Tan[v_]^2)^p_.] :=
  u*(b*Sin[v]^2+a*Cos[v]^2)^p /;
IntegersQ[m,p] && m==2*p *)

(* Basis: a+b*Tan[z] == (b*Sin[z]+a*Cos[z])/Cos[z] *)
TrigSimplifyAux[u_.*Cos[v_]^m_.*(a_.+b_.*Tan[v_]^n_.)^p_.] :=
  u*Cos[v]^(m-n*p)*(b*Sin[v]^n+a*Cos[v]^n)^p /;
IntegersQ[m,n,p]

(* Basis: (a+b*Tan[z])/Sec[z] == a*Cos[z] + b*Sin[z] *)
TrigSimplifyAux[u_*Sec[v_]^m_.*(a_.+b_.*Tan[v_]^2)^p_.] :=
  u*(b*Sin[v]^2+a*Cos[v]^2)^p /;
IntegersQ[m,p] && m+2*p==0

(* Basis: (a+b*Cot[z])/Csc[z] == a*Sin[z] + b*Cos[z] *)
TrigSimplifyAux[u_*Csc[v_]^m_.*(a_.+b_.*Cot[v_]^2)^p_.] :=
  u*(b*Cos[v]^2+a*Sin[v]^2)^p /;
IntegersQ[m,p] && m+2*p==0


(* Basis: a+b*Tan[z]+c*Sec[z] == (c+b*Sin[z]+a*Cos[z])/Cos[z] *)
TrigSimplifyAux[u_.*Cos[v_]^m_.*(a_.+b_.*Tan[v_]^n_.+c_.*Sec[v_]^n_.)^p_.] :=
  u*Cos[v]^(m-n*p)*(c+b*Sin[v]^n+a*Cos[v]^n)^p /;
IntegersQ[m,n,p]

(* Basis: a+b*Tan[z]+c*Sec[z] == Sec[z]*(c+b*Sin[z]+a*Cos[z]) *)
TrigSimplifyAux[u_.*Sec[v_]^m_.*(a_.+b_.*Tan[v_]^n_.+c_.*Sec[v_]^n_.)^p_.] :=
  u*Sec[v]^(m+n*p)*(c+b*Sin[v]^n+a*Cos[v]^n)^p /;
IntegersQ[m,n,p]

(* Basis: a+b*Cot[z]+c*Csc[z] == (c+b*Cos[z]+a*Sin[z])/Sin[z] *)
TrigSimplifyAux[u_.*Sin[v_]^m_.*(a_.+b_.*Cot[v_]^n_.+c_.*Csc[v_]^n_.)^p_.] :=
  u*Sin[v]^(m-n*p)*(c+b*Cos[v]^n+a*Sin[v]^n)^p /;
IntegersQ[m,n,p]

(* Basis: a+b*Cot[z]+c*Csc[z] == Csc[z]*(c+b*Cos[z]+a*Sin[z]) *)
TrigSimplifyAux[u_.*Csc[v_]^m_.*(a_.+b_.*Cot[v_]^n_.+c_.*Csc[v_]^n_.)^p_.] :=
  u*Csc[v]^(m+n*p)*(c+b*Cos[v]^n+a*Sin[v]^n)^p /;
IntegersQ[m,n,p]


(* Basis: If n is an integer, Tan[z]^n/(a+b*Sec[z]^n) == Sin[z]^n/(b+a*Cos[z]^n) *)
TrigSimplifyAux[u_.*Tan[v_]^n_./(a_+b_.*Sec[v_]^n_.)] :=
  u*Sin[v]^n/(b+a*Cos[v]^n) /;
IntegerQ[n] && n>0 && NonsumQ[a]

(* Basis: If n is an integer, Cot[z]^n/(a+b*Csc[z]^n) == Cos[z]^n/(b+a*Sin[z]^n) *)
TrigSimplifyAux[u_.*Cot[v_]^n_./(a_+b_.*Csc[v_]^n_.)] :=
  u*Cos[v]^n/(b+a*Sin[v]^n) /;
IntegerQ[n] && n>0 && NonsumQ[a]


(* Basis: If n is an integer, Sin[z]^(-n)*(a*Cos[z]^n+b*Sin[z]^n) == b+a*Cot[z]^n *)
TrigSimplifyAux[Sin[v_]^m_.*(a_.*Cos[v_]^n_.+b_.*Sin[v_]^n_.)^p_] :=
  (b+a*Cot[v]^n)^p /;
IntegersQ[m,n,p] && n>0 && p<0 && m==-n*p

(* Basis: If n is an integer, Cos[z]^(-n)*(a*Cos[z]^n+b*Sin[z]^n) == a+b*Tan[z]^n *)
TrigSimplifyAux[Cos[v_]^m_.*(a_.*Cos[v_]^n_.+b_.*Sin[v_]^n_.)^p_] :=
  (a+b*Tan[v]^n)^p /;
IntegersQ[m,n,p] && n>0 && p<0 && m==-n*p


(* Basis: If a^2+b^2=0, 1/(a*Cos[z] + b*Sin[z]) == Cos[z]/a + Sin[z]/b *)
TrigSimplifyAux[(a_.*Cos[v_]+b_.*Sin[v_])^n_] :=
  (Cos[v]/a + Sin[v]/b)^(-n) /;
IntegerQ[n] && n<0 && ZeroQ[a^2+b^2]


(* ::Subsubsection::Closed:: *)
(*Hyperbolic trig function simplification rules*)


(* Basis: Cosh[z]^2-Sinh[z]^2 == 1 *)
TrigSimplifyAux[u_.*Cosh[z_]^2+v_.*Sinh[z_]^2+w_.] :=
  u+w /;
u===-v

(* Basis: Sech[z]^2+Tanh[z]^2 == 1 *)
TrigSimplifyAux[u_.*Sech[z_]^2+v_.*Tanh[z_]^2+w_.] :=
  u+w /;
u===v

(* Basis: Coth[z]^2-Csch[z]^2 == 1 *)
TrigSimplifyAux[u_.*Coth[z_]^2+v_.*Csch[z_]^2+w_.] :=
  u+w /;
u===-v


(* Basis: 1+Sinh[z]^2 == Cosh[z]^2 *)
TrigSimplifyAux[u_+v_.*Sinh[z_]^2+w_.] :=
  u*Cosh[z]^2+w /;
u===v

(* Basis: -1+Cosh[z]^2 == Sinh[z]^2 *)
TrigSimplifyAux[u_+v_.*Cosh[z_]^2+w_.] :=
  v*Sinh[z]^2+w /;
u===-v

(* Basis: 1-Tanh[z]^2 == Sech[z]^2 *)
TrigSimplifyAux[u_+v_.*Tanh[z_]^2+w_.] :=
  u*Sech[z]^2+w /;
u===-v

(* Basis: -1+Coth[z]^2 == Csch[z]^2 *)
TrigSimplifyAux[u_+v_.*Coth[z_]^2+w_.] :=
  v*Csch[z]^2+w /;
u===-v

(* Basis: 1-Sech[z]^2 == Tanh[z]^2 *)
TrigSimplifyAux[u_+v_.*Sech[z_]^2+w_.] :=
  u*Tanh[z]^2+w /;
u===-v

(* Basis: 1+Csch[z]^2 == Coth[z]^2 *)
TrigSimplifyAux[u_+v_.*Csch[z_]^2+w_.] :=
  u*Coth[z]^2+w /;
u===v


(* Basis: If a^2-b^2==0, Sinh[z]^2/(a+b*Cosh[z]) == -1/a+Cosh[z]/b *)
TrigSimplifyAux[u_.*Sinh[v_]^2/(a_+b_.*Cosh[v_])] :=
  u*(-1/a + Cosh[v]/b) /;
ZeroQ[a^2-b^2]

(* Basis: If a^2+b^2==0, Cosh[z]^2/(a+b*Sinh[z]) == 1/a+Sinh[z]/b *)
TrigSimplifyAux[u_.*Cosh[v_]^2/(a_+b_.*Sinh[v_])] :=
  u*(1/a + Sinh[v]/b) /;
ZeroQ[a^2+b^2]


(* Basis: If n is an integer, Tanh[z]^n/(a+b*Tanh[z]^n) == 1/(b+a*Coth[z]^n) *)
TrigSimplifyAux[u_.*Tanh[v_]^n_./(a_+b_.*Tanh[v_]^n_.)] :=
  u/(b+a*Coth[v]^n) /;
IntegerQ[n] && n>0 && NonsumQ[a]

(* Basis: If n is an integer, Coth[z]^n/(a+b*Coth[z]^n) == 1/(b+a*Tanh[z]^n) *)
TrigSimplifyAux[u_.*Coth[v_]^n_./(a_+b_.*Coth[v_]^n_.)] :=
  u/(b+a*Tanh[v]^n) /;
IntegerQ[n] && n>0 && NonsumQ[a]

(* Basis: If n is an integer, Sech[z]^n/(a+b*Sech[z]^n) == 1/(b+a*Cosh[z]^n) *)
TrigSimplifyAux[u_.*Sech[v_]^n_./(a_+b_.*Sech[v_]^n_.)] :=
  u/(b+a*Cosh[v]^n) /;
IntegerQ[n] && n>0 && NonsumQ[a]

(* Basis: If n is an integer, Csch[z]^n/(a+b*Csch[z]^n) == 1/(b+a*Sinh[z]^n) *)
TrigSimplifyAux[u_.*Csch[v_]^n_./(a_+b_.*Csch[v_]^n_.)] :=
  u/(b+a*Sinh[v]^n) /;
IntegerQ[n] && n>0 && NonsumQ[a]


(* Basis: Csch[z]+Coth[z] == Coth[z/2] *)
TrigSimplifyAux[(a_.*Csch[v_]+b_.*Coth[v_])^n_] :=
  a^n*Coth[v/2]^n /;
EvenQ[n] && ZeroQ[a-b]

(* Basis: Csch[z]-Coth[z] == -Tanh[z/2] *)
TrigSimplifyAux[(a_.*Csch[v_]+b_.*Coth[v_])^n_] :=
  b^n*Tanh[v/2]^n /;
EvenQ[n] && ZeroQ[a+b]


(* Basis: Sinh[z]*(a+b*Coth[z]) == a*Sinh[z] + b*Cosh[z] *)
(* TrigSimplifyAux[u_*Sinh[v_]^m_.*(a_.+b_.*Coth[v_]^2)^p_.] :=
  u*(b*Cosh[v]^2+a*Sinh[v]^2)^p /;
IntegersQ[m,p] && m==2*p *)

(* Basis: a+b*Coth[z] == (b*Cosh[z]+a*Sinh[z])/Sinh[z] *)
TrigSimplifyAux[u_.*Sinh[v_]^m_.*(a_.+b_.*Coth[v_]^n_.)^p_.] :=
  u*Sinh[v]^(m-n*p)*(b*Cosh[v]^n+a*Sinh[v]^n)^p /;
IntegersQ[m,n,p]

(* Basis: Cosh[z]*(a+b*Tanh[z]) == a*Cosh[z] + b*Sinh[z] *)
(* TrigSimplifyAux[u_*Cosh[v_]^m_.*(a_.+b_.*Tanh[v_]^2)^p_.] :=
  u*(b*Sinh[v]^2+a*Cosh[v]^2)^p /;
IntegersQ[m,p] && m==2*p *)

(* Basis: a+b*Tanh[z] == (b*Sinh[z]+a*Cosh[z])/Cosh[z] *)
TrigSimplifyAux[u_.*Cosh[v_]^m_.*(a_.+b_.*Tanh[v_]^n_.)^p_.] :=
  u*Cosh[v]^(m-n*p)*(b*Sinh[v]^n+a*Cosh[v]^n)^p /;
IntegersQ[m,n,p]

(* Basis: (a+b*Tanh[z])/Sech[z] == a*Cosh[z] + b*Sinh[z] *)
TrigSimplifyAux[u_*Sech[v_]^m_.*(a_.+b_.*Tanh[v_]^2)^p_.] :=
  u*(b*Sinh[v]^2+a*Cosh[v]^2)^p /;
IntegersQ[m,p] && m+2*p==0

(* Basis: (a+b*Coth[z])/Csch[z] == a*Sinh[z] + b*Cosh[z] *)
TrigSimplifyAux[u_*Csch[v_]^m_.*(a_.+b_.*Coth[v_]^2)^p_.] :=
  u*(b*Cosh[v]^2+a*Sinh[v]^2)^p /;
IntegersQ[m,p] && m+2*p==0


(* Basis: a+b*Tanh[z]+c*Sech[z] == (c+b*Sinh[z]+a*Cosh[z])/Cosh[z] *)
TrigSimplifyAux[u_.*Cosh[v_]^m_.*(a_.+b_.*Tanh[v_]^n_.+c_.*Sech[v_]^n_.)^p_.] :=
  u*Cosh[v]^(m-n*p)*(c+b*Sinh[v]^n+a*Cosh[v]^n)^p /;
IntegersQ[m,n,p]

(* Basis: a+b*Tanh[z]+c*Sech[z] == Sech[z]*(c+b*Sinh[z]+a*Cosh[z]) *)
TrigSimplifyAux[u_.*Sech[v_]^m_.*(a_.+b_.*Tanh[v_]^n_.+c_.*Sech[v_]^n_.)^p_.] :=
  u*Sech[v]^(m+n*p)*(c+b*Sinh[v]^n+a*Cosh[v]^n)^p /;
IntegersQ[m,n,p]

(* Basis: a+b*Coth[z]+c*Csch[z] == (c+b*Cosh[z]+a*Sinh[z])/Sinh[z] *)
TrigSimplifyAux[u_.*Sinh[v_]^m_.*(a_.+b_.*Coth[v_]^n_.+c_.*Csch[v_]^n_.)^p_.] :=
  u*Sinh[v]^(m-n*p)*(c+b*Cosh[v]^n+a*Sinh[v]^n)^p /;
IntegersQ[m,n,p]

(* Basis: a+b*Coth[z]+c*Csch[z] == Csch[z]*(c+b*Cosh[z]+a*Sinh[z]) *)
TrigSimplifyAux[u_.*Csch[v_]^m_.*(a_.+b_.*Coth[v_]^n_.+c_.*Csch[v_]^n_.)^p_.] :=
  u*Csch[v]^(m+n*p)*(c+b*Cosh[v]^n+a*Sinh[v]^n)^p /;
IntegersQ[m,n,p]


(* Basis: If n is an integer, Tanh[z]^n/(a+b*Sech[z]^n) == Sinh[z]^n/(b+a*Cosh[z]^n) *)
TrigSimplifyAux[u_.*Tanh[v_]^n_./(a_+b_.*Sech[v_]^n_.)] :=
  u*Sinh[v]^n/(b+a*Cosh[v]^n) /;
IntegerQ[n] && n>0 && NonsumQ[a]

(* Basis: If n is an integer, Coth[z]^n/(a+b*Csch[z]^n) == Cosh[z]^n/(b+a*Sinh[z]^n) *)
TrigSimplifyAux[u_.*Coth[v_]^n_./(a_+b_.*Csch[v_]^n_.)] :=
  u*Cosh[v]^n/(b+a*Sinh[v]^n) /;
IntegerQ[n] && n>0 && NonsumQ[a]


(* Basis: If n is an integer, Sinh[z]^(-n)*(a*Cosh[z]^n+b*Sinh[z]^n) == b+a*Coth[z]^n *)
TrigSimplifyAux[Sinh[v_]^m_.*(a_.*Cosh[v_]^n_.+b_.*Sinh[v_]^n_.)^p_] :=
  (b+a*Coth[v]^n)^p /;
IntegersQ[m,n,p] && n>0 && p<0 && m==-n*p

(* Basis: If n is an integer, Cosh[z]^(-n)*(a*Cosh[z]^n+b*Sinh[z]^n) == a+b*Tanh[z]^n *)
TrigSimplifyAux[Cosh[v_]^m_.*(a_.*Cosh[v_]^n_.+b_.*Sinh[v_]^n_.)^p_] :=
  (a+b*Tanh[v]^n)^p /;
IntegersQ[m,n,p] && n>0 && p<0 && m==-n*p


(* Basis: If a^2-b^2=0, 1/(a*Cosh[z] + b*Sinh[z]) == Cosh[z]/a - Sinh[z]/b *)
TrigSimplifyAux[(a_.*Cosh[v_]+b_.*Sinh[v_])^n_] :=
  (Cosh[v]/a - Sinh[v]/b)^(-n) /;
IntegerQ[n] && n<0 && ZeroQ[a^2-b^2]


TrigSimplifyAux[u_] := u


(* ::Subsection::Closed:: *)
(*SmartTrigExpand*)


(* ::Subsubsection::Closed:: *)
(*Circular trig function expansion rules*)


(* Basis: If n>0 is even, Sin[n*z] == 
	Sum[(-1)^k*2^(2*k+1)*Binomial[n/2+k,n/2-k-1]*Cos[z]*Sin[z]^(2*k+1),{k,0,n/2-1}] *)
(* SmartTrigExpand[Sin[n_*u_],x_Symbol] :=
  Sum[(-1)^k*2^(2*k+1)*Binomial[n/2+k,n/2-k-1]*Cos[u]*Sin[u]^(2*k+1),
	{k,0,n/2-1}] /;
EvenQ[n] && n>1 && Not[FreeQ[u,x]]

(* Basis: If n>0 is odd, Sin[n*z] == 
	Sum[(-1)^k*2^(2*k)*n/((n+1)/2+k)*Binomial[(n+1)/2+k,(n+1)/2-k-1]*Sin[z]^(2*k+1),{k,0,(n-1)/2}] *)
SmartTrigExpand[Sin[n_*u_],x_Symbol] :=
  Sum[(-1)^k*2^(2*k)*n/((n+1)/2+k)*Binomial[(n+1)/2+k,(n+1)/2-k-1]*Sin[u]^(2*k+1),
	{k,0,(n-1)/2}] /;
OddQ[n] && n>1 && Not[FreeQ[u,x]] *)


(* Basis: If n>0 is even, Cos[n*z] == 
	Sum[(-1)^(n/2+k)*2^(2*k)*n/(n+2*k)*Binomial[n/2+k,n/2-k]*Cos[z]^(2*k),{k,0,n/2}] *)
(* SmartTrigExpand[Cos[n_*u_],x_Symbol] :=
  Sum[(-1)^(n/2+k)*2^(2*k)*n/(n+2*k)*Binomial[n/2+k,n/2-k]*Cos[u]^(2*k),
	{k,0,n/2}] /;
EvenQ[n] && n>1 && Not[FreeQ[u,x]]

(* Basis: If n>0 is odd, Cos[n*z] == 
	Sum[(-1)^((n+1)/2+k-1)*2^(2*k)*n/((n+1)/2+k)*Binomial[(n+1)/2+k,(n+1)/2-k-1]*Cos[z]^(2*k+1),{k,0,(n-1)/2}] *)
SmartTrigExpand[Cos[n_*u_],x_Symbol] :=
  Sum[(-1)^((n+1)/2+k-1)*2^(2*k)*n/((n+1)/2+k)*Binomial[(n+1)/2+k,(n+1)/2-k-1]*Cos[u]^(2*k+1),
	{k,0,(n-1)/2}] /;
OddQ[n] && n>1 && Not[FreeQ[u,x]] *)


(* Basis: If n>0 is even, Sec[z]*Sin[n*z] == 
		Sum[(-1)^(k+n/2)*2*Sin[(2*k-1)*z], {k,1,n/2}] *)
SmartTrigExpand[Sec[u_]*Sin[n_*u_],x_Symbol] :=
  Sum[(-1)^(k+n/2)*2*Sin[(2*k-1)*u], {k,1,n/2}] /;
EvenQ[n] && n>1 && Not[FreeQ[u,x]]

(* Basis: If n>0 is odd, Sec[z]*Sin[n*z] == 
		(-1)^((n-1)/2)*Tan[z] + Sum[(-1)^(k+(n-1)/2)*2*Sin[2*k*z], {k,1,(n-1)/2}] *)
SmartTrigExpand[Sec[u_]*Sin[n_*u_],x_Symbol] :=
  (-1)^((n-1)/2)*Tan[u] + Sum[(-1)^(k+(n-1)/2)*2*Sin[2*k*u], {k,1,(n-1)/2}] /;
OddQ[n] && n>1 && Not[FreeQ[u,x]]


(* Basis: If n>0 is even, Csc[z]*Cos[n*z] == 
		Csc[z] - Sum[2*Sin[(2*k-1)*z], {k,1,n/2}] *)
SmartTrigExpand[Csc[u_]*Cos[n_*u_],x_Symbol] :=
  Csc[u] - Sum[2*Sin[(2*k-1)*u], {k,1,n/2}] /;
EvenQ[n] && n>1 && Not[FreeQ[u,x]]

(* Basis: If n>0 is odd, Csc[z]*Cos[n*z] == 
		Cot[z] - Sum[2*Sin[2*k*z], {k,1,(n-1)/2}] *)
SmartTrigExpand[Csc[u_]*Cos[n_*u_],x_Symbol] :=
  Cot[u] - Sum[2*Sin[2*k*u], {k,1,(n-1)/2}] /;
OddQ[n] && n>1 && Not[FreeQ[u,x]]


(* ::Subsubsection::Closed:: *)
(*Hyperbolic trig function expansion rules*)


(* Basis: If n>0 is even, Sinh[n*z] == 
	Sum[2^(2*k+1)*Binomial[n/2+k,n/2-k-1]*Cosh[z]*Sinh[z]^(2*k+1),{k,0,n/2-1}] *)
(* SmartTrigExpand[Sinh[n_*u_],x_Symbol] :=
  Sum[2^(2*k+1)*Binomial[n/2+k,n/2-k-1]*Cosh[u]*Sinh[u]^(2*k+1),{k,0,n/2-1}] /;
EvenQ[n] && n>1 && Not[FreeQ[u,x]]

(* Basis: If n>0 is odd, Sinh[n*z] == 
	Sum[2^(2*k)*n/((n+1)/2+k)*Binomial[(n+1)/2+k,(n+1)/2-k-1]*Sinh[z]^(2*k+1),{k,0,(n-1)/2}] *)
SmartTrigExpand[Sinh[n_*u_],x_Symbol] :=
  Sum[2^(2*k)*n/((n+1)/2+k)*Binomial[(n+1)/2+k,(n+1)/2-k-1]*Sinh[u]^(2*k+1),{k,0,(n-1)/2}] /;
OddQ[n] && n>1 && Not[FreeQ[u,x]] *)


(* Basis: If n>0 is even, Cosh[n*z] == 
	Sum[(-1)^(n/2+k)*2^(2*k)*n/(n+2*k)*Binomial[n/2+k,n/2-k]*Cosh[z]^(2*k),{k,0,n/2}] *)
(* SmartTrigExpand[Cosh[n_*u_],x_Symbol] :=
  Sum[(-1)^(n/2+k)*2^(2*k)*n/(n+2*k)*Binomial[n/2+k,n/2-k]*Cosh[u]^(2*k),{k,0,n/2}] /;
EvenQ[n] && n>1 && Not[FreeQ[u,x]]

(* Basis: If n>0 is odd, Cosh[n*z] == 
	Sum[(-1)^((n+1)/2+k-1)*2^(2*k)*n/((n+1)/2+k)*Binomial[(n+1)/2+k,(n+1)/2-k-1]*Cosh[z]^(2*k+1),{k,0,(n-1)/2}] *)
SmartTrigExpand[Cosh[n_*u_],x_Symbol] :=
  Sum[(-1)^((n+1)/2+k-1)*2^(2*k)*n/((n+1)/2+k)*Binomial[(n+1)/2+k,(n+1)/2-k-1]*Cosh[u]^(2*k+1),
	{k,0,(n-1)/2}] /;
OddQ[n] && n>1 && Not[FreeQ[u,x]] *)


(* Basis: If n>0 is even, Sech[z]*Sinh[n*z] == 
		Sum[(-1)^(k+n/2)*2*Sinh[(2*k-1)*z], {k,1,n/2}] *)
SmartTrigExpand[Sech[u_]*Sinh[n_*u_],x_Symbol] :=
  Sum[(-1)^(k+n/2)*2*Sinh[(2*k-1)*u], {k,1,n/2}] /;
EvenQ[n] && n>1 && Not[FreeQ[u,x]]

(* Basis: If n>0 is odd, Sech[z]*Sinh[n*z] == 
		(-1)^((n-1)/2)*Tanh[z] + Sum[(-1)^(k+(n-1)/2)*2*Sinh[2*k*z], {k,1,(n-1)/2}] *)
SmartTrigExpand[Sech[u_]*Sinh[n_*u_],x_Symbol] :=
  (-1)^((n-1)/2)*Tanh[u] + Sum[(-1)^(k+(n-1)/2)*2*Sinh[2*k*u], {k,1,(n-1)/2}] /;
OddQ[n] && n>1 && Not[FreeQ[u,x]]


(* Basis: If n>0 is even, Csch[z]*Cosh[n*z] == 
		Csch[z] + Sum[2*Sinh[(2*k-1)*z], {k,1,n/2}] *)
SmartTrigExpand[Csch[u_]*Cosh[n_*u_],x_Symbol] :=
  Csch[u] + Sum[2*Sinh[(2*k-1)*u], {k,1,n/2}] /;
EvenQ[n] && n>1 && Not[FreeQ[u,x]]

(* Basis: If n>0 is odd, Csch[z]*Cosh[n*z] == 
		Coth[z] + Sum[2*Sinh[2*k*z], {k,1,(n-1)/2}] *)
SmartTrigExpand[Csch[u_]*Cosh[n_*u_],x_Symbol] :=
  Coth[u] + Sum[2*Sinh[2*k*u], {k,1,(n-1)/2}] /;
OddQ[n] && n>1 && Not[FreeQ[u,x]]


(* Basis: If n>0 is even, Csch[z]^2*Cosh[n*z] == 
		n + Csch[z]^2 + Sum[4*(n/2-k)*Cosh[2*k*z], {k,1,n/2}] *)
SmartTrigExpand[Csch[u_]^2*Cosh[n_*u_],x_Symbol] :=
  n + Csch[u]^2 + Sum[4*(n/2-k)*Cosh[2*k*u], {k,1,n/2}] /;
EvenQ[n] && n>1 && Not[FreeQ[u,x]]

(* Basis: If n>0 is odd, Csch[z]^2*Cosh[n*z] == 
		Coth[z]*Csch[z] + Sum[4*((n+1)/2-k)*Cosh[(2*k-1)*z], {k,1,(n+1)/2}] *)
SmartTrigExpand[Csch[u_]^2*Cosh[n_*u_],x_Symbol] :=
  Coth[u]*Csch[u] + Sum[4*((n+1)/2-k)*Cosh[(2*k-1)*u], {k,1,(n+1)/2}] /;
OddQ[n] && n>1 && Not[FreeQ[u,x]]


(* Basis: If n>0 is odd, Csch[z]^3*Cosh[n*z] == 
	(n^2-1)*Coth[z]/2 + Coth[z]*Csch[z]^2 + Sum[8*Binomial[(n+1)/2-k,2]*Sinh[2*k*z], {k,1,(n-3)/2}] *)
SmartTrigExpand[Csch[u_]^3*Cosh[n_*u_],x_Symbol] :=
  (n^2-1)*Coth[u]/2 + Coth[u]*Csch[u]^2 + 
	Sum[8*Binomial[(n+1)/2-k,2]*Sinh[2*k*u], {k,1,(n-3)/2}] /;
OddQ[n] && n>1 && Not[FreeQ[u,x]]


(* ::Subsubsection::Closed:: *)
(*Products and powers of trig function expansion rules*)


SmartTrigExpand[u_^n_,x_Symbol] :=
  Module[{tmp=SmartTrigExpand[u,x]},
  Expand[tmp^n,x] /;
 SumQ[tmp]] /;
NonsumQ[u] && IntegerQ[n] && n>1


SmartTrigExpand[u_*v_,x_Symbol] :=
  Module[{tmp1=SmartTrigExpand[u,x], tmp2=SmartTrigExpand[v,x]},
  Distribute[tmp1*tmp2] /;
 SumQ[tmp1] || SumQ[tmp2]] /;
NonsumQ[u] && NonsumQ[v]


(* TrigExpand[u] returns a bottom-up trig multiple-angle expansion of u. *)
SmartTrigExpand[u_,x_Symbol] :=
  u


(* ::Subsection::Closed:: *)
(*Sim*)


Sim[u_] := Together[u]


Sim[u_,x_] := 
  If[SumQ[u],
    Module[{tmp1=0,tmp2=0,lst},
    Scan[Function[lst=SplitFreeFactors[#,x];If[lst[[2]]===1,tmp1+=#,tmp2=tmp2+Together[lst[[1]]]*lst[[2]]]],u];
    Together[tmp1]+tmp2],
  Module[{lst=SplitFreeFactors[u,x]},
  Together[lst[[1]]]*lst[[2]]]]


(* ::Subsection::Closed:: *)
(*Simp*)


(* ::Subsubsection::Closed:: *)
(*Bottom-up simplification routine*)


SimpFlag=True;


SimpLite[u_,x_] := 
  If[SimpFlag,
    Simp[u],
  If[SumQ[u],
    Module[{tmp1=0,tmp2=0,lst},
    Scan[Function[lst=SplitFreeFactors[SimpLiteTerm[#],x];If[lst[[2]]===1,tmp1+=#,tmp2+=SimpAux[TogetherFix[Together[lst[[1]]]]]*lst[[2]]]],u];
    SimpAux[TogetherFix[Together[tmp1]]]+tmp2],
  Module[{lst=SplitFreeFactors[SimpLiteTerm[u],x]},
  SimpAux[TogetherFix[Together[lst[[1]]]]]*lst[[2]]]]]


(* Basis: If m is an integer, (a+b*z)^m/z^m == (b+a/z)^m *)
SimpLiteTerm[u_.*(a_+b_.*f_[v_])^m_.*g_[v_]^m_.] :=
  SimpLiteTerm[u*(b+a*g[v])^m] /;
TrigQ[f] && TrigQ[g] && f[v]===1/g[v] && IntegerQ[m]

SimpLiteTerm[u_] := u


TogetherFix[u_.*v_^n_.] :=
  If[EvenQ[n],
    TogetherFix[u]*(-v)^n,
  TogetherFix[-u]*(-v)^n] /;
IntegerQ[n] && SumQ[v] && MatchQ[First[v], m_*w_. /; RationalQ[m] && m<0]

TogetherFix[u_./(a_*v_.+a_^n_.*w_.)] :=
  TogetherFix[u/a]/(v+a^(n-1)*w) /;
IntegerQ[n] && n>0

TogetherFix[u_] := u


(* Simp[expn] returns a bottom-up simplification of expn. *)
Simp[expn_] :=
  If[AtomQ[expn],
    expn,
  If[Head[expn]===If || Head[expn]===Int || Head[expn]===Dif,
    expn,
  SimpAux[Map[Simp,expn]]]]


DownValues[SimpAux]={};
SimpAux[expn_] :=
  expn


SimpAux[expn_] :=
  Module[{tmp=ContentFactor[expn],lst,i=1,j},
  If[tmp=!=expn,
    tmp=Simp[tmp];
    If[ProductQ[tmp],
      ContentFactor[tmp],
    tmp],
  lst=Apply[List,expn];
  If[Length[lst]>=4 && 
     RationalQ[lst[[1]]] && 
     PowerQ[lst[[2]]] && RationalQ[lst[[2,1]]] && RationalQ[lst[[2,2]]] &&
     SqrtNumberSumQ[lst[[3]]] &&
     PowerQ[lst[[4]]] && SqrtNumberSumQ[lst[[4,1]]] && RationalQ[lst[[4,2]]],
    lst=Join[{lst[[3]],lst[[4]],lst[[1]],lst[[2]]}, Drop[lst,4]]];
  If[Length[lst]>=3 && 
     PowerQ[lst[[1]]] && RationalQ[lst[[1,1]]] && RationalQ[lst[[1,2]]] && 
     SqrtNumberSumQ[lst[[2]]] && 
     PowerQ[lst[[3]]] && SqrtNumberSumQ[lst[[3,1]]] && RationalQ[lst[[3,2]]],
    lst=Join[{lst[[2]],lst[[3]],lst[[1]]}, Drop[lst,3]]];
  While[True,
    If[i==Length[lst],
      Break[],
    j=i+1;
    While[True,
      If[j>Length[lst],
        i++;
        Break[],
      tmp=SimpProduct[lst[[i]],lst[[j]]];
      If[tmp=!=lst[[i]]*lst[[j]],
        If[ProductQ[tmp],
          lst[[i]]=First[tmp];
          lst[[j]]=Rest[tmp];
          Break[],
        lst[[i]]=tmp;
        lst=Delete[lst,j];
        i=1;
        Break[]],
      tmp=SimpProduct[lst[[j]],lst[[i]]];
      If[tmp=!=lst[[j]]*lst[[i]],
        If[ProductQ[tmp],
          lst[[i]]=First[tmp];
          lst[[j]]=Rest[tmp];
          Break[],
        lst[[i]]=tmp;
        lst=Delete[lst,j];
        i=1;
        Break[]],
      j++]]]]]];
  tmp=Apply[Times,lst];
  If[ProductQ[tmp],
    ContentFactor[tmp],
  tmp]]] /;
ProductQ[expn]


SimpAux[expn_] :=
  Module[{lst=Apply[List,expn],i=1,j,tmp},
  While[True,
    If[i==Length[lst],
      Break[],
    j=i+1;
    While[True,
      If[j>Length[lst],
        i++;
        Break[],
      tmp=SimpSum[lst[[i]],lst[[j]]];
      If[tmp=!=lst[[i]]+lst[[j]],
        If[SumQ[tmp],
          lst[[i]]=First[tmp];
          lst[[j]]=Rest[tmp];
          Break[],
        lst[[i]]=tmp;
        lst=Delete[lst,j];
        i=1;
        Break[]],
      tmp=SimpSum[lst[[j]],lst[[i]]];
      If[tmp=!=lst[[j]]+lst[[i]],
        If[SumQ[tmp],
          lst[[i]]=First[tmp];
          lst[[j]]=Rest[tmp];
          Break[],
        lst[[i]]=tmp;
        lst=Delete[lst,j];
        i=1;
        Break[]],
      j++]]]]]];
  Apply[Plus,lst]] /;
SumQ[expn]


DownValues[SimpProduct]={};
SimpProduct[u_,v_] :=
  u*v


DownValues[SimpSum]={};
SimpSum[u_,v_] :=
  u+v


(* ::Subsubsection::Closed:: *)
(*Integer powers of squareroot numbers simplification rules*)


(* Basis: 1/(a+b) == a/(a^2-b^2) - b/(a^2-b^2) *)
SimpAux[(a_+b_)^m_] :=
  SimpAux[(a/(a^2-b^2)-b/(a^2-b^2))^(-m)] /;
IntegerQ[m] && m<0 && SqrtNumberQ[a] && SqrtNumberQ[b]


(* Basis: If m is an integer, and a and b are squareroot numbers, 
	(a+b)^m == a^m+m*a^(m-1)*b+... == c+d, where c and d are squareroot numbers *)
SimpAux[u_^m_] :=
  Expand[u^m] /;
IntegerQ[m] && m>0 && SqrtNumberSumQ[u]


(* ::Subsubsection::Closed:: *)
(*Fractional powers of squareroot numbers simplification rules*)


(* Basis: If z<0, z^m == (-1)^m*(-z)^m *)
SimpAux[u_^m_] :=
  (-1)^m*SimpAux[(-u)^m] /;
FractionQ[m] && SqrtNumberSumQ[u] && NegativeQ[u]


(* Basis: If z>0, z^(m/n) == (z^m)^(1/n) *)
SimpAux[u_^m_] :=
  SimpAux[Expand[u^Denominator[1/m]]^(1/Numerator[1/m])] /;
FractionQ[m] && Denominator[1/m]>1 && SqrtNumberSumQ[u] && PositiveQ[u]


(* Basis: If m>0, (m*a+m*b)^n == m^n*(a+b)^n *)
SimpAux[(a_+b_)^m_] :=
  Module[{tmp=CommonNumericFactors[{a,b}]},
  tmp=Abs[NumericFactor[tmp[[1]]^(1/Denominator[m])]];
  tmp^Numerator[m]*SimpAux[(a/tmp^Denominator[m]+b/tmp^Denominator[m])^m] /;
 tmp!=1 ] /;
FractionQ[m] && Denominator[1/m]==1 && SqrtNumberQ[a] && SqrtNumberQ[b]


(* Basis: 1/(a+b) == a/(a^2-b^2) - b/(a^2-b^2) *)
SimpAux[(a_+b_)^m_] :=
  Module[{tmp1=Abs[Numerator[NumericFactor[(a^2-b^2)^(1/Denominator[m])]]],tmp2},
  tmp2=(a^2-b^2)/tmp1^Denominator[m];
  tmp1^Numerator[m]*SimpAux[(a/tmp2-b/tmp2)^(-m)] /;
 IntegerQ[a^2/tmp2^2] && IntegerQ[b^2/tmp2^2] && (tmp2^2==1 && m<0 || tmp2^2>1)] /;
FractionQ[m] && Denominator[1/m]==1 && SqrtNumberQ[a] && SqrtNumberQ[b] && PositiveQ[a^2-b^2] &&
IntegerQ[a^2] && IntegerQ[b^2]


(* Basis: c*(a+b) == c*a+c*b *)
SimpAux[(c_*(a_+b_))^m_] :=
  SimpAux[(c*a+c*b)^m] /;
FractionQ[m] && SqrtNumberQ[a] && SqrtNumberQ[b] && SqrtNumberQ[c]


(* Basis: If a,b>=0, Sqrt[a+b] == Sqrt[(a+Sqrt[a^2-b^2])/2] + Sqrt[(a-Sqrt[a^2-b^2])/2] *)
SimpAux[(a_+b_)^m_] :=
  Module[{q=Sqrt[a^2-b^2]},
  SimpAux[(Sqrt[(a+q)/2] + Sqrt[(a-q)/2])^(2*m)] /;
 RationalQ[q]] /;
EvenQ[1/m] && RationalQ[a] && a>0 && SqrtNumberQ[b] && PositiveQ[b] && PositiveQ[a^2-b^2]


(* Basis: If a,b>=0, Sqrt[a-b] == Sqrt[(a+Sqrt[a^2-b^2])/2] - Sqrt[(a-Sqrt[a^2-b^2])/2] *)
SimpAux[(a_+b_)^m_] :=
  Module[{q=Sqrt[a^2-b^2]},
  SimpAux[(Sqrt[(a+q)/2] - Sqrt[(a-q)/2])^(2*m)] /;
 RationalQ[q]] /;
EvenQ[1/m] && RationalQ[a] && a>0 && SqrtNumberQ[b] && NegativeQ[b] && PositiveQ[a^2-b^2]


(* ::Subsubsection::Closed:: *)
(*Products of squareroot numbers simplification rules*)


(* Basis: c*(a+b) == c*a+c*b *)
SimpProduct[c_,a_+b_] :=
  c*a+c*b /;
SqrtNumberQ[a] && SqrtNumberQ[b] && SqrtNumberQ[c] (* && Not[RationalQ[c]] *)


(* Basis: (a+b)*(c+d) == a*c+a*d+b*c+b*d *)
SimpProduct[a_+b_,c_+d_] :=
  Module[{u=a*c+a*d+b*c+b*d},
  u /;
 SqrtNumberSumQ[u]] /;
SqrtNumberQ[a] && SqrtNumberQ[b] && SqrtNumberQ[c] && SqrtNumberQ[d]


SimpProduct[c_,u_^m_] :=
  -SimpProduct[-c,u^m] /;
RationalQ[m] && SqrtNumberSumQ[u] && SqrtNumberQ[c] && NegativeQ[c]


(* Basis: If z<0, then z^m == (-1)^m*(-z)^m *)
SimpProduct[u_^m_,v_^n_] :=
  (-1)^m*SimpProduct[(-u)^m,v^n] /;
RationalQ[{m,n}] && SqrtNumberSumQ[u] && SqrtNumberSumQ[v] && NegativeQ[u]


(* Basis: If c>0 and m real, c*z^m == (c^(1/m)*z)^m *)
SimpProduct[c_,(a_+b_)^m_] :=
  SimpAux[(a*c^(1/m)+b*c^(1/m))^m] /;
RationalQ[m] && SqrtNumberQ[a] && SqrtNumberQ[b] && SqrtNumberQ[c^(1/m)] && PositiveQ[c] && 
Not[RationalQ[c]]


(* Basis: If z,w>0, then z^m*w^n == (z^(m/GCD[m,n])*w^(n/GCD[m,n]))^GCD[m,n] *)
SimpProduct[u_^m_,v_] :=
  Module[{gcd=GCD[m,1],w},
  w=Expand[u^(m/gcd)*(-v)^(1/gcd)];
  -SimpAux[w^gcd] /;
 SqrtNumberQ[w] || SqrtNumberSumQ[NonnumericFactors[w]]] /;
RationalQ[m] && m>0 && SqrtNumberSumQ[u] && SqrtNumberSumQ[v] && PositiveQ[u] && NegativeQ[v]


(* Basis: If z,w>0, then z^m*w^n == (z^(m/GCD[m,n])*w^(n/GCD[m,n]))^GCD[m,n] *)
SimpProduct[u_^m_,v_] :=
  Module[{gcd=GCD[m,1],w},
  w=Simp[u^(m/gcd)*(-v)^(1/gcd)];
  -SimpAux[w^gcd] /;
 SqrtNumberQ[w] || SqrtNumberSumQ[NonnumericFactors[w]]] /;
RationalQ[m] && m<0 && SqrtNumberSumQ[u] && SqrtNumberSumQ[v] && PositiveQ[u] && NegativeQ[v]


(* Basis: If z,w>0, then z^m*w^n == (z^(m/GCD[m,n])*w^(n/GCD[m,n]))^GCD[m,n] *)
SimpProduct[u_^m_.,v_^n_.] :=
  Module[{gcd=GCD[m,n],w},
  w=Expand[u^(m/gcd)*v^(n/gcd)];
  SimpAux[w^gcd] /;
 SqrtNumberQ[w] || SqrtNumberSumQ[w]] /;
RationalQ[{m,n}] && m>0 && n>0 && SqrtNumberSumQ[u] && SqrtNumberSumQ[v] && PositiveQ[u] && 
PositiveQ[v]


(* Basis: If z,w>0, then z^m*w^n == (z^(m/GCD[m,n])*w^(n/GCD[m,n]))^GCD[m,n] *)
SimpProduct[u_^m_,v_^n_.] :=
  Module[{gcd=GCD[m,n],w},
  w=Simp[u^(m/gcd)*v^(n/gcd)];
  SimpAux[w^gcd] /;
 SqrtNumberQ[w] || SqrtNumberSumQ[w]] /;
RationalQ[{m,n}] && m<0 && n>0 && SqrtNumberSumQ[u] && SqrtNumberSumQ[v] && PositiveQ[u] && 
PositiveQ[v]


(* ::Subsubsection::Closed:: *)
(*Elementary operator simplification rules*)


(* Basis: If n is an integer, then z^n == (-1)^n*(-z)^n *)
SimpAux[u_^n_] :=
  (-1)^n*SimpAux[Map[Minus,u]^n] /;
SumQ[u] && IntegerQ[n] && NumericFactor[u[[1]]]<0


(* Basis: z == -(-z) *)
SimpProduct[u_,v_] :=
  -SimpAux[Map[Minus,u]*v] /;
SumQ[u] && NumericFactor[u[[1]]]<0 && Not[SqrtNumberSumQ[u] && SqrtNumberSumQ[v]]


(* Basis: If n is an integer, (m*a+m*b)^n == m^n*(a+b)^n *)
SimpAux[u_^n_] :=
  Module[{lst=CommonFactors[Apply[List,u]]},
  Simp[lst[[1]]^n]*SimpAux[Apply[Plus,Rest[lst]]^n] /;
 lst[[1]]=!=1] /;
SumQ[u] && IntegerQ[n]


(* Basis: If m>0, (m*a+m*b)^n == m^n*(a+b)^n *)
SimpAux[u_^n_] :=
  Module[{lst=CommonNumericFactors[Apply[List,u]]},
  lst[[1]]^n*SimpAux[Apply[Plus,Rest[lst]]^n] /;
 lst[[1]]=!=1] /;
SumQ[u] && Not[IntegerQ[n]] && Not[SqrtNumberSumQ[u]]


(* Basis: If n is an integer and a*d-b*c==0, (a+b)^m*(c+d)^n == (a+b)^(m+n)*(d/b)^n *)
SimpProduct[(a_+b_)^m_,(c_+d_)^n_.] :=
  Simp[(a+b)^(m+n)*(d/b)^n] /;
IntegerQ[n] && ZeroQ[a*d-b*c] && Not[SqrtNumberSumQ[a+b]]


(* Basis: If n is an integer, u^m*(-u)^n == (-1)^n*u^(m+n) *)
SimpProduct[u_^m_.,v_^n_.] :=
  (-1)^n*u^(m+n) /;
IntegerQ[n] && ZeroQ[u+v] && Not[RationalQ[u]] && 
	(Not[IntegerQ[m]] || SmartLeafCount[u]<=SmartLeafCount[v])


(* Basis: (a+b)*(a-b) == a^2-b^2 *)
SimpProduct[(a_+b_)^n_.,(c_+d_)^n_.] :=
  Simp[(a^2-b^2)^n] /;
ZeroQ[a-c] && ZeroQ[b+d] && IntegerQ[n]


(* Basis: a/(a+b)+b/(a+b) == 1 *)
SimpSum[a_/u_,b_/u_] :=
  1 /;
a+b===u


(* Basis: If a+b*c^2=0, a+b*(c+d)^2 == b*d*(2*c+d) *)
SimpSum[a_,b_.*(c_+d_)^2] :=
  Simp[b*d*(2*c+d)] /;
ZeroQ[a+b*c^2]


(* Put sum over a common denominator if a cancellation occurs. *)
(* Basis: a+b*(c+d)/(e+f) == (a*e+a*f+b*c+b*d)/(e+f) *)
SimpSum[a_,b_.*(c_.+d_)/(e_+f_)] :=
  SimpAux[ContentFactor[a*e+b*c]/(e+f)] /;
NonsumQ[a] && NonsumQ[b] && NonsumQ[c] && NonsumQ[d] && NonsumQ[e] && NonsumQ[f] && ZeroQ[a*f+b*d]


(* Basis: If p is an integer, v^m*(a+b*v^n)^p == v^(m+n*p)*(a/v^n+b)^p *)
SimpProduct[v_^m_.,(a_+b_.*v_^n_)^p_.] :=
  SimpAux[v^(m+n*p)*Simp[(a*v^(-n)+b)^p]] /;
IntegerQ[p] && RationalQ[{m,n}] && n<0 && Not[SqrtNumberSumQ[a+b*v^n]]


(* Basis: If n is an integer, c^m*(a+b)^n == (a*c^(m/n) + b*c^(m/n))^n *)
SimpProduct[c_^m_.,(a_.*c_^p_.+b_.*c_^q_.)^n_.] :=
  SimpAux[(a*c^(p+m/n) + b*c^(q+m/n))^n] /;
IntegerQ[n] && RationalQ[{m,p,q}]


(* Rules for combining similar terms of sums disguised by terms involving sums. *) 

(* Basis: u/Sqrt[n]+v*Sqrt[n] == (1+n*v/u)*u/Sqrt[n] *)
SimpSum[u_./Sqrt[n_],v_.*Sqrt[n_]] :=
  (1+n*NumericFactor[v]/NumericFactor[u])*u/Sqrt[n] /;
RationalQ[n] && NonnumericFactors[u]===NonnumericFactors[v]

(* Basis: u+v*(a+b) == (1+v*a/u)*u+v*b *)
SimpSum[u_,v_*(a_+b_)] :=
  Module[{tmp=SimpAux[v*a]},
  SimpAux[(1+NumericFactor[tmp]/NumericFactor[u])*u+SimpAux[v*b]] /;
 NonnumericFactors[tmp]===NonnumericFactors[u]] /;
NonsumQ[a] && NonsumQ[u] && NonsumQ[v]

(* Basis: u*(a+b)+v*(c+d) == u*((1+v*c/(u*a))*a+b) + v*d *)
SimpSum[u_.*(a_+b_),v_*(c_+d_)] :=
  Module[{tmp1=SimpAux[v*c], tmp2=SimpAux[u*a]},
  (tmp1=NumericFactor[tmp1]/NumericFactor[tmp2];
  If[IntegerQ[tmp1] || -2<tmp1<0,
    SimpAux[u*((1+tmp1)*a+b)+SimpAux[v*d]],
  SimpAux[SimpAux[u*b]+v*((1+1/tmp1)*c+d)]]) /;
 NonnumericFactors[tmp1]===NonnumericFactors[tmp2]] /;
NonsumQ[a] && NonsumQ[c] && NonsumQ[u] && NonsumQ[v]


(* ::Subsubsection::Closed:: *)
(*Exponential function simplification rules*)


(* Basis: E^(a*Log[z]+b) == z^a*E^b *)
SimpAux[E^(c_.*(a_.*Log[v_]+b_))] :=
  SimpAux[v^(a*c)]*SimpAux[E^(b*c)]


(* Basis: E^(2*I*ArcTan[z]) == -1+2/(1-I*z) *)
SimpAux[E^(Complex[0,n_]*ArcTan[v_])] :=
  Simp[(-1+2/(1-I*v))^(n/2)] /;
EvenQ[n] && n>0

(* Basis: E^(-2*I*ArcTan[z]) == -1+2/(1+I*z) *)
SimpAux[E^(Complex[0,n_]*ArcTan[v_])] :=
  Simp[(-1+2/(1+I*v))^(-n/2)] /;
EvenQ[n] && n<0

(* Basis: E^(2*I*ArcCot[z]) == 1-2/(1+I*z) *)
SimpAux[E^(Complex[0,n_]*ArcCot[v_])] :=
  Simp[(1-2/(1+I*v))^(n/2)] /;
EvenQ[n] && n>0

(* Basis: E^(-2*I*ArcCot[z]) == 1-2/(1-I*z) *)
SimpAux[E^(Complex[0,n_]*ArcCot[v_])] :=
  Simp[(1-2/(1-I*v))^(-n/2)] /;
EvenQ[n] && n<0


(* Basis: E^(2*ArcTanh[z]) == -1+2/(1-z) *)
SimpAux[E^(n_*ArcTanh[v_])] :=
  Simp[(-1+2/(1-v))^(n/2)] /;
EvenQ[n] && n>0

(* Basis: E^(-2*ArcTanh[z]) == -1+2/(1+z) *)
SimpAux[E^(n_*ArcTanh[v_])] :=
  Simp[(-1+2/(1+v))^(-n/2)] /;
EvenQ[n] && n<0

(* Basis: E^(2*ArcCoth[z]) == 1-2/(1-z) *)
SimpAux[E^(n_*ArcCoth[v_])] :=
  Simp[(1-2/(1-v))^(n/2)] /;
EvenQ[n] && n>0

(* Basis: E^(-2*ArcCoth[z]) == 1-2/(1+z) *)
SimpAux[E^(n_*ArcCoth[v_])] :=
  Simp[(1-2/(1+v))^(-n/2)] /;
EvenQ[n] && n<0


(* Basis: If m and n are odd, E^(n*ArcTanh[z])*(1-z^2)^(m/2) == (1-z)^((m-n)/2)*(1+z)^((m+n)/2) *)
SimpProduct[E^(n_.*ArcTanh[v_]),(1+w_)^m_] :=
  Simp[(1-v)^(m-n/2)*(1+v)^(m+n/2)] /;
OddQ[n] && HalfIntegerQ[m] && ZeroQ[v^2+w]

(* Basis: If m and n are odd, E^(n*ArcCoth[z])*(1-1/z^2)^(m/2) == (-1+z)^((m-n)/2)*(1+z)^((m+n)/2)/z^m *)
SimpProduct[E^(n_.*ArcCoth[v_]),(1+w_)^m_] :=
  Simp[(-1+v)^(m-n/2)*(1+v)^(m+n/2)/v^(2*m)] /;
OddQ[n] && HalfIntegerQ[m] && ZeroQ[v^2+1/w]


(* ::Subsubsection::Closed:: *)
(*Circular trig function simplification rules*)


(* Basis: Sin[z]^2+Cos[z]^2 == 1 *)
SimpSum[u_.*Cos[z_]^2,v_.*Sin[z_]^2] :=
  u /;
u===v

(* Basis: Sec[z]^2-Tan[z]^2 == 1 *)
SimpSum[u_.*Sec[z_]^2,v_.*Tan[z_]^2] :=
  u /;
u===-v

(* Basis: Csc[z]^2-Cot[z]^2 == 1 *)
SimpSum[u_.*Csc[z_]^2,v_.*Cot[z_]^2] :=
  u /;
u===-v


(* Basis: 1-Sin[z]^2 == Cos[z]^2 *)
SimpSum[u_,v_.*Sin[z_]^2] :=
  u*Cos[z]^2 /;
u===-v

(* Basis: 1-Cos[z]^2 == Sin[z]^2 *)
SimpSum[u_,v_.*Cos[z_]^2] :=
  u*Sin[z]^2 /;
u===-v

(* Basis: 1+Tan[z]^2 == Sec[z]^2 *)
SimpSum[u_,v_.*Tan[z_]^2] :=
  u*Sec[z]^2 /;
u===v

(* Basis: 1+Cot[z]^2 == Csc[z]^2 *)
SimpSum[u_,v_.*Cot[z_]^2] :=
  u*Csc[z]^2 /;
u===v

(* Basis: -1+Sec[z]^2 == Tan[z]^2 *)
SimpSum[u_,v_.*Sec[z_]^2] :=
  v*Tan[z]^2 /;
u===-v

(* Basis: -1+Csc[z]^2 == Cot[z]^2 *)
SimpSum[u_,v_.*Csc[z_]^2] :=
  v*Cot[z]^2 /;
u===-v


(* Basis: If a^2+b^2=0, 1/(a*Cos[z] + b*Sin[z]) == Cos[z]/a + Sin[z]/b *)
SimpAux[(a_.*Cos[v_]+b_.*Sin[v_])^n_] :=
  SimpAux[(Cos[v]/a + Sin[v]/b)^(-n)] /;
IntegerQ[n] && n<0 && ZeroQ[a^2+b^2]


(* ::Subsubsection::Closed:: *)
(*Hyperbolic trig function simplification rules*)


(* Basis: Cosh[z]^2-Sinh[z]^2 == 1 *)
SimpSum[u_.*Cosh[z_]^2,v_.*Sinh[z_]^2] :=
  u /;
u===-v

(* Basis: Sech[z]^2+Tanh[z]^2 == 1 *)
SimpSum[u_.*Sech[z_]^2,v_.*Tanh[z_]^2] :=
  u /;
u===v

(* Basis: Coth[z]^2-Csch[z]^2 == 1 *)
SimpSum[u_.*Coth[z_]^2,v_.*Csch[z_]^2] :=
  u /;
u===-v


(* Basis: 1+Sinh[z]^2 == Cosh[z]^2 *)
SimpSum[u_,v_.*Sinh[z_]^2] :=
  u*Cosh[z]^2 /;
u===v

(* Basis: -1+Cosh[z]^2 == Sinh[z]^2 *)
SimpSum[u_,v_.*Cosh[z_]^2] :=
  v*Sinh[z]^2 /;
u===-v

(* Basis: 1-Tanh[z]^2 == Sech[z]^2 *)
SimpSum[u_,v_.*Tanh[z_]^2] :=
  u*Sech[z]^2 /;
u===-v

(* Basis: -1+Coth[z]^2 == Csch[z]^2 *)
SimpSum[u_,v_.*Coth[z_]^2] :=
  v*Csch[z]^2 /;
u===-v

(* Basis: 1-Sech[z]^2 == Tanh[z]^2 *)
SimpSum[u_,v_.*Sech[z_]^2] :=
  u*Tanh[z]^2 /;
u===-v

(* Basis: 1+Csch[z]^2 == Coth[z]^2 *)
SimpSum[u_,v_.*Csch[z_]^2] :=
  u*Coth[z]^2 /;
u===v


(* Basis: If a^2-b^2=0, 1/(a*Cosh[z] + b*Sinh[z]) == Cosh[z]/a - Sinh[z]/b *)
SimpAux[(a_.*Cosh[v_]+b_.*Sinh[v_])^n_] :=
  SimpAux[(Cosh[v]/a - Sinh[v]/b)^(-n)] /;
IntegerQ[n] && n<0 && ZeroQ[a^2-b^2]


(* ::Subsubsection::Closed:: *)
(*Ugly function simplification rules*)


(* Factor out common factors of ugly functions from a sum *) 
SimpSum[u_.*f_[a_]^n_.,v_.*f_[a_]^n_.] :=
  SimpAux[Simp[Simplify[u+v]]*f[a]^n] /;
MemberQ[{Erf, Erfc, Erfi, FresnelS, FresnelC, 
ExpIntegralEi, SinIntegral, CosIntegral, SinhIntegral, CoshIntegral, LogIntegral},f]

SimpSum[u_.*f_[a_,b_]^n_.,v_.*f_[a_,b_]^n_.] :=
  SimpAux[Simp[Simplify[u+v]]*f[a,b]^n] /;
MemberQ[{Int, Gamma, PolyLog, EllipticF, EllipticE},f]

SimpSum[u_.*f_[a_,b_,c_]^n_.,v_.*f_[a_,b_,c_]^n_.] :=
  SimpAux[Simp[Simplify[u+v]]*f[a,b,c]^n] /;
MemberQ[{EllipticPi},f]


(* ::Section::Closed:: *)
(*Expansion Functions*)


(* ::Subsection::Closed:: *)
(*ExpandIntegrandQ*)


(* If the integrand x^m*(a+b*x^n)^p should be expanded, ExpandIntegrandQ[m,n,p] returns True;
	else it returns False. *)
ExpandIntegrandQ[m_,n_,p_] :=
  IntegerQ[p] && p>0 && NonzeroQ[m-n+1] &&
	If[ZeroQ[n-1],
	   Not[IntegerQ[m]] || m<0 && Not[m+p+2<=0<=m+2*p+2] || p<=m+2,
	p==2 || Not[IntegerQ[(m+1)/n]] || Not[0<(m+1)/n<=3] && Not[(m+1)/n<=-(p+1)]]


(* ::Subsection::Closed:: *)
(*ExpandExpression*)


ExpnExpand[u_,x_Symbol] :=
  ExpnExpandAux[ExpandExpression[u,x],x]


(* Basis: e/x + f/(c+d*x) == (c*e + (d*e+f)*x)/(x*(c+d*x)) *)
ExpnExpandAux[u_.+e_./x_+f_./(c_+d_.*x_),x_Symbol] :=
  ExpnExpandAux[u,x]+c*e/(x*(c+d*x)) /;
FreeQ[{c,d,e,f},x] && ZeroQ[d*e+f]

(* Basis: e/(a+b*x) + f/(c+d*x) == (c*e+a*f + (d*e+b*f)*x)/(a*c + (b*c+a*d)*x + b*d*x^2) *)
ExpnExpandAux[u_.+e_./(a_+b_.*x_)+f_./(c_+d_.*x_),x_Symbol] :=
  ExpnExpandAux[u,x]+(c*e+a*f)/(a*c+b*d*x^2) /;
FreeQ[{a,b,c,d,e,f},x] && ZeroQ[d*e+b*f] && ZeroQ[b*c+a*d]

ExpnExpandAux[u_,x_Symbol] :=
  u


DownValues[ExpandExpression]={};


(* ::Subsubsection:: *)
(*Algebraic expansion rules*)


(* ExpandExpression[a_,x_Symbol] :=
  a /;
FreeQ[a,x]

ExpandExpression[a_*u_,x_Symbol] :=
  Dist[a,ExpandExpression[u,x]] /;
FreeQ[a,x] && Not[MatchQ[u,b_*v_ /; FreeQ[b,x]]] *)


(* Derivation: Algebraic simplification *)
(* Basis: a+b*x^n == x^n*(b+a/x^n) *)
ExpandExpression[u_.*(a_+b_.*x_^n_.)^p_,x_Symbol] :=
  ExpandExpression[u*x^(n*p)*(b+a/x^n)^p,x] /;
FreeQ[{a,b},x] && IntegersQ[n,p] && n<0


(* Derivation: Algebraic simplification *)
(* Basis: a*z^p+b*z^q == z^p*(a+b*z^(q-p)) *)
ExpandExpression[u_.*(a_.*x_^p_.+b_.*x_^q_.)^n_,x_Symbol] :=
  ExpandExpression[u*x^(n*p)*(a+b*x^(q-p))^n,x] /;
FreeQ[{a,b},x] && IntegersQ[n,p,q] && q-p>=0


(* Note: These rule is necessary because if a or b contains fractional powers, Apart rationalizes 
	denominator resulting in hard to integrate terms in partial fraction expansion. *)
ExpandExpression[u_.*(a_.+d_.*c_^m_+b_.*v_)^p_,x_Symbol] :=
  Module[{tmp},
  ReplaceAll[ExpandExpression[u*(a+d*tmp+b*v)^p,x],{tmp->c^m}]] /;
FreeQ[{a,b,c,d},x] && IntegerQ[p] && p<0 && FractionQ[m] && Not[FreeQ[v,x]]

ExpandExpression[u_.*(a_.+b_.*c_^m_*v_)^p_,x_Symbol] :=
  Module[{tmp},
  ReplaceAll[ExpandExpression[u*(a+b*tmp*v)^p,x],{tmp->c^m}]] /;
FreeQ[{a,b,c},x] && IntegerQ[p] && p<0 && FractionQ[m] && Not[FreeQ[v,x]]


(* Note: This rule necessary because if a+b*x^n is factorable, Apart factors denominator 
	resulting in hard to integrate terms in partial fraction expansion. *)
ExpandExpression[(c_.+d_.*x_^m_.)^q_.*(a_+b_.*x_^n_)^p_,x_Symbol] :=
  Module[{aa,bb},
  RegularizeTerm[ReplaceAll[Apart[(c+d*x^m)^q*(aa+bb*x^n)^p,x],{aa->a,bb->b}],x]] /;
FreeQ[{a,b,c,d},x] && IntegersQ[m,n,p,q] && p<0 && n>1 && (m>=n || m<0) && q>0


(* Derivation: Algebraic expansion *)
(* Basis: (a+b*z)/(c+d*z) == a/c + (b*c-a*d)*z/(c*(c+d*z)) *)
ExpandExpression[x_^m_*(a_+b_.*x_^n_.)/(c_+d_.*x_^n_.),x_Symbol] :=
  a*x^m/c + Dist[(b*c-a*d)/c, ExpandExpression[x^(m+n)/(c+d*x^n),x]] /;
FreeQ[{a,b,c,d},x] && IntegersQ[m,n] && n>0 && m<0


(* Derivation: Algebraic simplification *)
(* Basis: a*z^p+b*z^q+c*z^r == z^p*(a+b*z^(q-p)+c*z^(r-p)) *) 
(* ExpandExpression[u_.*(a_.*x_^p_.+b_.*x_^q_.+c_.*x_^r_.)^n_,x_Symbol] :=
  ExpandExpression[u*x^(n*p)*(a+b*x^(q-p)+c*x^(r-p))^n,x] /;
FreeQ[{a,b,c},x] && IntegersQ[n,p,q,r] && q-p>=0 && r-p>=0 *)


(* Derivation: Algebraic simplification *)
(* Basis: b^2/(4*c)+b*z+c*z^2 == (b/2+c*z)^2/c *)
ExpandExpression[u_.*(a_+b_.*x_^k_.+c_.*x_^j_)^n_,x_Symbol] :=
  ExpandExpression[u*(b/2+c*x^k)^(2*n)/c^n,x] /;
FreeQ[{a,b,c,j,k},x] && IntegersQ[n,k,j] && j==2*k && n<0 && ZeroQ[b^2-4*a*c]


(* Derivation: Algebraic normalization *)
ExpandExpression[u_^n_,x_Symbol] :=
  (Coefficient[u,x,0] + Coefficient[u,x,1]*x + Coefficient[u,x,2]*x^2)^n /;
RationalQ[n] && n<0 && PolynomialQ[u,x] && Exponent[u,x]==2 && 
Not[MatchQ[u,a_.+b_.*x+c_.*x^2 /; FreeQ[{a,b,c},x]]]


(* Derivation: Algebraic expansion *)
(* Basis: (c+d*x+e*x^2+f*x^3)/(a+b*x^4) == (c+e*x^2)/(a+b*x^4) + d*x/(a+b*x^4) + f*x^3/(a+b*x^4) *)
(* Note: Distributes denominator over terms in the numerator whose degrees+1 have a common 
	divisor with 4 making them easily integrable. *)
ExpandExpression[u_/(a_+b_.*x_^4),x_Symbol] :=
  (Coefficient[u,x,0]+Coefficient[u,x,2]*x^2)/(a+b*x^4) + 
  Coefficient[u,x,1]*x/(a+b*x^4) + 
  Coefficient[u,x,3]*x^3/(a+b*x^4) /;
FreeQ[{a,b},x] && PolynomialQ[u,x] && Exponent[u,x]<4


(* Note: Unlike Apart, the following rule does not expand (a+b*x)^n. *)
ExpandExpression[(a_.+b_.*x_)^m_*(c_+d_.*x_)^n_,x_Symbol] :=
  Map[Function[RegularizeSubst[#,x,a+b*x]], Apart[x^m*(c-a*d/b+d/b*x)^n,x]] /;
FreeQ[{a,b,c,d},x] && IntegersQ[m,n] && m>1 && n<0 (* && NonzeroQ[b*c-a*d] *)


ExpandExpression[x_^m_.*(a_.+b_.*x_)^n_*(c_+d_.*x_)^n_,x_Symbol] :=
  ExpandExpression[x^m*(a*c+(b*c+a*d)*x+b*d*x^2)^n,x] /;
FreeQ[{a,b,c,d},x] && IntegersQ[m,n] && n<0


(* Note: This rule is necessary because if a+b*x+c*x^2 is factorable, Apart factors denominator 
	resulting in hard to integrate terms in partial fraction expansion. *)
(* ExpandExpression[x_^m_.*(a_+b_.*x_^k_.+c_.*x_^j_)^p_,x_Symbol] :=
  ReplaceAll[Apart[x^m*(aa+bb*x^k+cc*x^j)^p,x],{aa->a,bb->b,cc->c}] /;
FreeQ[{a,b},x] && IntegersQ[m,p,j,k] && j===2*k && p<0 && k>0 && (m>=j || m<0) *)


(* ::Subsubsection::Closed:: *)
(*Circular trig expansion rules*)


ExpandExpression[(A_+u_+B_.*Sin[c_.+d_.*x_])*(a_.+b_.*Sin[c_.+d_.*x_])^n_,x_Symbol] :=
  (A+B*Sin[c+d*x])*(a+b*Sin[c+d*x])^n + ExpandExpression[u*(a+b*Sin[c+d*x])^n,x] /;
FreeQ[{a,b,c,d,A,B},x] && RationalQ[n] && Not[FreeQ[u,x]]

ExpandExpression[(A_+u_+B_.*Cos[c_.+d_.*x_])*(a_.+b_.*Cos[c_.+d_.*x_])^n_,x_Symbol] :=
  (A+B*Cos[c+d*x])*(a+b*Cos[c+d*x])^n + ExpandExpression[u*(a+b*Cos[c+d*x])^n,x] /;
FreeQ[{a,b,c,d,A,B},x] && RationalQ[n] && Not[FreeQ[u,x]]


ExpandExpression[Sin[v_]^n_,x_Symbol] :=
  Expand[TrigReduce[Sin[v]^n],x] /;
IntegerQ[n] && n>1

ExpandExpression[Cos[v_]^n_,x_Symbol] :=
  Expand[TrigReduce[Cos[v]^n],x] /;
IntegerQ[n] && n>1


(* Basis: Sin[z]^2/(c+d*Cos[z]^2) == -1/d + (c+d)/(d*(c+d*Cos[z]^2)) *)
ExpandExpression[Sin[v_]^n_/(c_.+d_.*Cos[v_]^2),x_Symbol] :=
  -Sin[v]^(n-2)/d + Dist[(c+d)/d,ExpandExpression[Sin[v]^(n-2)/(c+d*Cos[v]^2),x]] /;
FreeQ[{c,d},x] && EvenQ[n] && n>1

(* Basis: Cos[z]^2/(c+d*Sin[z]^2) == -1/d + (c+d)/(d*(c+d*Sin[z]^2)) *)
ExpandExpression[Cos[v_]^n_/(c_.+d_.*Sin[v_]^2),x_Symbol] :=
  -Cos[v]^(n-2)/d + Dist[(c+d)/d,ExpandExpression[Cos[v]^(n-2)/(c+d*Sin[v]^2),x]] /;
FreeQ[{c,d},x] && EvenQ[n] && n>1


(* Basis: (a+b*Sin[z]^2)/(c+d*Cos[z]^2) == -b/d + (b*c+(a+b)*d)/(d*(c+d*Cos[z]^2)) *)
ExpandExpression[(a_+b_.*Sin[v_]^2)/(c_.+d_.*Cos[v_]^2),x_Symbol] :=
  -b/d + (b*c+(a+b)*d)/(d*(c+d*Cos[v]^2)) /;
FreeQ[{a,b,c,d},x]

(* Basis: (a+b*Cos[z]^2)/(c+d*Sin[z]^2) == -b/d + (b*c+(a+b)*d)/(d*(c+d*Sin[z]^2)) *)
ExpandExpression[(a_+b_.*Cos[v_]^2)/(c_.+d_.*Sin[v_]^2),x_Symbol] :=
  -b/d + (b*c+(a+b)*d)/(d*(c+d*Sin[v]^2)) /;
FreeQ[{a,b,c,d},x]


(* Basis: If n is an integer, 
	Tan[z]^n/(a*Sin[z]^n+b*Cos[z]^n) == Sec[z]^n/a - b/(a*(a*Sin[z]^n+b*Cos[z]^n)) *)
ExpandExpression[Tan[v_]^n_./(a_.*Sin[v_]^n_.+b_.*Cos[v_]^n_.),x_Symbol] :=
  Sec[v]^n/a - b/(a*(a*Sin[v]^n+b*Cos[v]^n)) /;
FreeQ[{a,b},x] && IntegerQ[n]

(* Basis: If n is an integer, 
	Cot[z]^n/(a*Sin[z]^n+b*Cos[z]^n) == Csc[z]^n/b - a/(b*(a*Sin[z]^n+b*Cos[z]^n)) *)
ExpandExpression[Cot[v_]^n_./(a_.*Sin[v_]^n_.+b_.*Cos[v_]^n_.),x_Symbol] :=
  Csc[v]^n/b - a/(b*(a*Sin[v]^n+b*Cos[v]^n)) /;
FreeQ[{a,b},x] && IntegerQ[n]


(* Basis: If n is an integer, 
	Sec[z]^n/(a+b*Cot[z]^n) == Sec[z]^n/a - b/(a*(a*Sin[z]^n+b*Cos[z]^n)) *)
ExpandExpression[Sec[v_]^n_./(a_+b_.*Cot[v_]^n_.),x_Symbol] :=
  Sec[v]^n/a - b/(a*(a*Sin[v]^n+b*Cos[v]^n)) /;
FreeQ[{a,b},x] && IntegerQ[n]

(* Basis: If n is an integer, 
	Csc[z]^n/(a+b*Tan[z]^n) == Csc[z]^n/a - b/(a*(b*Sin[z]^n+a*Cos[z]^n)) *)
ExpandExpression[Csc[v_]^n_./(a_+b_.*Tan[v_]^n_.),x_Symbol] :=
  Csc[v]^n/a - b/(a*(b*Sin[v]^n+a*Cos[v]^n)) /;
FreeQ[{a,b},x] && IntegerQ[n]


(* Basis: If a^2+b^2=0, 1/(a+b*Tan[z]) == Cos[z]^2/a + Cos[z]*Sin[z]/b *)
ExpandExpression[u_*(a_+b_.*Tan[v_])^n_,x_Symbol] :=
  ExpandExpression[u*(Cos[v]^2/a + Cos[v]*Sin[v]/b)^(-n),x] /;
FreeQ[{a,b},x] && IntegerQ[n] && n<0 && ZeroQ[a^2+b^2]

(* Basis: If a^2+b^2=0, 1/(a+b*Cot[z]) == Sin[z]^2/a + Cos[z]*Sin[z]/b *)
ExpandExpression[u_*(a_+b_.*Cot[v_])^n_,x_Symbol] :=
  ExpandExpression[u*(Sin[v]^2/a + Cos[v]*Sin[v]/b)^(-n),x] /;
FreeQ[{a,b},x] && IntegerQ[n] && n<0 && ZeroQ[a^2+b^2]


ExpandExpression[u_*Sin[a_.+b_.*x_]*Cos[a_.+b_.*x_],x_Symbol] :=
  ExpandExpression[u*Sin[2*a+2*b*x]/2,x] /;
FreeQ[{a,b},x]


ExpandExpression[u_*Sec[a_.+b_.*x_]*Csc[a_.+b_.*x_],x_Symbol] :=
  ExpandExpression[2*u*Csc[2*a+2*b*x],x] /;
FreeQ[{a,b},x]


(* Basis: Cos[z]*Sin[z] ==  Sin[2*z]/2 *)
(* ExpandExpression[x_^m_.*Cos[u_]*Sin[u_],x_Symbol] :=
  ExpandExpression[x^m*Sin[Dist[2,u]]/2,x] /;
RationalQ[m] *)

(* Basis: Cos[z]*Sin[z] ==  Sin[2*z]/2 *)
ExpandExpression[v_*Cos[u_]*Sin[u_],x_Symbol] :=
  ExpandExpression[v*Sin[Dist[2,u]]/2,x] /;
MatchQ[v,x^m_ /; RationalQ[m]] || MatchQ[v,f_^w_ /; FreeQ[f,x] && LinearQ[w,x]]


(* ::Subsubsection::Closed:: *)
(*Hyperbolic trig expansion rules*)


ExpandExpression[(A_+u_+B_.*Sinh[c_.+d_.*x_])*(a_.+b_.*Sinh[c_.+d_.*x_])^n_,x_Symbol] :=
  (A+B*Sinh[c+d*x])*(a+b*Sinh[c+d*x])^n + Expand[u*(a+b*Sinh[c+d*x])^n,x] /;
FreeQ[{a,b,c,d,A,B},x] && RationalQ[n] && Not[FreeQ[u,x]]

ExpandExpression[(A_+u_+B_.*Cosh[c_.+d_.*x_])*(a_.+b_.*Cosh[c_.+d_.*x_])^n_,x_Symbol] :=
  (A+B*Cosh[c+d*x])*(a+b*Cosh[c+d*x])^n + Expand[u*(a+b*Cosh[c+d*x])^n,x] /;
FreeQ[{a,b,c,d,A,B},x] && RationalQ[n] && Not[FreeQ[u,x]]


(* Note: Temporary variable z used because TrigReduce expands hyperbolic functions of sums
	having numeric terms! *) 
ExpandExpression[Sinh[v_]^n_,x_Symbol] :=
  Module[{z},
  Expand[NormalForm[Subst[TrigReduce[Sinh[z]^n],z,v],x],x]] /;
IntegerQ[n] && n>1

ExpandExpression[x_^m_.*Sinh[v_]^n_.,x_Symbol] :=
  Module[{z},
  Expand[x^m*NormalForm[Subst[TrigReduce[Sinh[z]^n],z,v],x],x]] /;
FreeQ[m,x] && IntegerQ[n] && n>0

ExpandExpression[Cosh[v_]^n_,x_Symbol] :=
  Module[{z},
  Expand[NormalForm[Subst[TrigReduce[Cosh[z]^n],z,v],x],x]] /;
IntegerQ[n] && n>1

ExpandExpression[x_^m_.*Cosh[v_]^n_.,x_Symbol] :=
  Module[{z},
  Expand[x^m*NormalForm[Subst[TrigReduce[Cosh[z]^n],z,v],x],x]] /;
FreeQ[m,x] && IntegerQ[n] && n>0


(* Basis: Sinh[z]^2/(c+d*Cosh[z]^2) == 1/d - (c+d)/(d*(c+d*Cosh[z]^2)) *)
ExpandExpression[Sinh[v_]^n_/(c_.+d_.*Cosh[v_]^2),x_Symbol] :=
  Sinh[v]^(n-2)/d - Dist[(c+d)/d,ExpandExpression[Sinh[v]^(n-2)/(c+d*Cosh[v]^2),x]] /;
FreeQ[{c,d},x] && EvenQ[n] && n>1

(* Basis: Cosh[z]^2/(c+d*Sinh[z]^2) == 1/d - (c-d)/(d*(c+d*Sinh[z]^2)) *)
ExpandExpression[Cosh[v_]^n_/(c_.+d_.*Sinh[v_]^2),x_Symbol] :=
  Cosh[v]^(n-2)/d - Dist[(c-d)/d,ExpandExpression[Cosh[v]^(n-2)/(c+d*Sinh[v]^2),x]] /;
FreeQ[{c,d},x] && EvenQ[n] && n>1


(* Basis: (a+b*Sinh[z]^2)/(c+d*Cosh[z]^2) == b/d - (b*c-(a-b)*d)/(d*(c+d*Cosh[z]^2)) *)
ExpandExpression[(a_+b_.*Sinh[v_]^2)/(c_.+d_.*Cosh[v_]^2),x_Symbol] :=
  b/d - (b*c-(a-b)*d)/(d*(c+d*Cosh[v]^2)) /;
FreeQ[{a,b,c,d},x]

(* Basis: (a+b*Cosh[z]^2)/(c+d*Sinh[z]^2) == b/d - (b*c-(a+b)*d)/(d*(c+d*Sinh[z]^2)) *)
ExpandExpression[(a_+b_.*Cosh[v_]^2)/(c_.+d_.*Sinh[v_]^2),x_Symbol] :=
  b/d - (b*c-(a+b)*d)/(d*(c+d*Sinh[v]^2)) /;
FreeQ[{a,b,c,d},x]


(* Basis: If n is an integer, 
	Tanh[z]^n/(a*Sinh[z]^n+b*Cosh[z]^n) == Sech[z]^n/a - b/(a*(a*Sinh[z]^n+b*Cosh[z]^n)) *)
ExpandExpression[Tanh[v_]^n_./(a_.*Sinh[v_]^n_.+b_.*Cosh[v_]^n_.),x_Symbol] :=
  Sech[v]^n/a - b/(a*(a*Sinh[v]^n+b*Cosh[v]^n)) /;
FreeQ[{a,b},x] && IntegerQ[n]

(* Basis: If n is an integer, 
	Coth[z]^n/(a*Sinh[z]^n+b*Cosh[z]^n) == Csch[z]^n/b - a/(b*(a*Sinh[z]^n+b*Cosh[z]^n)) *)
ExpandExpression[Coth[v_]^n_./(a_.*Sinh[v_]^n_.+b_.*Cosh[v_]^n_.),x_Symbol] :=
  Csch[v]^n/b - a/(b*(a*Sinh[v]^n+b*Cosh[v]^n)) /;
FreeQ[{a,b},x] && IntegerQ[n]


(* Basis: If n is an integer, 
	Sech[z]^n/(a+b*Coth[z]^n) == Sech[z]^n/a - b/(a*(a*Sinh[z]^n+b*Cosh[z]^n)) *)
ExpandExpression[Sech[v_]^n_./(a_+b_.*Coth[v_]^n_.),x_Symbol] :=
  Sech[v]^n/a - b/(a*(a*Sinh[v]^n+b*Cosh[v]^n)) /;
FreeQ[{a,b},x] && IntegerQ[n]

(* Basis: If n is an integer, 
	Csch[z]^n/(a+b*Tanh[z]^n) == Csch[z]^n/a - b/(a*(b*Sinh[z]^n+a*Cosh[z]^n)) *)
ExpandExpression[Csch[v_]^n_./(a_+b_.*Tanh[v_]^n_.),x_Symbol] :=
  Csch[v]^n/a - b/(a*(b*Sinh[v]^n+a*Cosh[v]^n)) /;
FreeQ[{a,b},x] && IntegerQ[n]


(* Basis: If a^2-b^2=0, 1/(a+b*Tanh[z]) == Cosh[z]^2/a - Cosh[z]*Sinh[z]/b *)
ExpandExpression[u_*(a_+b_.*Tanh[v_])^n_,x_Symbol] :=
  ExpandExpression[u*(Cosh[v]^2/a - Cosh[v]*Sinh[v]/b)^(-n),x] /;
FreeQ[{a,b},x] && IntegerQ[n] && n<0 && ZeroQ[a^2-b^2]

(* Basis: If a^2-b^2=0, 1/(a+b*Coth[z]) == -Sinh[z]^2/a + Cosh[z]*Sinh[z]/b *)
ExpandExpression[u_*(a_+b_.*Coth[v_])^n_,x_Symbol] :=
  ExpandExpression[u*(-Sinh[v]^2/a + Cosh[v]*Sinh[v]/b)^(-n),x] /;
FreeQ[{a,b},x] && IntegerQ[n] && n<0 && ZeroQ[a^2-b^2]


ExpandExpression[u_*Sinh[a_.+b_.*x_]*Cosh[a_.+b_.*x_],x_Symbol] :=
  ExpandExpression[u*Sinh[2*a+2*b*x]/2,x] /;
FreeQ[{a,b},x]


ExpandExpression[u_*Sech[a_.+b_.*x_]*Csch[a_.+b_.*x_],x_Symbol] :=
  ExpandExpression[2*u*Csch[2*a+2*b*x],x] /;
FreeQ[{a,b},x]


(* Basis: Cosh[z]*Sinh[z] ==  Sinh[2*z]/2 *)
(* ExpandExpression[x_^m_.*Cosh[u_]*Sinh[u_],x_Symbol] :=
  ExpandExpression[x^m*Sinh[Dist[2,u]]/2,x] /;
RationalQ[m] *)

ExpandExpression[v_*Cosh[u_]*Sinh[u_],x_Symbol] :=
  ExpandExpression[v*Sinh[Dist[2,u]]/2,x] /;
MatchQ[v,x^m_ /; RationalQ[m]] || MatchQ[v,f_^w_ /; FreeQ[f,x] && LinearQ[w,x]]


(* ::Subsubsection:: *)
(*Partial fraction expansion rules*)


(* Fractional powers of sums are removed because Apart does wierd things with them!!! *)
ExpandExpression[u_.*v_^n_,x_Symbol] :=
  Module[{w=ExpandExpression[u,x]},
  If[SumQ[w],
    Map[Function[RegularizeTerm[#*v^n,x]],w],
  w*v^n]] /;
(* Not[SumFreeQ[v]] && *) FractionQ[n] && (n<0 || n>1)


SumFreeQ[u_] :=
  If[AtomQ[u],
    True,
  If[SumQ[u],
    False,
  Catch[
    Scan[Function[If[SumFreeQ[#],Null,Throw[False]]],u];
    True]]]


(* Derivation: Algebraic expansion *)
ExpandExpression[u_,x_Symbol] :=
  If[SumQ[u] ||
  	MatchQ[u, c_./(a_+b_.*x^n_.) /; 
		FreeQ[{a,b,c},x] && IntegerQ[n]] ||
	  MatchQ[u, c_.*x^m_.*(a_+b_.*x^n_.)^p_. /; 
		FreeQ[{a,b,c},x] && IntegersQ[m,n,p] && ( (* p<0 || *) ZeroQ[m-n+1])] ||
	  MatchQ[u, c_.*x^m_.*(v_+b_.*x^n_.)^p_. /; 
		FreeQ[{b,c},x] && Not[AlgebraicFunctionQ[v,x]] && IntegersQ[m,n,p] && p<0 && 0<n<=m] ||
      MatchQ[u, x^m_*f_[a_.+b_.*x]^n_. /; 
		FreeQ[{a,b,m,n},x] && MemberQ[{Tan,Cot,Tanh,Coth},f] && Not[IntegerQ[m] && m>0]] ||
      MatchQ[u, f_[a_.+b_.*x+c_.*x^2]/x /; 
		FreeQ[{a,b,c},x] && MemberQ[{Sin,Cos,Sinh,Cosh},f]] ||
      MatchQ[u, (a_+b_.*f_[c_.+d_.*x]^2)^n_ /; 
		FreeQ[{a,b},x] && MemberQ[{Sin,Cos,Sinh,Cosh},f] && IntegerQ[n]],
    u,
  Module[{tmp,lst},
  tmp = FindKernel[u,x];
  If[NotFalseQ[tmp] && FunctionOfKernelQ[u,tmp,x],
    Subst[ExpandExpression[Subst[u,tmp,x],x],x,tmp],

  tmp = FunctionOfTrigQ[u,x,x];
  If[tmp && (ContainsQ[u,Sin,x] || ContainsQ[u,Csc,x]) && FunctionOfQ[Sin[x],u,x],
    ExpandTrigExpression[u,Sin[x],x],
  If[tmp && (ContainsQ[u,Cos,x] || ContainsQ[u,Sec,x]) && FunctionOfQ[Cos[x],u,x],
    ExpandTrigExpression[u,Cos[x],x],
  If[tmp && (ContainsQ[u,Tan,x] || ContainsQ[u,Cot,x]) && FunctionOfQ[Tan[x],u,x],
    ExpandTrigExpression[u,Tan[x],x],

  tmp = FunctionOfHyperbolicQ[u,x,x];
  If[tmp && (ContainsQ[u,Sinh,x] || ContainsQ[u,Csch,x]) && FunctionOfQ[Sinh[x],u,x],
    ExpandTrigExpression[u,Sinh[x],x],
  If[tmp && (ContainsQ[u,Cosh,x] || ContainsQ[u,Sech,x]) && FunctionOfQ[Cosh[x],u,x],
    ExpandTrigExpression[u,Cosh[x],x],
  If[tmp && (ContainsQ[u,Tanh,x] || ContainsQ[u,Coth,x]) && FunctionOfQ[Tanh[x],u,x],
    ExpandTrigExpression[u,Tanh[x],x],

  Module[{v},
  If[AlgebraicFunctionQ[u,x] && Not[RationalFunctionQ[u,x]],
    If[GoodExpansionQ[u,v=ExpandExpressionAux[u,x],x],
      RegularizeTerm[v,x],
    u],
  If[GoodExpansionQ[u,v=Apart[u,x],x],
    RegularizeTerm[v,x],
  If[GoodExpansionQ[u,v=Apart[u],x],
    RegularizeTerm[v,x],
  If[GoodExpansionQ[u,v=ExpandExpressionAux[u,x],x],
    RegularizeTerm[v,x],
  If[TrigHyperbolicFreeQ[u,x],
    v=Apart[u,x];
    RegularizeTerm[If[GoodExpansionQ[u,v,x],v,u],x],

  tmp=TryTrigReduceQ[u];
  If[tmp, lst=SplitFreeFactors[TrigReduce[u],x]];
  If[tmp && GoodExpansionQ[u,v=Apart[lst[[2]],x],x],
    Map[Function[RegularizeTerm[lst[[1]]*#,x]],v],
  If[tmp && GoodExpansionQ[u,v=Apart[lst[[2]]],x],
    Map[Function[RegularizeTerm[lst[[1]]*#,x]],v],
  If[tmp && GoodExpansionQ[u,v=ExpandExpressionAux[lst[[2]],x],x],
    Map[Function[RegularizeTerm[lst[[1]]*#,x]],v],

  v=SmartTrigExpand[u,x];
  If[SumQ[v],
    RegularizeTerm[v,x],
  v=TrigExpand[u]; 
(* Replace Simplify with TrigSimplify on the next line when TrigSimplify simplifies 
	a*Sin[x]^2+b*Cos[x]^2 to b+(a-b)*Sin[x]^2! *)
  If[SumQ[v] && SumQ[v=Simplify[v]],
    RegularizeTerm[v,x],
  v=Apart[u,x];
  RegularizeTerm[If[GoodExpansionQ[u,v,x],v,u],x]]]]]]]]]]]]]]]]]]]]]


(* If TrigReduce[u] will result in a linear expansion of trig functions, TryTrigReduceQ[u] returns True;
	else it returns False. *)
TryTrigReduceQ[u_] :=
  MatchQ[u,v_*f_[w_]^n_ /; IntegerQ[n] && MemberQ[{Sin,Cos,Sinh,Cosh},f]] (* &&
  Not[MatchQ[u,v_*f_[w_]^n_ /; IntegerQ[n] && MemberQ[{Tan,Cot,Sec,Csc,Tanh,Coth,Sech,Csch},f]]] *)


ExpandTrigExpression[u_,v_,x_Symbol] :=
  Module[{w=TrigSimplify[Subst[ExpandExpression[SubstFor[v,u,x],x],x,v]]},
  If[SumQ[w],
    Map[Function[ExpandTrigExpressionAux[#,x]],w],
  w]]

(* Note: Since TrigSimplify can result in a product (e.g. when integrating Tanh[x]^2/(a+b*Sinh[x])), 
	this function is called to distribute factors over a sum. *)
ExpandTrigExpressionAux[u_*v_,x_Symbol] :=
  Map[Function[u*#],v] /;
SumQ[v] && Not[FreeQ[u,x]] && Not[FreeQ[v,x]]

ExpandTrigExpressionAux[u_,x_Symbol] :=
  u


(* If v is a sum and the terms of v are all simpler than u, returns True; else it returns False. *) 
GoodExpansionQ[u_,v_,x_Symbol] :=
  If[SumQ[v],
    If[PolynomialQ[u,x],
      True,
    If[RationalFunctionQ[u,x],
      If[RationalFunctionQ[v,x],
        Module[{lst=RationalFunctionExponents[u,x]},
        Catch[
          Scan[Function[If[SimplerRationalFunctionQ[lst,RationalFunctionExponents[#,x]],Throw[False]]],v];
          True]],
      False],
    True]],
  False]

SimplerRationalFunctionQ[lst1_,lst2_] :=
  lst1[[1]]<=lst2[[1]] && lst1[[2]]<=lst2[[2]] && (lst1[[1]]<lst2[[1]] || lst1[[2]]<lst2[[2]])


(* Kludge until there is a rule for this type of integrand! *)
ExpandExpressionAux[(d_+e_.*x_^k_.)*(a_+b_.*x_^k_+c_.*x_^j_)^n_,x_Symbol] :=
  d*(a+b*x^k+c*x^j)^n + e*x^k*(a+b*x^k+c*x^j)^n /;
FreeQ[{a,b,c,d,e},x] && IntegersQ[n,k,j] && EvenQ[k] && j==2*k && n<-1

ExpandExpressionAux[u_*v_,x_Symbol] :=
  Map[Function[#*v],u] /;
SumQ[u] && Not[FreeQ[u,x]]

ExpandExpressionAux[u_^n_*v_.,x_Symbol] :=
  Module[{w=Expand[u^n,x]},
  Map[Function[#*v],w] /;
 SumQ[w]] /;
SumQ[u] && IntegerQ[n] && n>0 && Not[FreeQ[u,x]]


DownValues[RegularizeTerm]={};

(* Basis: a*u+b*u == (a+b)*u *)
RegularizeTerm[a_.*u_+b_.*u_+v_.,x_Symbol] :=
  RegularizeTerm[(a+b)*u+v,x] /;
FreeQ[{a,b},x]

(* Basis: e/(a+b*x) + f/(c+d*x) == (c*e+a*f + (d*e+b*f)*x)/(a*c + (b*c+a*d)*x + b*d*x^2) *)
(* RegularizeTerm[u_.+e_./(a_.+b_.*x_)+f_./(c_+d_.*x_),x_Symbol] :=
  RegularizeTerm[u+(c*e+a*f)/(a*c+(b*c+a*d)*x+b*d*x^2),x] /;
FreeQ[{a,b,c,d,e,f},x] && ZeroQ[d*e+b*f] && (ZeroQ[a] || ZeroQ[b*c+a*d]) *)

RegularizeTerm[c_.*u_/(a_+b_.*x_^n_),x_Symbol] :=
  If[PolynomialQ[u,x],
    Module[{k},
    Sum[RegularizeTerm[c*Coefficient[u,x,k]*x^k/(a+b*x^n),x],{k,0,Exponent[u,x]}]],    
  Map[Function[RegularizeTerm[c*#/(a+b*x^n),x]],u]] /;
FreeQ[{a,b,c},x] && IntegerQ[n] && n>0 && SumQ[u] && Not[FreeQ[u,x]]

RegularizeTerm[u_,x_Symbol] :=
  If[SumQ[u],
    Map[Function[RegularizeTerm[#,x]],u],
  Module[{lst1,lst2},
  lst1=SplitFreeFactors[NormalForm[u,x],x];
  lst2=SplitFreeFactors[Regularize[lst1[[2]],x],x];
  lst2={lst1[[1]]*lst2[[1]], lst2[[2]]};
  lst2={Simplify[Numerator[lst2[[1]]]] / Simplify[Denominator[lst2[[1]]]], lst2[[2]]};
  If[SumQ[lst2[[2]]],
    Map[Function[lst2[[1]]*#],lst2[[2]]],
  lst2[[1]] * lst2[[2]]]]]


ContainsQ[u_,f_,x_] :=
  Count[u, f[n_.*x] /; IntegerQ[n], Infinity] > 0


FunctionOfKernelQ[u_,v_,x_] :=
  If[u===v,
    True,
  If[AtomQ[u],
    u=!=x,
  Catch[
    Scan[Function[If[Not[FunctionOfKernelQ[#,v,x]],Throw[False]]],u];
    True]]]

(* If u contains a subexpression of the form f[v] where f is a function and v is an algebraic
	function of and dependent on x, FindKernel[u,x] returns f[v]; else it returns False. *)
FindKernel[u_,x_] :=
  If[AlgebraicFunctionQ[u,x],
    False,
  If[Length[u]===1 && AlgebraicFunctionQ[u[[1]],x],
    u,
  If[Length[u]===2 && AlgebraicFunctionQ[u[[1]],x] && FreeQ[u[[2]],x],
    u,
  If[Length[u]===2 && AlgebraicFunctionQ[u[[2]],x] && FreeQ[u[[1]],x],
    u,
  Module[{tmp},
  Catch[
    Scan[Function[If[NotFalseQ[tmp=FindKernel[#,x]], Throw[tmp]]],u];
    False]]]]]]


(* ::Section::Closed:: *)
(*Algebraic Simplification Rules*)


(* ::Subsection::Closed:: *)
(*Inverse Trig Function Simplification Rules*)


Unprotect[ArcSin,ArcCos,ArcTan,ArcCot,ArcSec,ArcCsc];

ArcSin[1/u_] := ArcCsc[u];
ArcCos[1/u_] := ArcSec[u];
ArcTan[1/u_] := ArcCot[u];
ArcCot[1/u_] := ArcTan[u];
ArcSec[1/u_] := ArcCos[u];
ArcCsc[1/u_] := ArcSin[u];

ArcSin[1/(u_*v_)] := ArcCsc[u*v];
ArcCos[1/(u_*v_)] := ArcSec[u*v];
ArcTan[1/(u_*v_)] := ArcCot[u*v];
ArcCot[1/(u_*v_)] := ArcTan[u*v];
ArcSec[1/(u_*v_)] := ArcCos[u*v];
ArcCsc[1/(u_*v_)] := ArcSin[u*v];

ArcSin[Cot[u_]] := ArcCsc[Tan[u]];
ArcCos[Cot[u_]] := ArcSec[Tan[u]];
ArcTan[Cot[u_]] := ArcCot[Tan[u]];
ArcCot[Cot[u_]] := ArcTan[Tan[u]];
ArcSec[Cot[u_]] := ArcCos[Tan[u]];
ArcCsc[Cot[u_]] := ArcSin[Tan[u]];

ArcSin[Sec[u_]] := ArcCsc[Cos[u]];
ArcCos[Sec[u_]] := ArcSec[Cos[u]];
ArcTan[Sec[u_]] := ArcCot[Cos[u]];
ArcCot[Sec[u_]] := ArcTan[Cos[u]];
ArcSec[Sec[u_]] := ArcCos[Cos[u]];
ArcCsc[Sec[u_]] := ArcSin[Cos[u]];

ArcSin[Csc[u_]] := ArcCsc[Sin[u]];
ArcCos[Csc[u_]] := ArcSec[Sin[u]];
ArcTan[Csc[u_]] := ArcCot[Sin[u]];
ArcCot[Csc[u_]] := ArcTan[Sin[u]];
ArcSec[Csc[u_]] := ArcCos[Sin[u]];
ArcCsc[Csc[u_]] := ArcSin[Sin[u]];

ArcSin[Coth[u_]] := ArcCsc[Tanh[u]];
ArcCos[Coth[u_]] := ArcSec[Tanh[u]];
ArcTan[Coth[u_]] := ArcCot[Tanh[u]];
ArcCot[Coth[u_]] := ArcTan[Tanh[u]];
ArcSec[Coth[u_]] := ArcCos[Tanh[u]];
ArcCsc[Coth[u_]] := ArcSin[Tanh[u]];

ArcSin[Sech[u_]] := ArcCsc[Cosh[u]];
ArcCos[Sech[u_]] := ArcSec[Cosh[u]];
ArcTan[Sech[u_]] := ArcCot[Cosh[u]];
ArcCot[Sech[u_]] := ArcTan[Cosh[u]];
ArcSec[Sech[u_]] := ArcCos[Cosh[u]];
ArcCsc[Sech[u_]] := ArcSin[Cosh[u]];

ArcSin[Csch[u_]] := ArcCsc[Sinh[u]];
ArcCos[Csch[u_]] := ArcSec[Sinh[u]];
ArcTan[Csch[u_]] := ArcCot[Sinh[u]];
ArcCot[Csch[u_]] := ArcTan[Sinh[u]];
ArcSec[Csch[u_]] := ArcCos[Sinh[u]];
ArcCsc[Csch[u_]] := ArcSin[Sinh[u]];

Protect[ArcSin,ArcCos,ArcTan,ArcCot,ArcSec,ArcCsc];


(* Unprotect[ArcTan,ArcCot];
ArcTan[Tan[u_]] := Pi*Mods[Distribute[u/Pi,Plus,Times]];
ArcCot[Tan[u_]] := Pi*Mods[1/2-Distribute[u/Pi,Plus,Times]];
Protect[ArcTan,ArcCot]; *)


(* ::Subsection::Closed:: *)
(*Inverse Hyperbolic Function Simplification Rules*)


Unprotect[ArcSinh,ArcCosh,ArcTanh,ArcCoth,ArcSech,ArcCsch];

ArcSinh[1/u_] := ArcCsch[u];
ArcCosh[1/u_] := ArcSech[u];
ArcTanh[1/u_] := ArcCoth[u];
ArcCoth[1/u_] := ArcTanh[u];
ArcSech[1/u_] := ArcCosh[u];
ArcCsch[1/u_] := ArcSinh[u];

ArcSinh[1/(u_*v_)] := ArcCsch[u*v];
ArcCosh[1/(u_*v_)] := ArcSech[u*v];
ArcTanh[1/(u_*v_)] := ArcCoth[u*v];
ArcCoth[1/(u_*v_)] := ArcTanh[u*v];
ArcSech[1/(u_*v_)] := ArcCosh[u*v];
ArcCsch[1/(u_*v_)] := ArcSinh[u*v];

ArcSinh[Coth[u_]] := ArcCsch[Tanh[u]];
ArcCosh[Coth[u_]] := ArcSech[Tanh[u]];
ArcTanh[Coth[u_]] := ArcCoth[Tanh[u]];
ArcCoth[Coth[u_]] := ArcTanh[Tanh[u]];
ArcSech[Coth[u_]] := ArcCosh[Tanh[u]];
ArcCsch[Coth[u_]] := ArcSinh[Tanh[u]];

ArcSinh[Sech[u_]] := ArcCsch[Cosh[u]];
ArcCosh[Sech[u_]] := ArcSech[Cosh[u]];
ArcTanh[Sech[u_]] := ArcCoth[Cosh[u]];
ArcCoth[Sech[u_]] := ArcTanh[Cosh[u]];
ArcSech[Sech[u_]] := ArcCosh[Cosh[u]];
ArcCsch[Sech[u_]] := ArcSinh[Cosh[u]];

ArcSinh[Csch[u_]] := ArcCsch[Sinh[u]];
ArcCosh[Csch[u_]] := ArcSech[Sinh[u]];
ArcTanh[Csch[u_]] := ArcCoth[Sinh[u]];
ArcCoth[Csch[u_]] := ArcTanh[Sinh[u]];
ArcSech[Csch[u_]] := ArcCosh[Sinh[u]];
ArcCsch[Csch[u_]] := ArcSinh[Sinh[u]];

ArcSinh[Cot[u_]] := ArcCsch[Tan[u]];
ArcCosh[Cot[u_]] := ArcSech[Tan[u]];
ArcTanh[Cot[u_]] := ArcCoth[Tan[u]];
ArcCoth[Cot[u_]] := ArcTanh[Tan[u]];
ArcSech[Cot[u_]] := ArcCosh[Tan[u]];
ArcCsch[Cot[u_]] := ArcSinh[Tan[u]];

ArcSinh[Sec[u_]] := ArcCsch[Cos[u]];
ArcCosh[Sec[u_]] := ArcSech[Cos[u]];
ArcTanh[Sec[u_]] := ArcCoth[Cos[u]];
ArcCoth[Sec[u_]] := ArcTanh[Cos[u]];
ArcSech[Sec[u_]] := ArcCosh[Cos[u]];
ArcCsch[Sec[u_]] := ArcSinh[Cos[u]];

ArcSinh[Csc[u_]] := ArcCsch[Sin[u]];
ArcCosh[Csc[u_]] := ArcSech[Sin[u]];
ArcTanh[Csc[u_]] := ArcCoth[Sin[u]];
ArcCoth[Csc[u_]] := ArcTanh[Sin[u]];
ArcSech[Csc[u_]] := ArcCosh[Sin[u]];
ArcCsch[Csc[u_]] := ArcSinh[Sin[u]];

Protect[ArcSinh,ArcCosh,ArcTanh,ArcCoth,ArcSech,ArcCsch];


(* Unprotect[ArcTanh,ArcCoth];
ArcTanh[Tanh[u_]] := I*Pi*Mods[Distribute[-I*u/Pi,Plus,Times]];
ArcCoth[Tanh[u_]] := -I*Pi*Mods[1/2-Distribute[-I*u/Pi,Plus,Times]];
Protect[ArcTanh,ArcCoth]; *)
