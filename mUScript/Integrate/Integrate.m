(* ::Package:: *)

RubiNotLoaded = True

(* Mathematica Raw Program *)
IntegrateU[f_,x_]:=Module[
	{e=f,ans},
	
	(*Get[CWD<>"/IntegralFunctions.m"];
	Get[CWD<>"/Rubi/init.m"];
	Get[CWD<>"/Albi/init.m"];*)
	
	If[RubiNotLoaded, Get[Calculus`CWD <> "Rubi/init.m"]; RubiNotLoaded = False];
	ans=Calculus`Rubi`Int[e,x];

	If[FreeQ[ans,Calculus`Rubi`Int], Return[ans],
		(*Print["Rubi Failed!"];*)
		ans=IintTable[e,x]
		(*ans=Calculus`Albi`Risch`pmint[e,x]*)
	];

	(*If[StringMatchQ[ToString[ans],"*Calculus`Albi`Risch*"],
		Print["Albi Failed!"];
		ans=IintTable[e,x],
		Return[ans]
	];*)
	If[ans=!="NotMatch",Return[ans],Return[$Failed]];
]


IntegrateList=
{
A[Sqrt[1+(2 x_)/(1+x_^2)]/(1+x_^2),x_] :> ((-1+x) Sqrt[1+(2 x)/(1+x^2)])/(1+x),
A[1/((1+x_^4) Sqrt[-x_^2+Sqrt[1+x_^4]]),x_] :> ArcCot[Sqrt[-x^2+Sqrt[1+x^4]]/x],
A[Sqrt[1+(2 x_)/(1+x_^2)],x_] :> (Sqrt[(1+x)^2/(1+x^2)] (1+x^2+Sqrt[1+x^2] ArcSinh[x]))/(1+x),
A[1/Sqrt[1+(2 x_)/(1+x_^2)],x_] :> ((1+x) (Sqrt[1+x^2]-ArcSinh[x]+Sqrt[2] Log[1+x]-Sqrt[2] Log[1-x+Sqrt[2] Sqrt[1+x^2]]))/(Sqrt[(1+x)^2/(1+x^2)] Sqrt[1+x^2]),
A[(x_^2+2 x_ Log[x_]+Log[x_]^2+(1+x_) Sqrt[x_+Log[x_]])/(x_^3+2 x_^2 Log[x_]+x_ Log[x_]^2),x_] :> Log[x]-2/Sqrt[x+Log[x]],
A[(Cos[x_]+Sin[x_])/(E^-x_+Sin[x_]),x_] :> Log[1+E^x Sin[x]],
A[E^Sin[x_] Sec[x_]^2 (x_ Cos[x_]^3-Sin[x_]),x_] :> E^Sin[x] (-1+x Cos[x]) Sec[x],
A[E^x_^x_ x_^(2 x_) (1+Log[x_]),x_] :> E^x^x (-1+x^x),
A[x_^(-2-1/x_) (1-Log[x_]),x_] :> -x^(-1/x),
A[(x_ Cos[x_]-Sin[x_])/(x_-Sin[x_])^2,x_] :> -(x/(-x+Sin[x])),
A[1/(Cos[x_]^(3/2) Sqrt[3 Cos[x_]+Sin[x_]]),x_] :> (2 Sqrt[3 Cos[x]+Sin[x]])/Sqrt[Cos[x]],
A[(Csc[x_] Sqrt[Cos[x_]+Sin[x_]])/Cos[x_]^(3/2),x_] :> (2 (Cos[x]+Sin[x]-ArcCoth[Sqrt[Cos[x]+Sqrt[Sin[x]^2]]/Sqrt[Cos[x]]] Sqrt[Cos[x]] Sqrt[Cos[x]+Sqrt[Sin[x]^2]]))/(Sqrt[Cos[x]] Sqrt[Cos[x]+Sin[x]]),
A[(-3+10 x_+4 x_^3-30 x_^5)/(3+x_+x_^4)^3-(3 (1+4 x_^3) (2-3 x_+5 x_^2+x_^4-5 x_^6))/(3+x_+x_^4)^4,x_] :> (2-3 x+5 x^2+x^4-5 x^6)/(3+x+x^4)^3,
A[1/(x_ Log[E^x_]),x_] :> (-Log[x]+Log[Log[E^x]])/(x-Log[E^x]),
A[Cot[x_]/Log[E^Sin[x_]],x_] :> (Log[Log[E^Sin[x]]]-Log[Sin[x]])/(-Log[E^Sin[x]]+Sin[x]),
A[x_/(Sqrt[-1+x_] Sqrt[1+x_] ArcCosh[x_]),x_] :> CoshIntegral[ArcCosh[x]],
A[(x_ Cosh[x_]-Sinh[x_])/(x_-Sinh[x_])^2,x_] :> -(x/(-x+Sinh[x]))
};

IintTable[f_,x_]:=Module[
    {},
    ret=A[f,x]//.IntegrateList;
    If[Head[ret]===A,Return["NotMatch"],Return[ret]];
	Return["NotMatch"]
];
