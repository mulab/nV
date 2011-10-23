(* ::Package:: *)

Unprotect[SinIntegral, CosIntegral, SinhIntegral, CoshIntegral, FresnelS, FresnelC];

(* evaluated for inexact z. *)
SinIntegral[z_] := Module[
	{x, y},
	x = Re[z];
	y = Im[z];
	Which[
		z == 0,	z,
		x + y < 0,	-SinIntegral[-z],
		y == 0,	Im[ExpIntegralE[1,I x]] + N[Pi/2,Precision[z]] + I y/x,
		x >= y,	-I/2 (ExpIntegralE[1, I z] - ExpIntegralE[1, -I z]) + N[Pi/2, Precision[z]],
		True,	I/2 (ExpIntegralEi[-I z] + ExpIntegralE[1,-I z])
	]
] /; (NumberQ[z] && Precision[z] < Infinity)

CosIntegral[z_] := Module[
	{x, y},
	x = Re[z];
	y = Im[z];
	Which[
		z == 0,	-Infinity,
		x + y < 0,	CosIntegral[-z] + If[y < 0, -I, I] N[Pi, Precision[z]],
		y == 0,	-Re[ExpIntegralE[1, I x]] + I y/x,
		x >= y,	-(ExpIntegralE[1, I z] + ExpIntegralE[1, -I z])/2,
		True,	(ExpIntegralEi[-I z] - ExpIntegralE[1, -I z])/2 + I N[Pi/2, Precision[z]]
	]
] /; (NumberQ[z] && Precision[z] < Infinity)

SinhIntegral[z_] := Module[
	{shi = -I SinIntegral[I z]},
	If[Head[z] === Real, Re[shi], shi]
] /; (NumberQ[z] && Precision[z] < Infinity)

CoshIntegral[z_] := Module[
	{chi},
	If[z == 0, Return[-Infinity]];
	chi = CosIntegral[I z] + If[Re[z] < 0 && Im[z] >= 0, 3I, -I] * N[Pi/2, Precision[z]];
	If[Head[z] === Real && z > 0, Re[chi], chi]
] /; (NumberQ[z] && Precision[z] < Infinity)

FresnelS[z_] := Module[
	{ipiz2 = N[Pi, Precision[z]] I z^2/2},
	If[Head[z] === Real,
	    z Im[Hypergeometric1F1[1/2, 3/2, ipiz2]],
	    -I z (Hypergeometric1F1[1/2, 3/2, ipiz2] - Hypergeometric1F1[1/2, 3/2, -ipiz2])/2
	]
] /; (NumberQ[z] && Precision[z] < Infinity)

FresnelC[z_] := Module[
	{ipiz2 = N[Pi, Precision[z]] I z^2/2},
	If[Head[z] === Real,
	    z Re[Hypergeometric1F1[1/2, 3/2, ipiz2]],
	    z (Hypergeometric1F1[1/2, 3/2, ipiz2] + Hypergeometric1F1[1/2, 3/2, -ipiz2])/2
	]
] /; (NumberQ[z] && Precision[z] < Infinity)

SinIntegral[0]:=0
CosIntegral[0]:=-Infinity
SinhIntegral[0]:=0
CoshIntegral[0]:=-Infinity
FresnelS[0]:=0
FresnelC[0]:=0

(* rules for DirectedInfinity *)
SinIntegral[DirectedInfinity[d_]] :=
	Switch[d,
	    1, 	Pi/2,
	    I,	DirectedInfinity[I],
	    -1,	-Pi/2,
	    -I,	DirectedInfinity[-I]
	] /; MemberQ[{1, I, -1, -I}, d]

CosIntegral[DirectedInfinity[d_]] :=
	Switch[d,
	    1, 	0,
	    I,	DirectedInfinity[1] + I Pi/2,
	    -1,	-I Pi,
	    -I,	DirectedInfinity[1] - I Pi/2
	] /; MemberQ[{1, I, -1, -I}, d]

SinhIntegral[DirectedInfinity[d_]] :=
	Switch[d,
	    1, 	DirectedInfinity[1],
	    I,	I Pi/2,
	    -1,	DirectedInfinity[-1],
	    -I,	-I Pi/2
	] /; MemberQ[{1, I, -1, -I}, d]

CoshIntegral[DirectedInfinity[d_]] :=
	Switch[d,
	    1, 	DirectedInfinity[1],
	    I,	I Pi/2,
	    -1,	DirectedInfinity[1] + I Pi,
	    -I,	-I Pi/2
	] /; MemberQ[{1, I, -1, -I}, d]

FresnelS[DirectedInfinity[d_]] :=
	Switch[d,
	    1, 	1/2,
	    I,	-I/2,
	    -1,	-1/2,
	    -I,	I/2
	] /; MemberQ[{1, I, -1, -I}, d]

FresnelC[DirectedInfinity[d_]] :=
	Switch[d,
	    1, 	1/2,
	    I,	I/2,
	    -1,	-1/2,
	    -I,	-I/2
	] /; MemberQ[{1, I, -1, -I}, d]

(* rules for Derivative *)
(*SinIntegral/: SinIntegral' := Sin[#]/# &
CosIntegral/: CosIntegral' := Cos[#]/# &
SinhIntegral/: SinhIntegral' := Sinh[#]/# &
CoshIntegral/: CoshIntegral' := Cosh[#]/# &
FresnelS/: FresnelS' := Sin[Pi/2 #^2]&
FresnelC/: FresnelC' := Cos[Pi/2 #^2]&*)
SinIntegral' := Sin[#]/# &
CosIntegral' := Cos[#]/# &
SinhIntegral' := Sinh[#]/# &
CoshIntegral' := Cosh[#]/# &
FresnelS' := Sin[Pi/2 #^2]&
FresnelC' := Cos[Pi/2 #^2]&

Attributes[SinIntegral] = {Listable, NumericFunction, ReadProtected};
Attributes[CosIntegral] = {Listable, NumericFunction, ReadProtected};
Attributes[SinhIntegral] = {Listable, NumericFunction, ReadProtected};
Attributes[CoshIntegral] = {Listable, NumericFunction, ReadProtected};
Attributes[FresnelS] = {Listable, NumericFunction, ReadProtected};
Attributes[FresnelC] = {Listable, NumericFunction, ReadProtected};

Protect[SinIntegral, CosIntegral, SinhIntegral, CoshIntegral, FresnelS, FresnelC];


(* :Tests:

d1[z_] := Chop[1 - SinIntegral[z] / NIntegrate[Sin[t]/t, {t, 0, z},
		PrecisionGoal -> 13, Method -> DoubleExponential]];
d2[z_] := Chop[1 - SinhIntegral[z] / NIntegrate[Sinh[t]/t, {t, 0, z},
		PrecisionGoal -> 13, Method -> DoubleExponential]];
d3[z_] := Chop[1 - CosIntegral[z] / (N[EulerGamma] + Log[z] +
		NIntegrate[(Cos[t] - 1)/t, {t, 0, z},
			PrecisionGoal -> 13, Method -> DoubleExponential])];
d4[z_] := Chop[1 - CoshIntegral[z] / (N[EulerGamma] + Log[z] +
		NIntegrate[(Cosh[t] - 1)/t, {t, 0, z},
			PrecisionGoal -> 13, Method -> DoubleExponential])];
d5[z_] := Chop[1 - FresnelS[z] / NIntegrate[Sin[Pi/2 t^2], {t, 0, z},
		PrecisionGoal -> 13, Method -> DoubleExponential]];
d6[z_] := Chop[1 - FresnelC[z] / NIntegrate[Cos[Pi/2 t^2], {t, 0, z},
		PrecisionGoal -> 13, Method -> DoubleExponential]];

Do[zz = Random[Complex, {-20-20I, 20+20I}];
	Print[{zz, d1[zz], d2[zz], d3[zz], d4[zz], d5[zz], d6[zz]}], {20}];
Do[zz = Random[Real, {-20, 20}];
	Print[{zz, d1[zz], d2[zz], d3[zz], d4[zz], d5[zz], d6[zz]}], {20}];
Do[zz = Random[Real, {-20, 20}]I;
	Print[{zz, d1[zz], d2[zz], d3[zz], d4[zz], d5[zz], d6[zz]}], {20}];*)
