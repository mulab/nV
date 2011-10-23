(* ::Package:: *)

BeginPackage["System`"]

Unprotect[I, Complex, Re, Im, Abs, Conjugate, Arg];
I::usage = "I represents the imaginary unit Sqrt[-1].";
Complex::usage = "Complex is used as the head of complex numbers.";
Re::usage = "Re[z] gives the real part of a complex number z.";
Im::usage = "Im[z] gives the imaginary part of a complex number z";
Arg::usage = "Arg[z] gives the argument of a complex number z";
Conjugate::usage = "Conjugate[z] or z^\[Conjugate] gives the complex conjugate of z";

SetAttributes[I,{Locked,ReadProtected}];
SetAttributes[Re,{Listable,NumericFunction}];
SetAttributes[Im,{Listable,NumericFunction}];
SetAttributes[Conjugate,{Listable,NumericFunction}];
SetAttributes[Arg,{Listable,NumericFunction}];

CProc["musystem", Complex, ComplexRe, ComplexIm, ComplexConjugate]

Begin["`Private`"]


(*-----------------------------------------------------------*)
(* :Summary: 	Basic Definition of {I, Complex, Re, Im, Abs, Conjugate, Arg} for atom numbers
	It's better to move these basics into nV kernel.
*)

I := Complex[0,1];
(* LACKS: Switch
Should better replace with general z_Complex^x?NumberQ rules
I ^n_Integer := Switch[Mod[n,4],
		0,	1,
		1,	I,
		2,	-1,
		3,	-I
	];
Sqrt[x_?Negative] := Sqrt[-x] * I;
LACKS *)

(* Re[] and Im[] are only for complex numbers in mU Kernel *)
Re[x_Complex] := ComplexRe[x]
Im[x_Complex] := ComplexIm[x]
Re[x_Real] := x;
Im[x_Real] := 0;
Re[x_Rational] := x;
Im[x_Rational] := 0;
Re[x_Integer] := x;
Im[x_Integer] := 0;
(* Can be combined by | *)

Complex[x_,0] := x;
(*
x_Complex + y_?NumberQ := Complex[Re[x]+Re[y], Im[x]+Im[y]];
x_Complex * y_Complex := Complex[Re[x]*Re[y]-Im[x]*Im[y], Im[x]*Re[y]+Re[x]*Im[y]];
x_Complex * y_Real := Complex[Re[x]*y, Im[x]*y];
x_Complex * y_Rational := Complex[Re[x]*y, Im[x]*y];
x_Complex * y_Integer := Complex[Re[x]*y, Im[x]*y];
x_Complex ^ n_Integer?Function[#<0] := Complex[Re[x]/(Re[x]^2+Im[x]^2), -Im[x]/(Re[x]^2+Im[x]^2)]^(-n); (* Use Negative instead of Function[#<0] ? *)
*)

Abs[z_Complex] := Sqrt[Re[z]^2+Im[z]^2];

Arg[z_?NumberQ] := Module[
	{x=Re[z], y=Im[z]},
		If[x==0,
			If[y==0,
				0,
				If[y>0,
					Pi/2,
					-Pi/2
				]
			],
			If[x>0,
				ArcTan[y/x],
				If[y>=0,
					ArcTan[y/x]+Pi,
					ArcTan[y/x]-Pi
				]
			]
		]
	];

Conjugate[z_?NumberQ] := ComplexConjugate[z]
(*Conjugate[z_] := Re[z]-Im[z]*I /; NumberQ[z];*)

(*-----------------------------------------------------------*)
(* :Summary: 	Simplifications with Re[] and Im[]
                                                                      
  The basic idea is to allow variables to be assumed real.            
  To declare a variable x real, use                                   
                                                                      
  	x/: Im[x] = 0                                                   
                                                                      
  There are also rules for Arg[], Abs[], and Conjugate[].             
  Sometimes more simplifications are possible if variables are        
  known to be positive or negative (e.g., with Log[x]).                
  Such declarations can be made with                                 
                                                                      
  	p /: Positive[p] = True                                         
                                                                      
  There can be problems with multivalued functions, e.g., with
                                                                      
  			n /: Negative[n] = True                                        
  we get 
  			Im[Log[-n]] --> 2Pi                                         
                                                                      
  Functions can also be declared real-valued, meaning that f[x]       
  is real if x is real. To declare a function to be real valued, use                        
  	RealValued[f, g,...]                                            
                                                                      
  Reading in this file makes such definitions for some of the         
  built-in functions, for example the trigonometric ones.             
                                                                      
  See also ComplexExpand[].
*)

(* :Warning: Re, Im, Abs, Conjugate, and Arg redefined. *)

RealValued::usage = "RealValued[f] declares f to be a real-valued function
	(for real-valued arguments)."

(* LACKS: Positive, Negative, PatternTest
(* fundamental rules, Im[x]==0 serves as our test for 'reality' *)

Re[x_] := x  /; Im[x] == 0
Arg[x_] := 0 /; Positive[x]
Arg[x_] :=Pi /; Negative[x]
Conjugate[x_] :=  x /; Im[x] == 0
Conjugate[x_] := -x /; Re[x] == 0

(* there must not be a rule for Im[x] in terms of Re[x] !! *)

(* things known to be real *)

Im[Re[_]] := 0
Im[Im[_]] := 0
Im[Abs[_]] := 0
Im[Arg[_]] := 0
Im[x_?Positive] = 0
Im[x_?Negative] = 0

Im[x_ ^ y_] := 0 /; Positive[x] && Im[y] == 0

Im[Log[r_?Positive]] := 0

(* arithmetic *)

Re[x_Plus] := Re /@ x
Im[x_Plus] := Im /@ x

Re[x_ y_Plus] := Re[Expand[x y]]
Im[x_ y_Plus] := Im[Expand[x y]]

Re[x_ y_] := Re[x] Re[y] - Im[x] Im[y]
Im[x_ y_] := Re[x] Im[y] + Im[x] Re[y]

(* hidden products *)
Re[(x_ y_)^k_] := Re[x^k y^k]
Im[(x_ y_)^k_] := Im[x^k y^k]

(* nested powers *)
Re[(x_^y_)^k_] := Re[x^(y k)]
Im[(x_^y_)^k_] := Im[x^(y k)]

Re[ 1/x_ ] :=  Re[x] / (Re[x]^2 + Im[x]^2)
Im[ 1/x_ ] := -Im[x] / (Re[x]^2 + Im[x]^2)

Im[x_^2] := 2 Re[x] Im[x]

Re[ x_^n_Integer ] :=
	Block[{a, b},
		a = Round[n/2]; b = n-a;
		Re[x^a] Re[x^b] - Im[x^a] Im[x^b]
	]

Im[ x_^n_Integer ] :=
	Block[{a, b},
		a = Round[n/2]; b = n-a;
		Re[x^a] Im[x^b] + Im[x^a] Re[x^b]
	]

Re[x_Integer^n_Rational] := 0                /; IntegerQ[2n] && Negative[x]
Im[x_Integer^n_Rational] := 
	(-x)^n (-1)^((Numerator[n]-1)/2)     /; IntegerQ[2n] && Negative[x]

(* functions *)

Re[Log[r_?Negative]] := Log[-r]
Im[Log[r_?Negative]] := Pi
Re[Log[z_]] := Log[Abs[z]] /; Im[z] == 0
Re[Log[z_]] := (1/2) Log[Re[z]^2 + Im[z]^2]
Im[Log[z_]] := Arg[z]

Re[Log[a_ b_]] := Re[Log[a] + Log[b]]
Im[Log[a_ b_]] := Im[Log[a] + Log[b]]
Re[Log[a_^c_]] := Re[c Log[a]]
Im[Log[a_^c_]] := Im[c Log[a]]

Re[E^x_] := Cos[Im[x]] Exp[Re[x]]
Im[E^x_] := Sin[Im[x]] Exp[Re[x]]

Re[Sin[x_]] := Sin[Re[x]] Cosh[Im[x]]
Im[Sin[x_]] := Cos[Re[x]] Sinh[Im[x]]

Re[Cos[x_]] :=  Cos[Re[x]] Cosh[Im[x]]
Im[Cos[x_]] := -Sin[Re[x]] Sinh[Im[x]]

Re[Sinh[x_]] := Sinh[Re[x]] Cos[Im[x]]
Im[Sinh[x_]] := Cosh[Re[x]] Sin[Im[x]]

Re[Cosh[x_]] := Cosh[Re[x]] Cos[Im[x]]
Im[Cosh[x_]] := Sinh[Re[x]] Sin[Im[x]]

(* conjugates *)

Re[Conjugate[z_]] :=  Re[z]
Im[Conjugate[z_]] := -Im[z]

Conjugate[x_Plus]:= Conjugate /@ x
Conjugate[x_Times]:= Conjugate /@ x
Conjugate[x_^n_Integer]:= Conjugate[x]^n
Conjugate[Conjugate[x_]]:= x

(* real-valued rules *)

Attributes[RealValued] = {Listable, HoldAll}
Attributes[RealValuedQ] = {HoldFirst}

RealValued[f_Symbol] := (f/: RealValuedQ[f] = True; f)
RealValued[f__] := RealValued /@ {f}

Im[ (_?RealValuedQ)[_?(Im[#]==0&)...] ] := 0

(* define built-in function to be real-valued *)

DoRules[flist_] :=
	Block[{protected},
		protected = Unprotect[flist];
		RealValued[flist];
		Protect[Evaluate[protected]]
	]

DoRules[{Sin, Cos, Tan, ArcSin, ArcCos, ArcTan, ArcCot,
	 Sinh, Cosh, Tanh, ArcSinh, ArcCosh, ArcTanh,
	 Floor, Ceiling, Round, Sign, Factorial}]

(*-----------------------------------------------------------*)
(*	:Keywords:
	Positivity, Positive, Negative, NonNegative, NonNegativeQ
*)

(*	:Limitations:
	NonNegativeQ is not in any sense general.
*)

(*	:Discussion:

	The Mathematica functions Positive, Negative and NonNegative evaluate
	for numbers only. They can be used to define properties of symbols
	but for combinations of such symbols, the properties are not evaluated
	any further.
	
	The function NonNegativeQ (defined in this package) tries to find out
	if the result can not be negative. In such cases, it returns True,
	otherwise False.
*)

(*	:Summary:
	The function NonNegativeQ tries to find out if expressions can not be negative.
*)

NonNegativeQ::usage =
"NonNegativeQ[expr] returns True if expr is obviously not negative and else 
False. Properties of symbols can be defined with the functions Positive, 
NonNegative and Negative (e.g.: x /: Positive[x] = True). Real symbols can be 
declared by definitions like a /: Im[a] = 0";

RealTest[x_?AtomQ] := True /; NonNegativeQ[x]

RealTest[x_] := True /; Im[x] == 0 ||
  Positive[x] || NonNegative[x] || Negative[x]

RealTest[x_ + y_] := RealTest[x] && RealTest[y]

RealTest[x_ y_] := RealTest[x] && RealTest[y]

RealTest[x_^_Integer] := RealTest[x]

NonNegativeQ[x_] := True /; Positive[x] || NonNegative[x]

NonNegativeQ[x_?(!AtomQ[#]&)] := NNQ[Factor[x]]

NonNegativeQ[x_] := False

NNQ[x_ y_] := (NNQ[x] && NNQ[y]) || (NNQ[-x] && NNQ[-y]) /;
    x =!= -1 && y =!= -1

NNQ[x_ + y_] := NNQ[x] && NNQ[y]

NNQ[x_^_?OddQ] := NNQ[x] /; RealTest[x]

NNQ[-x_^_?OddQ] := NNQ[-x] /; RealTest[x]

NNQ[x_^_?EvenQ] := True /; RealTest[x]

NNQ[Abs[x_]] := True

NNQ[Sqrt[x_]] := True /; RealTest[x]

NNQ[Exp[x_]] := True /; RealTest[x]

NNQ[-x_] := True /;
  Positive[-x] || NonNegative[-x] || Negative[x]

NNQ[x_] := True /;
  Positive[x] || NonNegative[x] || Negative[-x]

NNQ[x_?AtomQ] := NonNegativeQ[x]

NNQ[x_] := False

(*-----------------------------------------------------------*)
(*:Keywords:
	Complex Symbols, Abs, Arg
*)

(* Discussion

	Background:
	The built in functions Re, Im, Conjugate, Abs and Arg evaluate for
	numbers only. The package Algebra`ReIm` extends Re, Im and Conjugate
	to symbols and expressions. This package also extends Abs and Arg.

	Mathematica does not know a priori if a symbol stands for a real or
	a complex number. A real symbol can be defined by:
	
	a /: Im[a] = 0;
	
	or, equivalently, by:
	
	Im[a] ^= 0;
	
	Abs and Arg often allow further simplification if information about
	the positivity of symbols is given. This can be done with the functions
	Positive, Negative and NonNegative or with NonNegativeQ from the
	package SKPackages`NonNegativeQ`, e.g.:
	Positive[a] ^= True;
*)

Abs[z_] := Re[z] /; Im[z] == 0 && NonNegativeQ[Re[z]]

Abs[z_] := -Re[z] /; Im[z] == 0 && NonNegativeQ[-Re[z]]

Abs[z_] := Im[z] /; Re[z] == 0 && NonNegativeQ[Im[z]]

Abs[z_] := -Im[z] /; Re[z] == 0 && NonNegativeQ[-Im[z]]

Abs[z_] := Sqrt[Re[z]^2 + Im[z]^2]

Arg[z_] := 0 /; NonNegativeQ[z]

Arg[z_] := Pi /; NonNegativeQ[-z]

Arg[z_] := Pi/2 /; Re[z] == 0 && NonNegativeQ[Im[z]]

Arg[z_] := -Pi/2 /; Re[z] == 0 && NonNegativeQ[-Im[z]]

Arg[z_] := ArcTan[Im[z]/Re[z]] /; Re[z] =!= 0 && Im[z] =!= 0

LACKS *)


End[]

Protect[I, Complex, Re, Im, Abs, Conjugate, Arg];

EndPackage[]
