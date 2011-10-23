(* ::Package:: *)

(* ::Title:: *)
(*Calculus Function Integration Rules*)


(* ::Subsection::Closed:: *)
(*Differentiation Function Integration Rules*)


(* ::Item::Closed:: *)
(*Reference: G&R 2.02.4*)


(* ::Item:: *)
(*Derivation: Primitive rule*)


Int[f_'[x_],x_Symbol] :=
  f[x] /;
FreeQ[f,x]


(* ::Item:: *)
(*Derivation: Primitive rule*)


Int[Derivative[n_][f_][x_],x_Symbol] :=
  Derivative[n-1][f][x] /;
FreeQ[{f,n},x]


(* ::Item::Closed:: *)
(*Reference: G&R 2.02.7*)


(* ::Item:: *)
(*Derivation: Integration by substitution*)


Int[u_*g_'[x_],x_Symbol] :=
  Subst[Int[Regularize[SubstFor[g[x],u,x],x],x],x,g[x]] /;
FreeQ[g,x] && FunctionOfQ[g[x],u,x]


(* ::Item:: *)
(*Derivation: Integration by substitution*)


Int[u_*Derivative[n_][g_][x_],x_Symbol] :=
  Subst[Int[Regularize[SubstFor[Derivative[n-1][g][x],u,x],x],x],x,Derivative[n-1][g][x]] /;
FreeQ[{g,n},x] && FunctionOfQ[Derivative[n-1][g][x],u,x]


(* ::Item::Closed:: *)
(*Reference: G&R 2.02.5*)


(* ::Item:: *)
(*Derivation: Inverse of derivative of a product rule*)


Int[f_'[x_]*g_[x_] + f_[x_]*g_'[x_],x_Symbol] :=
  f[x]*g[x] /;
FreeQ[{f,g},x]


(* ::Item::Closed:: *)
(*Reference: G&R 2.02.11*)


(* ::Item:: *)
(*Derivation: Inverse of derivative of a quotient rule*)


Int[(f_'[x_]*g_[x_] - f_[x_]*g_'[x_])/g_[x_]^2,x_Symbol] :=
  f[x]/g[x] /;
FreeQ[{f,g},x]


(* ::Item::Closed:: *)
(*Reference: G&R 2.02.12*)


(* ::Item:: *)
(*Derivation: Inverse of derivative of log of a quotient rule*)


Int[(f_'[x_]*g_[x_] - f_[x_]*g_'[x_])/(f_[x_]*g_[x_]),x_Symbol] :=
  Log[f[x]/g[x]] /;
FreeQ[{f,g},x]


(* ::Item::Closed:: *)
(*Reference: G&R 2.02.24*)


(* ::Item:: *)
(*Derivation: Inverse of derivative of arctangent of a quotient rule*)


Int[(f_'[x_]*g_[x_] - f_[x_]*g_'[x_])/(f_[x_]^2 + g_[x_]^2),x_Symbol] :=
  ArcTan[f[x]/g[x]] /;
FreeQ[{f,g},x]


(* ::Item::Closed:: *)
(*Reference: G&R 2.02.25*)


(* ::Item:: *)
(*Derivation: Inverse of derivative of log of a quotient rule*)


Int[(f_'[x_]*g_[x_] - f_[x_]*g_'[x_])/(f_[x_]^2 - g_[x_]^2),x_Symbol] :=
  Log[(f[x]-g[x])/(f[x]+g[x])]/2 /;
FreeQ[{f,g},x]
