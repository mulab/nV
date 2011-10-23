// Decompiled by Jad v1.5.8e2. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://kpdus.tripod.com/jad.html
// Decompiler options: packimports(3) fieldsfirst ansi space 

package com.wolfram.mexpr.visitors;

import com.wolfram.mexpr.MExpr;
import com.wolfram.mexpr.MNormal;

public interface MExprFilter
{

	public abstract void startNode(MExpr mexpr);

	public abstract MExpr endNode(MNormal mnormal);
}
