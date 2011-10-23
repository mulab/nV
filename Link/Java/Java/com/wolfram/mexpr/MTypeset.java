// Decompiled by Jad v1.5.8e2. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://kpdus.tripod.com/jad.html
// Decompiler options: packimports(3) fieldsfirst ansi space 

package com.wolfram.mexpr;

import com.wolfram.mexpr.visitors.MExprVisitor;

// Referenced classes of package com.wolfram.mexpr:
//			MExpr

public class MTypeset extends MExpr
{

	String data;

	public MTypeset(String s)
	{
		data = s;
	}

	public int length()
	{
		return 0;
	}

	public String getStringData()
	{
		return data;
	}

	public boolean sameQ(MExpr mexpr)
	{
		if (!(mexpr instanceof MTypeset))
		{
			return false;
		} else
		{
			MTypeset mtypeset = (MTypeset)mexpr;
			return data.equals(mtypeset.getStringData());
		}
	}

	public String toString()
	{
		return data;
	}

	public void accept(MExprVisitor mexprvisitor)
	{
		mexprvisitor.visit(this);
	}

	public MExpr head()
	{
		return MExpr.TYPESETexpr;
	}

	public MExpr part(int i)
	{
		if (i == 0)
			return head();
		else
			throw new IllegalArgumentException("Cannot take part " + i + " from this Expr because it is an atom.");
	}
}
