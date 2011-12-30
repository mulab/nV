// Decompiled by Jad v1.5.8e2. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://kpdus.tripod.com/jad.html
// Decompiler options: packimports(3) fieldsfirst ansi space 

package com.wolfram.mexpr;


// Referenced classes of package com.wolfram.mexpr:
//			MNormal, MExpr

public class MNormalSpecial extends MNormal
{

	public MNormalSpecial(MExpr mexpr, MExpr mexpr1, MExpr mexpr2)
	{
		super(mexpr, mexpr1, mexpr2);
		setCharStart(mexpr1.getCharStart());
	}

	public MNormalSpecial(MExpr mexpr, MExpr mexpr1)
	{
		super(mexpr, mexpr1);
		setCharStart(mexpr1.getCharStart());
		setCharEnd(mexpr.getCharEnd());
	}
}
