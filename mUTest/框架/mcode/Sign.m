(* ::Package:: *)

s={};
s=Append[s,Equal[ -1,-1]];
s=Append[s,Equal[ 1,1]];
s=Append[s,Equal[ 0,0]];
s=Append[s,Equal[ {-1,-1,0,1,1},{-1, -1, 0, 1, 1}]];
s=Append[s,Equal[ $Failed,XXXXXX]];
s=Append[s,Equal[ $Failed,-1]];
s=Append[s,Equal[ $Failed,(1 + I)/Sqrt[2]]];
s=Append[s,Equal[ sgn x,1]];
s=Append[s,Equal[ $Failed,Sign[x]]];
s=Append[s,Equal[ $Failed,1]];
s=Append[s,Equal[ $Failed,I]];
s=Append[s,Equal[ SparseArray[{{1,1}->-6,{1,3}->Pi I,{3,3}->2,{4,2}->u}],Indeterminate]];
s=Append[s,Equal[ $Failed,SparseArray[]]];
s=Append[s,Equal[ $Failed,SparseArray[]]];
s=Append[s,Equal[ XXXXXX,{{-1, 0, I}, {0, 0, 0}, {0, 0, 1}, {0, Sign[u], 0}}]];
s=Append[s,Equal[ {{1,0,0},{0,1,0},{0,0,1}},Piecewise[{{-1, x < 0}, {1, x > 0}}]]];
s=Append[s,Equal[ $Failed,XXXXXX]];
s=Append[s,Equal[ $Failed,Plot[Evaluate[Table[r[n, x] + n/5, {n, 5}]], {x, -Pi, Pi}, Exclusions -> Sin[2^5*Pi*x] == 0]]];
s=Append[s,Equal[ E^(I Im[z]),FullSimplify[Table[Integrate[r[n1, x]*r[n2, x], {x, 0, 1}], {n1, 3}, {n2, 3}]]]];
s=Append[s,Equal[ $Failed,Sign[1/z]]];
s=Append[s,Equal[ $Failed,Sign[z^2]]];
s=Append[s,Equal[ 2,Sign[Exp[z]]]];
s=Append[s,Equal[ $Failed,Sign[Sign[z]]]];
s=Append[s,Equal[ $Failed,FullSimplify[Sign[a*b] - Sign[a]*Sign[b] + Sign[a] - Abs[a]/a]]];

Export["E:\\Tsinghua\\mathmu\:8c03\:8bd5\\\:6846\:67b6\\mcode\\Sign_cmp.txt",s,"List"];



