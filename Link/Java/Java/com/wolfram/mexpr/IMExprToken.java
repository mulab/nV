// Decompiled by Jad v1.5.8e2. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://kpdus.tripod.com/jad.html
// Decompiler options: packimports(3) fieldsfirst ansi space 

package com.wolfram.mexpr;


public interface IMExprToken
{

	public abstract int getCharStart();

	public abstract int getCharEnd();

	public abstract int getTokenIndex();

	public abstract String getText();
}
