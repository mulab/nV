// Decompiled by Jad v1.5.8e2. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://kpdus.tripod.com/jad.html
// Decompiler options: packimports(3) fieldsfirst ansi space 

package com.wolfram.mexpr.visitors;

import com.wolfram.mexpr.*;

public interface MExprVisitor
{

	public abstract boolean visit(MNormal mnormal);

	public abstract void visit(MInteger minteger);

	public abstract void visit(MString mstring);

	public abstract void visit(MReal mreal);

	public abstract void visit(MSymbol msymbol);

	public abstract void visit(MTypeset mtypeset);
}
