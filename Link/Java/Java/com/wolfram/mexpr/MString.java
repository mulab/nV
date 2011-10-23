// Decompiled by Jad v1.5.8e2. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://kpdus.tripod.com/jad.html
// Decompiler options: packimports(3) fieldsfirst ansi space 

package com.wolfram.mexpr;

import com.wolfram.mexpr.visitors.MExprVisitor;

// Referenced classes of package com.wolfram.mexpr:
//			MExpr, MExprUtilities, IMExprToken

public class MString extends MExpr
{

	String data;

	public MString(String s)
	{
		data = s;
		charStart = -1;
		charEnd = -1;
	}

	public MString(String s, IMExprToken imexprtoken)
	{
		data = s;
		setCharPositions(imexprtoken);
	}

	public int length()
	{
		return 0;
	}

	private String getStringData()
	{
		return data;
	}

	public boolean sameQ(MExpr mexpr)
	{
		if (!(mexpr instanceof MString))
		{
			return false;
		} else
		{
			MString mstring = (MString)mexpr;
			return data.equals(mstring.getStringData());
		}
	}

	public String toString()
	{
		return MExprUtilities.escapeString(data);
	}

	public String toJavaString()
	{
		return data;
	}

	public void accept(MExprVisitor mexprvisitor)
	{
		mexprvisitor.visit(this);
	}

	public MExpr head()
	{
		return MExpr.STRINGexpr;
	}

	public MExpr part(int i)
	{
		if (i == 0)
			return head();
		else
			throw new IllegalArgumentException("Cannot take part " + i + " from this Expr because it is an atom.");
	}
}
