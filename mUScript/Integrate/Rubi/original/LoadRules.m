(* ::Package:: *)

BeginPackage["Calculus`Rubi`"]

If[System`mU===True, DownValuesDefaultType[0]]
Unprotect[Int];
DownValues[Int]={};

Begin["`Private`"]

  RulesDirectory = (Calculus`CWD <> "Rubi/original/")
  Get[RulesDirectory<>"ShowStep.m"];
  Get[RulesDirectory<>"IntegrationUtilityFunctions.m"];
  Get[RulesDirectory<>"IntegrationSimplificationFunctions.m"];
  Get[RulesDirectory<>"DebugFunctions.m"];

  Get[RulesDirectory<>"RationalFunctions/RationalFunctionsOfLinears.m"];
  Get[RulesDirectory<>"RationalFunctions/RationalFunctionsOfBinomials.m"];
  Get[RulesDirectory<>"RationalFunctions/RationalFunctionsOfTrinomials.m"];

  Get[RulesDirectory<>"AlgebraicFunctions/AlgebraicFunctionsOfLinears.m"];
  Get[RulesDirectory<>"AlgebraicFunctions/AlgebraicFunctionsOfBinomials.m"];
  Get[RulesDirectory<>"AlgebraicFunctions/AlgebraicFunctionsOfTrinomials.m"];

  Get[RulesDirectory<>"ExponentialFunctions/ExponentialFunctions.m"];
  Get[RulesDirectory<>"ExponentialFunctions/ProductsOfExponentialAndTrigFunctions.m"];
  Get[RulesDirectory<>"ExponentialFunctions/ProductsOfExponentialAndHyperbolicFunctions.m"];
  Get[RulesDirectory<>"ExponentialFunctions/LogarithmFunctions.m"];

  Get[RulesDirectory<>"TrigFunctions/(sin^j)^m (a+a sin^k)^n.m"];
  Get[RulesDirectory<>"TrigFunctions/(sin^j)^m (a+b sin^k)^n.m"];
  Get[RulesDirectory<>"TrigFunctions/(sin^j)^m (A+B sin^k) (a+a sin^k)^n.m"];
  Get[RulesDirectory<>"TrigFunctions/(sin^j)^m (A+B sin^k) (a+b sin^k)^n.m"];
  Get[RulesDirectory<>"TrigFunctions/(sin^j)^m (A+B sin^k+C sin^(2k)) (a+a sin^k)^n.m"];
  Get[RulesDirectory<>"TrigFunctions/(sin^j)^m (A+B sin^k+C sin^(2k)) (a+b sin^k)^n.m"];
  Get[RulesDirectory<>"TrigFunctions/TrigNormalization.m"];
  Get[RulesDirectory<>"TrigFunctions/SineFunctions.m"];
  Get[RulesDirectory<>"TrigFunctions/TangentFunctions.m"];
  Get[RulesDirectory<>"TrigFunctions/SecantFunctions.m"];
  Get[RulesDirectory<>"TrigFunctions/RationalFunctionsOfSinesAndCosines.m"];
  Get[RulesDirectory<>"TrigFunctions/TwoTrigFunctions.m"];
  Get[RulesDirectory<>"TrigFunctions/TrigSubstitution.m"];

  Get[RulesDirectory<>"HyperbolicFunctions/HyperbolicSineFunctions.m"];
  Get[RulesDirectory<>"HyperbolicFunctions/HyperbolicTangentFunctions.m"];
  Get[RulesDirectory<>"HyperbolicFunctions/HyperbolicSecantFunctions.m"];
  Get[RulesDirectory<>"HyperbolicFunctions/RationalFunctionsOfHyperbolicSines.m"];
  Get[RulesDirectory<>"HyperbolicFunctions/RationalFunctionsOfHyperbolicSinesAndCosines.m"];
  Get[RulesDirectory<>"HyperbolicFunctions/TwoHyperbolicFunctions.m"];
  Get[RulesDirectory<>"HyperbolicFunctions/HyperbolicSubstitution.m"];

  Get[RulesDirectory<>"InverseTrigFunctions/InverseSineFunctions.m"];
  Get[RulesDirectory<>"InverseTrigFunctions/InverseCosineFunctions.m"];
  Get[RulesDirectory<>"InverseTrigFunctions/InverseTangentFunctions.m"];
  Get[RulesDirectory<>"InverseTrigFunctions/InverseCotangentFunctions.m"];
  Get[RulesDirectory<>"InverseTrigFunctions/InverseSecantFunctions.m"];
  Get[RulesDirectory<>"InverseTrigFunctions/InverseCosecantFunctions.m"];

  Get[RulesDirectory<>"InverseHyperbolicFunctions/InverseHyperbolicSineFunctions.m"];
  Get[RulesDirectory<>"InverseHyperbolicFunctions/InverseHyperbolicCosineFunctions.m"];
  Get[RulesDirectory<>"InverseHyperbolicFunctions/InverseHyperbolicTangentFunctions.m"];
  Get[RulesDirectory<>"InverseHyperbolicFunctions/InverseHyperbolicCotangentFunctions.m"];
  Get[RulesDirectory<>"InverseHyperbolicFunctions/InverseHyperbolicSecantFunctions.m"];
  Get[RulesDirectory<>"InverseHyperbolicFunctions/InverseHyperbolicCosecantFunctions.m"];

  Get[RulesDirectory<>"SpecialFunctions/ErrorFunctions.m"];
  Get[RulesDirectory<>"SpecialFunctions/FresnelIntegralFunctions.m"];
  Get[RulesDirectory<>"SpecialFunctions/ExponentialIntegralFunctions.m"];
  Get[RulesDirectory<>"SpecialFunctions/TrigIntegralFunctions.m"];
  Get[RulesDirectory<>"SpecialFunctions/HyperbolicIntegralFunctions.m"];
  Get[RulesDirectory<>"SpecialFunctions/LogarithmIntegralFunctions.m"];
  Get[RulesDirectory<>"SpecialFunctions/GammaFunctions.m"];
  Get[RulesDirectory<>"SpecialFunctions/ZetaFunctions.m"];
  Get[RulesDirectory<>"SpecialFunctions/PolylogarithmFunctions.m"];
  Get[RulesDirectory<>"SpecialFunctions/ProductLogarithmFunctions.m"];

  Get[RulesDirectory<>"MiscellaneousFunctions/CalculusFunctions.m"];

  Get[RulesDirectory<>"GeneralIntegrationRules.m"];
  RulesDirectory=.
End[]

If[System`mU===True, DownValuesDefaultType[1]]
EndPackage[]
