// Decompiled by Jad v1.5.8e2. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://kpdus.tripod.com/jad.html
// Decompiler options: packimports(3) fieldsfirst ansi space 

package com.wolfram.mexpr;


// Referenced classes of package com.wolfram.mexpr:
//			MSymbol, IMExprToken

public class MOperator extends MSymbol
{

	public MOperator(String s)
	{
		super(s);
	}

	public MOperator(String s, IMExprToken imexprtoken)
	{
		super(s, imexprtoken);
	}

	public MOperator(IMExprToken imexprtoken)
	{
		super(imexprtoken.getText(), imexprtoken);
	}
}
