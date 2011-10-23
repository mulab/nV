// Decompiled by Jad v1.5.8e2. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://kpdus.tripod.com/jad.html
// Decompiler options: packimports(3) fieldsfirst ansi space 

package com.wolfram.mexpr;

import com.wolfram.mexpr.visitors.MExprVisitor;

// Referenced classes of package com.wolfram.mexpr:
//			MExpr, IMExprToken

public class MReal extends MExpr
{

	String data;

	public MReal(String s)
	{
		data = s;
	}

	public MReal(IMExprToken imexprtoken)
	{
		data = imexprtoken.getText();
		setCharPositions(imexprtoken);
	}

	public int length()
	{
		return 0;
	}

	String getRealData()
	{
		return data;
	}

	public String toString()
	{
		return data;
	}

	public boolean sameQ(MExpr mexpr)
	{
		if (!(mexpr instanceof MReal))
		{
			return false;
		} else
		{
			MReal mreal = (MReal)mexpr;
			return data.equals(mreal.getRealData());
		}
	}

	public void accept(MExprVisitor mexprvisitor)
	{
		mexprvisitor.visit(this);
	}

	public MExpr head()
	{
		return MExpr.REALexpr;
	}

	public MExpr part(int i)
	{
		if (i == 0)
			return head();
		else
			throw new IllegalArgumentException("Cannot take part " + i + " from this Expr because it is an atom.");
	}
}
