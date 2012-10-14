(* ::Package:: *)

(* Begin["System`"] *)

SetAttributes[All,Protected]
SetAttributes[None,Protected]

CProc["musystem",COper,CInstall,Complex,CUninstall,Box,Defer,Timing,MatchQ,MemberQ,ReplaceRepeated, Replace,ReplaceAll,OddQ,EvenQ,SetAttributes,Pretty,StringJoin,Join,Attributes,NumberQ,AtomQ,IntegerQ,SymbolQ,StringQ,Clear,BeginPackage,EndPackage,Begin,End,Evaluate,Unevaluated,Dispatch,Length,Path,Head,Context, Contexts,ContextPath,Apply,Map,Unset,FullForm,ToString,ToExpression, Exit,Quit,Set,SetDelayed,Hold,Run,Array,Table,Do, N,IntegerPart,Floor,Ceiling,Round,Expand,Variables,Coefficient, Exponent,CoefficientList,FromCoefficientList,StringLength, Set,Property,SetDelayed,CompoundExpression,Power,Mod]
CProc["musystem", Increment, PreIncrement, Decrement, PreDecrement]
CProc["musystem", Quiet, DownValues, UpValues, Information, Protect, Unprotect, ListQ, NumericQ, Rest, Prepend, Dimensions, LeafCount, Min, Max]
CProc["musystem", Abort, Interrupt, AbortProtect, CheckAbort, Catch, Throw, Sow, Reap, Map, ConstantArray]
CProc["musystem", Part,Extract,First,Last,Most,Rest,Take,Drop,Depth,Scan,Level]
CProc["musystem", ReplacePart, Prepend,Append,Reverse,Distribute, Symbols, List, Range, Count, Union, Verbose,DownValuesDefaultType]
CProc["musystem",Pipe,Buffer,File,Link,Read,Write,Server,Request,Parse]
CProc["musystem", TraceRule]
(*Block.cpp*)
CProc["musystem",If,For,While,Flatten,FlattenAll,With,Block,Module,Order,Compare]
(*Inequality.cpp*)
CProc["musystem",SameQ,Less,Equal,Greater,UnsameQ,GreaterEqual,Unequal,LessEqual,FreeQ,And,Or,Not,Inequality]
(*Input.cpp*)
CProc["musystem",Input,Get,ReadList]
(*Print.cpp*)
CProc["musystem",Print,Put,PutAppend]
(*Differential.cpp*)
CProc["musystem",D,$MultiD]
COper["musystem",Function,Derivative,$Derivative]

SetAttributes[Information, {HoldAll, Protected, ReadProtected}]
SetAttributes[Increment, {HoldFirst, Protected, ReadProtected}]
SetAttributes[Decrement, {HoldFirst, Protected, ReadProtected}]
SetAttributes[PreIncrement, {HoldFirst, Protected, ReadProtected}]
SetAttributes[PreDecrement, {HoldFirst, Protected, ReadProtected}]
SetAttributes[Defer,{HoldAll,Protected,ReadProtected}]
SetAttributes[Map,{Protected}]
SetAttributes[Part,{NHoldRest,Protected,ReadProtected}]
SetAttributes[Extract,{NHoldRest,Protected}]
SetAttributes[First,Protected]
SetAttributes[Last,Protected]
SetAttributes[Most,Protected]
SetAttributes[Rest,Protected]
SetAttributes[Take,{NHoldRest,Protected}]
SetAttributes[Drop,{NHoldRest,Protected}]
SetAttributes[Scan,Protected]
SetAttributes[Level,Protected]
SetAttributes[ReplacePart,Protected]
SetAttributes[Prepend,Protected]
SetAttributes[Append,Protected]
SetAttributes[Reverse,Protected]
SetAttributes[Distribute,Protected]
SetAttributes[Symbols,Protected]
SetAttributes[List,{Locked, Protected}]
SetAttributes[Range,{Listable,Protected}]
SetAttributes[Count,Protected]
SetAttributes[Union,{Flat, Protected}] (* Union has attribute OneIdentity in mathematica *)
SetAttributes[ConstantArray, Protected]
SetAttributes[Min, {Flat, NumericFunction, Orderless, Protected}]
SetAttributes[Max, {Flat, NumericFunction, Orderless, Protected, ReadProtected}]
SetAttributes[Function, Protected]
SetAttributes[TraceRule, {Protected, HoldAll}]
SetAttributes[Quiet, {Protected, HoldAll}]

Default[Plus] = 0
Default[Times] = 1
Default[Power] = 1

General::general = "Unknown message"
General::error = "`1`"
General::argx = "`1` called with `2` arguments; 1 argument is expected."
General::argt = "`1` called with `2` arguments; `3` or `4` arguments are expected."
General::argr = "`1` called with 1 argument; `2` arguments are expected."
General::argrx = "`1` called with `2` arguments; `3` arguments are expected."
General::argb = "`1` called with `2` arguments; between `3` and `4` arguments are expected."
General::argbu = "`1` called with 1 argument; between `2` and `3` arguments are expected."
General::opttf = "Value of option `1` -> `2` should be True or False."
General::level = "Level specification `1` is not of the form n, {n}, or {m, n}."
General::nonopt = "Options expected (instead of `1`) beyond position `2` in `3`. An option must be a rule or a list of rules."
General::optx = "Unknown option `1` in `2`."
General::atomicx = "Exception thrown in atomic context `1`, kernel in unstable state."
Message::name = "Message name `1` is not of the form symbol::name."


End[]

<< Path["Number/mUNumber.m"]
<< Path["Polynomial/mUPolynomial.m"]
<< Path["Matrix/mUMatrix.m"]

<< NotebookDirectory[] <> "Complex.m"
<< NotebookDirectory[] <> "Fraction.m"
<< NotebookDirectory[] <> "Utility.m"
<< NotebookDirectory[] <> "Simplify.m"
<< NotebookDirectory[] <> "Sum.m"
<< NotebookDirectory[] <> "D.m"
<< NotebookDirectory[] <> "Range.m"
<< NotebookDirectory[] <> "String.m"
(*<< NotebookDirectory[] <> "TeX.m"*)

<< Path["mULink/EmbednV.m"]
<< NotebookDirectory[] <> "../Integrate/init.m"
(*<< NotebookDirectory[] <> "../Matrix/init.m"
<< NotebookDirectory[] <> "../Number/init.m"
<< NotebookDirectory[] <> "../Polynomial/init.m"*)