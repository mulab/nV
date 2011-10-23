(* ::Package:: *)

Test[file_String] := Do[Test[lst],{lst,ReadList[file]}]
Test[lst_List] := Module[{result},
result = Calculus`Rubi`Int[lst[[1]],lst[[2]]];
If[!FreeQ[result, Calculus`Rubi`Int], result = $Failed];
Print[lst[[1]]," : ",result];
If[result == lst[[4]],Print[":-)"],Print[":-( ",lst[[4]]]];(*Input[]*)]
