// Decompiled by Jad v1.5.8e2. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://kpdus.tripod.com/jad.html
// Decompiler options: packimports(3) fieldsfirst ansi space 

package com.wolfram.mexpr;

import com.wolfram.mexpr.visitors.MExprVisitor;

// Referenced classes of package com.wolfram.mexpr:
//			MExpr, IMExprToken

public class MInteger extends MExpr
{

	String data;

	public MInteger(String s)
	{
		data = s;
	}

	public MInteger(IMExprToken imexprtoken)
	{
		data = imexprtoken.getText();
		setCharPositions(imexprtoken);
	}

	public int length()
	{
		return 0;
	}

	public String getIntegerData()
	{
		return data;
	}

	public String toString()
	{
		return data;
	}

	public boolean sameQ(MExpr mexpr)
	{
		if (!(mexpr instanceof MInteger))
		{
			return false;
		} else
		{
			MInteger minteger = (MInteger)mexpr;
			return data.equals(minteger.getIntegerData());
		}
	}

	public void accept(MExprVisitor mexprvisitor)
	{
		mexprvisitor.visit(this);
	}

	public MExpr head()
	{
		return MExpr.INTEGERexpr;
	}

	public MExpr part(int i)
	{
		if (i == 0)
			return head();
		else
			throw new IllegalArgumentException("Cannot take part " + i + " from this Expr because it is an atom.");
	}
}
