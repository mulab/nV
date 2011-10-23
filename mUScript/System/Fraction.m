(* ::Package:: *)

Begin["System`"]

Numerator::usage = "Numerator[expr] gives the numerator of expr.";
Denominator::usage = "Denominator[expr] gives the denominator of expr.";
Cancel::usage = "Cancel[expr] cancels out common factors in the numerator and denominator of expr.";
Together::usage = "Together[expr] puts terms in a sum over a common denominator, and cancels factors in the result.";
Apart::usage = "Apart[expr] rewrites a rational expression as a sum of terms with minimal denominators. 
Apart[expr,var] treats all variables other than var as constants.";

SetAttributes[Numerator,Listable];
SetAttributes[Denominator,Listable];
SetAttributes[Cancel,Listable];
SetAttributes[Together,Listable];
SetAttributes[Apart,Listable];

CProc["musystem",(*RationalQ,*)RationalNumerator,RationalDenominator]

Begin["`Private`"]


Numerator[x_Rational] := RationalNumerator[x]
Numerator[f_]:=Module[
	{e=f},
	If[Head[f]=!=Power && Head[f]=!=Times && Head[f]=!=Exp,
		If[Head[f]===List,
			Return[Numerator/@f],
			Return[f]
		]
	];
	e = e//. Power[b_,n_]/;n<0 -> 1;
	e = e//. Power[b_,n_*c_]/;n<0 -> 1;
	e = e//. Power[b_,n_*c_+d_]/;n<0 -> Power[b,d];
	(* If Exp[x]=!=E^x in mU kernel:*)
	e = e//. Exp[n_]/;n<0 -> 1;
	e = e//. Exp[n_*c_]/;n<0 -> 1;
	e = e//. Exp[n_*c_+d_]/;n<0 -> Exp[d];
	Return[e];
]

Denominator[x_Rational] := RationalDenominator[x]
Denominator[f_]:=Module[
	{},
	If[f==0, 1, Numerator[f]/f]
]

(* Useful when single variable*)
Cancel[f_] := Module[
	{e=f, num, den, gcd, v},
	(*Print["Cancel: ",f];*)
	If[AtomQ[e],Return[e]];
	If[Head[e]===List || Head[e]===Plus,
		(* Head[e]=!=Times && Head[e]=!=Power leads to unknown error *)
		(* ? TODO: /@ only when meets List, Equation, Inequality, Logic Function *)
		Return[Cancel/@e]
	];
	num = Numerator[e];
	den = Denominator[e];
	v = First[Variables[e]];
	If[!PolynomialQ[num, v], Return[e]];
	If[!PolynomialQ[den, v], Return[e]];
	gcd = PolynomialGCD[num, den];
	(* Is this ok to choose an arbitrary variable to do poly divide? *)
	num = PolynomialQuotient[num, gcd, v];
	den = PolynomialQuotient[den, gcd, v];
	Return[num/den];
]

Together[f_] := Module[
	{denom, lcm, num, v, i},
	If[Head[f]===List || Head[f]===Times,
		(* Head[f]=!=Plus leads to unknown error *)
		(* ? TODO: /@ only when meets List, Equation, Inequality, Logic Function *)
		Return[Together/@f]
	];
	(*Print[f];*)
	If[Head[f]===Plus,
		(*Print[6];*)
		denom = Denominator[List@@f];
		(*Print["denom=",denom];*)
		lcm = PolynomialLCM@@denom;
		v=Variables[lcm];
		(*Print[7];*)
		If[Length[v]==0, Return[f]];
		v = First[v];
		Do[
			denom[[i]] = PolynomialQuotient[lcm, denom[[i]], v], (* it's better to use multi-var exact div *)
			{i,Length[denom]}
		];
		(*Print[8];*)
		num = Expand[Plus@@(Numerator[List@@f]*denom)];
		Cancel[num/lcm],
		(*Print[11];*)
		f
	]
]

(* Given a Euclidean domain D and a,b,c\[Element]D with c\[Element](a,b), return s,t,g\[Element]D such that g=gcd(a,b) and s*a+t*b=g *)
ExtendedEuclidean[A_, B_, x_Symbol] := Module[
	{a=Expand[A],b=Expand[B],s,t,g,q,r,a1,b1,r1},
	a1=1;b1=0;
	While[b=!=0,
		{q,r}=PolynomialQuotientRemainder[a,b,x];
		a=b;b=Expand[r];
		r1=a1-q*b1;a1=b1;b1=r1;
	];
	{s,g}={a1,a};
	{t,r}=PolynomialQuotientRemainder[g-s*A,B,x];
	{s,t,g}
]

(* return {s,t} so that s*a+t*b=c *)
ExtendedEuclidean[a_, b_, c_, x_Symbol] := Module[
	{g,s,t,q,r},
	(*Print["{a,b,c}=",{a,b,c}];*)
	{s,t,g}=ExtendedEuclidean[a,b,x];
	(*Print[4];*)
	q=PolynomialQuotient[c,g,x];
	(* deal with exception that r!=0 *)
	{s,t}=q*{s,t};
	{q,r}=PolynomialQuotientRemainder[s,b,x];
	(*Print[5];*)
	{r,Together[t+q*a]}
]

PartialFraction[A_, d_List, x_Symbol] := Module[
	{a0, a1, ai, b0, r, t},
	(*Print["{A,d}=",{A,d}];*)
	{a0,r} = PolynomialQuotientRemainder[A, Times@@d, x];
	If[Length[d]==1, Return[{a0,{r}}]];
	(*Print[1];*)
	{a1,t} = ExtendedEuclidean[Times@@Rest[d], First[d], r, x];
	(*Print[2];*)
	{b0,ai} = PartialFraction[t, Rest[d], x];
	(*Print[3];*)
	{a0+b0,Flatten[{a1,ai}]}
]

(* reference: Bronstein P17 *)
PartialFraction[A_, d_List, e_List, x_Symbol] := Module[
	{a0, a, n=Length[d], c, i, j},
	{a0, a} = PartialFraction[A, Power[d,e], x];
	(*Print["{a0,a}=",{a0,a}];*)
	c = Table[0,{n}];
	Do[
		c[[i]] = Table[0,{e[[i]]}];
		Do[
			{a[[i]], c[[i,j]]} = PolynomialQuotientRemainder[a[[i]], d[[i]], x]
			,{j,e[[i]],1,-1}
		];
		a[[0]] = a[[0]]+a[[i]];
		,{i,n}
	];
	{a0, c}
] /; Length[d]==Length[e]

Apart[f_, x_Symbol] := Module[
	{F=Cancel[f], lst, d, e, ans, c, i, j},
	If[Head[F]===Plus,
		Return[Apart/@F]
	];
	lst = FactorList[Denominator[F]];
	d = (#[[1]]&)/@lst(*lst[[All,1]]*);
	e = (#[[2]]&)/@lst(*lst[[All,2]]*);
	{ans, c} = PartialFraction[Numerator[F], d, e, x];
	Do[
		Do[
			ans = ans + c[[i,j]]*Power[d[[i]],-j]
			,{j,e[[i]]}
		]
		,{i,Length[d]}
	];
	ans
]

Apart[f_] := Module[
	{F=Cancel[f]},
	If[Head[f]===List,
		Apart/@F,
		If[Length[Variables[F]] > 0,
			Apart[F, Variables[F][[1]]],
			F
		]
	]
]


End[]

Protect[Numerator,Denominator,Cancel,Together,Apart];

End[]
