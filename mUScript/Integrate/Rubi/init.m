(* ::Package:: *)

BeginPackage["Calculus`Rubi`"]
Int::usage="Int[f[x],x] returns the integral";


Begin["`Private`"]
(* Edit ShowSteps and SimplifyFlag in ShowStep.m *)
Get[Calculus`CWD<>"Rubi/original/LoadRules.m"];
If[ShowSteps, StepFunction[Int]];
End[];


EndPackage[];


(*Int[E^x,x]
Int[x^4*(1-x^2)^(-5/2),x]
Int[x^(1/2)*(1+x)^(5/2),x]
Int[Log[3*(4*(5+6*x)^7)^2],x]
Int[(x^3)^(-1/3),x]
Int[x E^x,x]*)

(*Int[(a+b*Sqrt[x])^20/Sqrt[x],x]*)
(*Int[(1-2 Sqrt[x])^20/Sqrt[x],x]*)
