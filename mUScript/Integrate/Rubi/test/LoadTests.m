(* ::Package:: *)

BeginPackage["Calculus`Rubi`"]
Begin["`Private"]

TestsDirectory = (Calculus`CWD <> "Rubi/test/")
Get[TestsDirectory<>"TestUtilityFunctions.m"]

Test[TestsDirectory<>"RationalFunctions/RationalFunctionsOfLinears.m"]
Test[TestsDirectory<>"RationalFunctions/RationalFunctionsOfBinomials.m"]
Test[TestsDirectory<>"RationalFunctions/RationalFunctionsOfTrinomials.m"]

Test[TestsDirectory<>"AlgebraicFunctions/AlgebraicFunctionsOfLinears.m"]
  Test[TestsDirectory<>"AlgebraicFunctions/AlgebraicFunctionsOfBinomials.m"]
  Test[TestsDirectory<>"AlgebraicFunctions/AlgebraicFunctionsOfTrinomials.m"]

Test[TestsDirectory<>"ExponentialFunctions/ExponentialFunctions.m"]
Test[TestsDirectory<>"ExponentialFunctions/LogarithmFunctions.m"]

Test[TestsDirectory<>"TrigFunctions/cos(x)^m (a+a cos(x))^n.m"]
Test[TestsDirectory<>"TrigFunctions/cos(x)^m (a+b cos(x))^n.m"]
Test[TestsDirectory<>"TrigFunctions/cos(x)^m (b cos(x))^n.m"]
Test[TestsDirectory<>"TrigFunctions/CosecantFunctions.m"]
Test[TestsDirectory<>"TrigFunctions/CosineFunctions.m"]
Test[TestsDirectory<>"TrigFunctions/CotangentFunctions.m"]
Test[TestsDirectory<>"TrigFunctions/csc(x)^m (a+a csc(x))^n.m"]
Test[TestsDirectory<>"TrigFunctions/csc(x)^m (a+b csc(x))^n.m"]
Test[TestsDirectory<>"TrigFunctions/sec(x)^m (a+a sec(x))^n.m"]
Test[TestsDirectory<>"TrigFunctions/sec(x)^m (a+b sec(x))^n.m"]
Test[TestsDirectory<>"TrigFunctions/sec(x)^m (b sec(x))^n.m"]
Test[TestsDirectory<>"TrigFunctions/SecantFunctions.m"]
Test[TestsDirectory<>"TrigFunctions/sin(x)^m (a+a sin(x))^n.m"]
Test[TestsDirectory<>"TrigFunctions/sin(x)^m (a+b sin(x))^n.m"]
Test[TestsDirectory<>"TrigFunctions/SineFunctions.m"]
Test[TestsDirectory<>"TrigFunctions/TangentFunctions.m"]
Test[TestsDirectory<>"TrigFunctions/TrigFunctions.m"]
Test[TestsDirectory<>"TrigFunctions/TwoTrigFunctions.m"]

Test[TestsDirectory<>"HyperbolicFunctions/HyperbolicCosecantFunctions.m"]
Test[TestsDirectory<>"HyperbolicFunctions/HyperbolicCosineFunctions.m"]
Test[TestsDirectory<>"HyperbolicFunctions/HyperbolicCotangentFunctions.m"]
Test[TestsDirectory<>"HyperbolicFunctions/HyperbolicFunctions.m"]
Test[TestsDirectory<>"HyperbolicFunctions/HyperbolicSecantFunctions.m"]
Test[TestsDirectory<>"HyperbolicFunctions/HyperbolicSineFunctions.m"]
Test[TestsDirectory<>"HyperbolicFunctions/HyperbolicTangentFunctions.m"]
Test[TestsDirectory<>"HyperbolicFunctions/TwoHyperbolicFunctions.m"]

  Test[TestsDirectory<>"InverseTrigFunctions/InverseSineFunctions.m"]
  Test[TestsDirectory<>"InverseTrigFunctions/InverseCosineFunctions.m"]
  Test[TestsDirectory<>"InverseTrigFunctions/InverseTangentFunctions.m"]
  Test[TestsDirectory<>"InverseTrigFunctions/InverseCotangentFunctions.m"]
  Test[TestsDirectory<>"InverseTrigFunctions/InverseSecantFunctions.m"]
  Test[TestsDirectory<>"InverseTrigFunctions/InverseCosecantFunctions.m"]

  Test[TestsDirectory<>"InverseHyperbolicFunctions/InverseHyperbolicSineFunctions.m"]
  Test[TestsDirectory<>"InverseHyperbolicFunctions/InverseHyperbolicCosineFunctions.m"]
  Test[TestsDirectory<>"InverseHyperbolicFunctions/InverseHyperbolicTangentFunctions.m"]
  Test[TestsDirectory<>"InverseHyperbolicFunctions/InverseHyperbolicCotangentFunctions.m"]
  Test[TestsDirectory<>"InverseHyperbolicFunctions/InverseHyperbolicSecantFunctions.m"]
  Test[TestsDirectory<>"InverseHyperbolicFunctions/InverseHyperbolicCosecantFunctions.m"]

  Test[TestsDirectory<>"SpecialFunctions/ErrorFunctions.m"]
  Test[TestsDirectory<>"SpecialFunctions/FresnelIntegralFunctions.m"]
  Test[TestsDirectory<>"SpecialFunctions/ExponentialIntegralFunctions.m"]
  Test[TestsDirectory<>"SpecialFunctions/TrigIntegralFunctions.m"]
  Test[TestsDirectory<>"SpecialFunctions/HyperbolicIntegralFunctions.m"]
  Test[TestsDirectory<>"SpecialFunctions/LogarithmIntegralFunctions.m"]
  Test[TestsDirectory<>"SpecialFunctions/GammaFunctions.m"]
  Test[TestsDirectory<>"SpecialFunctions/ZetaFunctions.m"]
  Test[TestsDirectory<>"SpecialFunctions/PolylogarithmFunctions.m"]
  Test[TestsDirectory<>"SpecialFunctions/ProductLogarithmFunctions.m"]

  TestsDirectory=.
  
End[]
EndPackage[]