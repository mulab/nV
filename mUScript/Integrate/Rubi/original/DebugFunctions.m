(* ::Package:: *)

(* ::Title:: *)
(*Debug Functions*)


nf[u_] := NormalForm[u,x]
rf[u_] := RecognizedFormQ[u,x]


ds := (ShowSteps=True; Null);
ns := (ShowSteps=False; Null);


sim[u_] := Simplify[u];
fsim[u_] := FullSimplify[u];


ee[u_] := ExpnExpand[u,x];
simp[u_] := Simp[u];
ts[u_] := TrigSimplify[u]


dx[u_] := D[u,x];
sdx[u_] := SimplifyD[u,x];
fsdx[u_] := FullSimplify[SimplifyD[u,x]]


dz[u_] := D[u,z];
sdz[u_] := SimplifyD[u,z];
fsdz[u_] := FullSimplify[SimplifyD[u,z]]


SimplifyD[u_,x_] := If[SumQ[u], Simplify[Map[Function[Simplify[D[#,x]]],u]], Simplify[D[u,x]]]


int[u_] := Int[u,x];
sint[u_] := (ShowSteps=False; Simplify[Int[u,x]]);
fsint[u_] := (ShowSteps=False; FullSimplify[Int[u,x]]);


ix[u_] := Integrate[u,x];
six[u_] := Simplify[Integrate[u,x]];
fsix[u_] := FullSimplify[Integrate[u,x]]


IndentedPrint[n_Integer,u_] :=
  (Print[StringJoin[MakeList[n," "]],u]; u)


Prt[u_] := (Print[u]; u)


MakeList[n_Integer,u_] :=
  If[n==0, {}, Append[MakeList[n-1,u],u]]


(* ::Subsection::Closed:: *)
(*Symmetric Mod Function Simplification Rules*)


Mods[u_] :=
  If[LeafCount[1/2-u]<LeafCount[u],
    ArcCot[Tan[Pi*(1/2-u)]]/Pi,
  ArcTan[Tan[Pi*u]]/Pi]


(* Mods[u] is the symmetric mod of u. *)
Mods[n_] :=
  1/2-Mod[1-2*n,2]/2 /;
NumericQ[n]


(* Mods'[u_] := 1 *)
