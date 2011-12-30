// Decompiled by Jad v1.5.8e2. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://kpdus.tripod.com/jad.html
// Decompiler options: packimports(3) fieldsfirst ansi space 

package com.wolfram.mexpr;

import java.util.Vector;

// Referenced classes of package com.wolfram.mexpr:
//			MExprVector

public class MExprHeadVector
{

	Vector args;

	public MExprHeadVector()
	{
		args = new Vector(2);
	}

	public void add(MExprVector mexprvector)
	{
		args.add(mexprvector);
	}

	public int length()
	{
		return args.size();
	}

	public MExprVector getElem(int i)
	{
		return (MExprVector)args.get(i);
	}
}
