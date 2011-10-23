// Decompiled by Jad v1.5.8e2. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://kpdus.tripod.com/jad.html
// Decompiler options: packimports(3) fieldsfirst ansi space 

package com.wolfram.mexpr;

import com.wolfram.mexpr.visitors.MExprVisitor;

// Referenced classes of package com.wolfram.mexpr:
//			MExpr, IMExprToken

public class MSymbol extends MExpr
{

	String name;

	public MSymbol(String s)
	{
		name = s;
	}

	public MSymbol(IMExprToken imexprtoken)
	{
		name = imexprtoken.getText();
		setCharPositions(imexprtoken);
	}

	public MSymbol(String s, IMExprToken imexprtoken)
	{
		name = s;
		setCharPositions(imexprtoken);
	}

	public String symbolName()
	{
		return name;
	}

	public void setSymbolName(String s)
	{
		name = s;
	}

	public int length()
	{
		return 0;
	}

	public String toString()
	{
		return name;
	}

	public boolean sameQ(MExpr mexpr)
	{
		if (!(mexpr instanceof MSymbol))
		{
			return false;
		} else
		{
			MSymbol msymbol = (MSymbol)mexpr;
			return symbolName().equals(msymbol.symbolName());
		}
	}

	public void accept(MExprVisitor mexprvisitor)
	{
		mexprvisitor.visit(this);
	}

	public MExpr head()
	{
		return MExpr.SYMBOLexpr;
	}

	public MExpr part(int i)
	{
		if (i == 0)
			return head();
		else
			throw new IllegalArgumentException("Cannot take part " + i + " from this Expr because it is an atom.");
	}
}
