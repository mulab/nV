// Decompiled by Jad v1.5.8e2. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://kpdus.tripod.com/jad.html
// Decompiler options: packimports(3) fieldsfirst ansi space 

package com.wolfram.mexpr;

import java.util.Vector;

// Referenced classes of package com.wolfram.mexpr:
//			MExpr

public class ArgumentList
{

	Vector args;

	public ArgumentList()
	{
		args = new Vector(2);
	}

	public void add(MExpr mexpr)
	{
		args.add(mexpr);
	}

	public int length()
	{
		return args.size();
	}

	public MExpr getElem(int i)
	{
		return (MExpr)args.get(i);
	}

	public MExpr[] getArray()
	{
		Object aobj[] = args.toArray();
		MExpr amexpr[] = new MExpr[aobj.length];
		for (int i = 0; i < aobj.length; i++)
			amexpr[i] = (MExpr)aobj[i];

		return amexpr;
	}
}
