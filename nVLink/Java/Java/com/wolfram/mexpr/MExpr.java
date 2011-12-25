// Decompiled by Jad v1.5.8e2. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://kpdus.tripod.com/jad.html
// Decompiler options: packimports(3) fieldsfirst ansi space 

package com.wolfram.mexpr;

import com.wolfram.mexpr.visitors.MExprVisitor;

// Referenced classes of package com.wolfram.mexpr:
//			MOperator, MInteger, MSymbol, IMExprToken

public abstract class MExpr
{

	public static final MExpr PLUSexpr = new MOperator("Plus");
	public static final MExpr PREINCREMENTexpr = new MOperator("PreIncrement");
	public static final MExpr PREDECREMENTexpr = new MOperator("PreDecrement");
	public static final MExpr INCREMENTexpr = new MOperator("Increment");
	public static final MExpr DECREMENTexpr = new MOperator("Decrement");
	public static final MExpr TIMESexpr = new MOperator("Times");
	public static final MExpr RULEexpr = new MOperator("Rule");
	public static final MExpr RULEDELAYEDexpr = new MOperator("RuleDelayed");
	public static final MExpr POWERexpr = new MOperator("Power");
	public static final MExpr LISTexpr = new MOperator("List");
	public static final MExpr SETexpr = new MOperator("Set");
	public static final MExpr SETDELAYEDexpr = new MOperator("SetDelayed");
	public static final MExpr UPSETexpr = new MOperator("UpSet");
	public static final MExpr UPSETDELAYEDexpr = new MOperator("UpSetDelayed");
	public static final MExpr TAGSETexpr = new MOperator("TagSet");
	public static final MExpr TAGSETDELAYEDexpr = new MOperator("TagSetDelayed");
	public static final MExpr BLANKexpr = new MOperator("Blank");
	public static final MExpr BLANKSEQUENCEexpr = new MOperator("BlankSequence");
	public static final MExpr BLANKNULLSEQUENCEexpr = new MOperator("BlankNullSequence");
	public static final MExpr PATTERNexpr = new MOperator("Pattern");
	public static final MExpr OPTIONALexpr = new MOperator("Optional");
	public static final MExpr CONDITIONexpr = new MOperator("Condition");
	public static final MExpr ALTERNATIVESexpr = new MOperator("Alternatives");
	public static final MExpr DOTexpr = new MOperator("Dot");
	public static final MExpr FUNCTIONexpr = new MOperator("Function");
	public static final MExpr PARTexpr = new MOperator("Part");
	public static final MExpr COMPOUNDEexpr = new MOperator("CompoundExpression");
	public static final MExpr NOTexpr = new MOperator("Not");
	public static final MExpr FACTORIALexpr = new MOperator("Factorial");
	public static final MExpr FACTORIAL2expr = new MOperator("Factorial2");
	public static final MExpr DERIVATIVE1expr = new MOperator("Derivative1");
	public static final MExpr PATTERNTESTexpr = new MOperator("PatternTest");
	public static final MExpr NULLexpr = new MOperator("Null");
	public static final MExpr SAMEQexpr = new MOperator("SameQ");
	public static final MExpr UNSAMEQexpr = new MOperator("UnsameQ");
	public static final MExpr EQUALexpr = new MOperator("Equal");
	public static final MExpr UNEQUALexpr = new MOperator("Unequal");
	public static final MExpr GREATERexpr = new MOperator("Greater");
	public static final MExpr LESSexpr = new MOperator("Less");
	public static final MExpr GREATEREQUALexpr = new MOperator("GreaterEqual");
	public static final MExpr LESSEQUALexpr = new MOperator("LessEqual");
	public static final MExpr INEQUALITYexpr = new MOperator("Inequality");
	public static final MExpr GETexpr = new MOperator("Get");
	public static final MExpr PUTexpr = new MOperator("Put");
	public static final MExpr PUTAPPENDexpr = new MOperator("PutAppend");
	public static final MExpr PACKAGEexpr = new MOperator("Package");
	public static final MExpr JOINPACKAGEexpr = new MOperator("JoinPackage");
	public static final MExpr MESSAGENAMEexpr = new MOperator("MessageName");
	public static final MExpr UNSETexpr = new MOperator("Unset");
	public static final MExpr TAGUNSETexpr = new MOperator("TagUnset");
	public static final MExpr REPEATEDexpr = new MOperator("Repeated");
	public static final MExpr REPEATEDNULLexpr = new MOperator("RepeatedNull");
	public static final MExpr SLOTexpr = new MOperator("Slot");
	public static final MExpr SLOTSEQUENCEexpr = new MOperator("SlotSequence");
	public static final MExpr MAPexpr = new MOperator("Map");
	public static final MExpr MAPALLexpr = new MOperator("MapAll");
	public static final MExpr APPLYexpr = new MOperator("Apply");
	public static final MExpr APPLYONEexpr = new MOperator("ApplyOne");
	public static final MExpr ANDexpr = new MOperator("And");
	public static final MExpr ORexpr = new MOperator("Or");
	public static final MExpr STRINGJOINexpr = new MOperator("StringJoin");
	public static final MExpr STRINGEXPRESSIONexpr = new MOperator("StringExpression");
	public static final MExpr REPLACEALLexpr = new MOperator("ReplaceAll");
	public static final MExpr REPLACEREPEATEDexpr = new MOperator("ReplaceRepeated");
	public static final MExpr PARENexpr = new MOperator("Parenthesis");
	public static final MExpr MINUSexpr = new MOperator("Minus");
	public static final MExpr SUBTRACTexpr = new MOperator("Subtract");
	public static final MExpr DIVIDEexpr = new MOperator("Divide");
	public static final MExpr RECIPROCALexpr = new MOperator("Reciprocal");
	public static final MExpr ADDTOexpr = new MOperator("AddTo");
	public static final MExpr SUBTRACTFROMexpr = new MOperator("SubtractFrom");
	public static final MExpr TIMESBYexpr = new MOperator("TimesBy");
	public static final MExpr DIVIDEBYexpr = new MOperator("DivideBy");
	public static final MExpr NONCOMMUTATIVEMULTIPLYexpr = new MOperator("NonCommutativeMultiply");
	public static final MExpr OUTexpr = new MOperator("Out");
	public static final MExpr MinusOne = new MInteger("-1");
	public static final MExpr ENULLexpr = new MSymbol("<ENULL>");
	public static final MExpr SYMBOLexpr = new MSymbol("Symbol");
	public static final MExpr STRINGexpr = new MSymbol("String");
	public static final MExpr INTEGERexpr = new MSymbol("Integer");
	public static final MExpr REALexpr = new MSymbol("Real");
	public static final MExpr TYPESETexpr = new MSymbol("Typeset");
	public static final MExpr ERRORNODEexpr = new MSymbol("ERROR_NODE");
	public static final MExpr TYPESETPARENexpr = new MOperator("TypesetParen");
	public static final MExpr TYPESETSUPERSCRIPTexpr = new MOperator("TypesetSuperscript");
	public static final MExpr TYPESETSUBSCRIPTexpr = new MOperator("TypesetSubscript");
	public static final MExpr TYPESETDIVIDEexpr = new MOperator("TypesetDivide");
	public static final MExpr TYPESETSQRTexpr = new MOperator("TypesetSqrt");
	public static final MExpr TYPESETFULLFORMexpr = new MOperator("TypesetFullForm");
	public int charStart;
	public int charEnd;

	public MExpr()
	{
		charStart = -1;
		charEnd = -1;
	}

	public abstract int length();

	public abstract boolean sameQ(MExpr mexpr);

	public abstract String toString();

	public abstract MExpr head();

	public abstract MExpr part(int i);

	public abstract void accept(MExprVisitor mexprvisitor);

	public void setCharPositions(IMExprToken imexprtoken)
	{
		if (imexprtoken != null)
		{
			setCharStart(imexprtoken);
			setCharEnd(imexprtoken);
		}
	}

	public void setCharEnd(IMExprToken imexprtoken)
	{
		charEnd = imexprtoken.getTokenIndex() >= 0 ? imexprtoken.getTokenIndex() : imexprtoken.getCharEnd();
	}

	public void setCharStart(IMExprToken imexprtoken)
	{
		charStart = imexprtoken.getTokenIndex() >= 0 ? imexprtoken.getTokenIndex() : imexprtoken.getCharStart();
	}

	public void setCharEnd(int i)
	{
		charEnd = i;
	}

	public void setCharStart(int i)
	{
		charStart = i;
	}

	public void setCharPositions(int i, int j)
	{
		charStart = i;
		charEnd = j;
	}

	public void cloneCharPositions(MExpr mexpr)
	{
		charStart = mexpr.charStart;
		charEnd = mexpr.charEnd;
	}

	public int getCharStart()
	{
		return charStart;
	}

	public int getCharEnd()
	{
		return charEnd;
	}

	public boolean isError()
	{
		if (!(this instanceof MOperator))
		{
			return false;
		} else
		{
			MOperator moperator = (MOperator)this;
			return moperator.symbolName().equals("ERROR_NODE");
		}
	}
}
