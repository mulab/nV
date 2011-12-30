// Decompiled by Jad v1.5.8e2. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://kpdus.tripod.com/jad.html
// Decompiler options: packimports(3) fieldsfirst ansi space 

package com.wolfram.mexpr.visitors;

import com.wolfram.mexpr.*;

// Referenced classes of package com.wolfram.mexpr.visitors:
//			MExprVisitor

public class ToStringBufferVisitor
	implements MExprVisitor
{

	StringBuffer buffer;
	int depth;
	int spaceNum;
	boolean addspace;

	public ToStringBufferVisitor()
	{
		spaceNum = 4;
		addspace = true;
		buffer = new StringBuffer();
		depth = 0;
	}

	public StringBuffer getStringBuffer()
	{
		return buffer;
	}

	public boolean visit(MNormal mnormal)
	{
		mnormal.head().accept(this);
		buffer.append('[');
		if (mnormal.length() == 0)
			buffer.append(' ');
		else
		if (mnormal.length() == 1 && !(mnormal.part(1) instanceof MNormal))
		{
			boolean flag = addspace;
			addspace = false;
			mnormal.part(1).accept(this);
			addspace = flag;
		} else
		{
			buffer.append('\n');
			depth++;
			for (int i = 1; i <= mnormal.length(); i++)
			{
				mnormal.part(i).accept(this);
				if (i != mnormal.length())
					buffer.append(',');
				buffer.append('\n');
			}

			depth--;
			addSpaces();
		}
		buffer.append(']');
		return false;
	}

	void addSpaces()
	{
		if (!addspace)
			return;
		for (int i = 0; i < depth * spaceNum; i++)
			buffer.append(' ');

	}

	public void visit(MInteger minteger)
	{
		addSpaces();
		buffer.append(minteger.toString());
	}

	public void visit(MReal mreal)
	{
		addSpaces();
		buffer.append(mreal.toString());
	}

	public void visit(MString mstring)
	{
		addSpaces();
		buffer.append(mstring.toString());
	}

	public void visit(MSymbol msymbol)
	{
		addSpaces();
		buffer.append(msymbol.toString());
	}

	public void visit(MTypeset mtypeset)
	{
		addSpaces();
		buffer.append(mtypeset.toString());
	}
}
