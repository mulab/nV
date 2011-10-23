// Decompiled by Jad v1.5.8e2. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://kpdus.tripod.com/jad.html
// Decompiler options: packimports(3) fieldsfirst ansi space 

package com.wolfram.mexpr;

import com.wolfram.mexpr.visitors.MExprVisitor;

// Referenced classes of package com.wolfram.mexpr:
//			MExpr, MExprVector, MSymbol

public class MNormal extends MExpr
{

	MExpr head;
	MExprVector args;
	String string;

	public MNormal(MExpr mexpr, MExprVector mexprvector)
	{
		args = new MExprVector();
		string = null;
		head = mexpr;
		args = mexprvector;
		fixPositionsFromArgs();
	}

	public MNormal(MExpr mexpr, MExpr mexpr1, MExprVector mexprvector)
	{
		args = new MExprVector();
		string = null;
		head = mexpr;
		mexprvector.prepend(mexpr1);
		args = mexprvector;
		fixPositionsFromArgs();
	}

	public MNormal(MExpr mexpr)
	{
		args = new MExprVector();
		string = null;
		head = mexpr;
	}

	public MNormal(String s)
	{
		args = new MExprVector();
		string = null;
		head = new MSymbol(s);
	}

	public MNormal(String s, MExpr mexpr)
	{
		args = new MExprVector();
		string = null;
		head = new MSymbol(s);
		args.add(mexpr);
		fixPositionsFromArgs();
	}

	public MNormal(MExpr mexpr, MExpr mexpr1, MExpr mexpr2)
	{
		args = new MExprVector();
		string = null;
		head = mexpr;
		args.add(mexpr1);
		args.add(mexpr2);
		fixPositionsFromArgs();
	}

	public MNormal(MExpr mexpr, MExpr mexpr1, MExpr mexpr2, MExpr mexpr3)
	{
		args = new MExprVector();
		string = null;
		head = mexpr;
		args.add(mexpr1);
		args.add(mexpr2);
		args.add(mexpr3);
		fixPositionsFromArgs();
	}

	public MNormal(MExpr mexpr, MExpr mexpr1)
	{
		args = new MExprVector();
		string = null;
		head = mexpr;
		args.add(mexpr1);
		fixPositionsFromArgs();
	}

	void fixPositionsFromArgs()
	{
		if (args.length() > 0)
		{
			charStart = args.getElem(0).getCharStart();
			charEnd = args.getElem(args.length() - 1).getCharEnd();
		}
	}

	public MExpr getHead()
	{
		return head;
	}

	public int length()
	{
		return args.length();
	}

	public MExpr getElem(int i)
	{
		return args.getElem(i);
	}

	public void append(MExpr mexpr)
	{
		string = null;
		args.add(mexpr);
		fixPositionsFromArgs();
	}

	public void prepend(MExpr mexpr)
	{
		string = null;
		args.prepend(mexpr);
		fixPositionsFromArgs();
	}

	public String toString()
	{
		if (string != null)
			return string;
		String s = "";
		String s1 = "";
		s = head.toString();
		if (head.sameQ(MExpr.PACKAGEexpr))
			s1 = "\n";
		s = s + "[";
		for (int i = 0; i < args.length(); i++)
		{
			if (i != 0)
				s = s + ",";
			s = s + s1;
			s = s + args.getElem(i).toString();
		}

		s = s + s1 + "]";
		string = s;
		return s;
	}

	public boolean sameQ(MExpr mexpr)
	{
		if (!(mexpr instanceof MNormal))
			return false;
		MNormal mnormal = (MNormal)mexpr;
		if (length() != mnormal.length())
			return false;
		if (!getHead().sameQ(mnormal.getHead()))
			return false;
		for (int i = 0; i < length(); i++)
			if (!getElem(i).sameQ(mnormal.getElem(i)))
				return false;

		return true;
	}

	public void accept(MExprVisitor mexprvisitor)
	{
		if (!mexprvisitor.visit(this))
			return;
		head.accept(mexprvisitor);
		for (int i = 0; i < length(); i++)
			getElem(i).accept(mexprvisitor);

	}

	public MExpr head()
	{
		return head;
	}

	public MExpr part(int i)
	{
		if (i == 0)
			return head();
		if (i < 0 || i > length())
			throw new IllegalArgumentException("Cannot take part " + i + " from this Expr because it is an atom.");
		else
			return getElem(i - 1);
	}
}
