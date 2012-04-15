(* ::Package:: *)

Begin["System`"]

(*Range.cpp*)
CProc["musystem",Range]

SetAttributes[Range, {Listable, Protected}]

Range[x_]:=Range[1, x, 1]
Range[x_, y_]:=Range[x, y, 1]

Range::range = "Range specification in `1` does not have appropriate bounds."
Range::overflow = "Range specification in `1` have too large bounds to progress."

End[]
