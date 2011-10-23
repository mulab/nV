// Decompiled by Jad v1.5.8e2. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://kpdus.tripod.com/jad.html
// Decompiler options: packimports(3) fieldsfirst ansi space 

package com.wolfram.mexpr;

import java.util.Vector;

// Referenced classes of package com.wolfram.mexpr:
//			MExpr, IMExprToken

public class MExprVector
{

	Vector args;
	public boolean isPart;
	public int charEnd;

	public MExprVector()
	{
		args = new Vector();
		isPart = false;
	}

	public void add(MExpr mexpr)
	{
		args.add(mexpr);
	}

	public void setCharEnd(IMExprToken imexprtoken)
	{
		charEnd = imexprtoken.getTokenIndex() >= 0 ? imexprtoken.getTokenIndex() : imexprtoken.getCharEnd();
	}

	public void prepend(MExpr mexpr)
	{
		args.add(0, mexpr);
	}

	public int length()
	{
		return args.size();
	}

	public MExpr getElem(int i)
	{
		return (MExpr)args.elementAt(i);
	}
}
