(* ::Package:: *)

Begin["System`"]

{\[Infinity],\[Pi],E,I} = {Infinity,Pi,E,I}
Simplify[x_] := Expand[x]
FullSimplify[x_]:=Simplify[x]
Usage[x_Symbol] := x::usage
mU := True
PossibleZeroQ[x_] := Simplify[x] == 0

End[]

(*
simpson[f_, a_, b_, n_] := 
  Module[{h = N[(b - a)/n], u, v, r}, 
   u = Table[N[f[a + i h]], {i, 0, n}];
   v = Table[u[[i]] + u[[i + 1]], {i, n}];
   r = u[[1]] + u[[n + 1]] + 2 v[[1]];
   Do[r += 2 u[[i]] + 2 v[[i]], {i, 2, n}]; (h/6) r];
f[x_] := 1/(x^2 + 1);
simpson[f, 0, 1, 200]


<<"../Calculus/cn/init.m"
Calculus`Rubi`Int[x,x]
*)
