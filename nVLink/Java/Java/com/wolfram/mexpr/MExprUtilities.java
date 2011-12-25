// Decompiled by Jad v1.5.8e2. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://kpdus.tripod.com/jad.html
// Decompiler options: packimports(3) fieldsfirst ansi space 

package com.wolfram.mexpr;


public class MExprUtilities
{

	public MExprUtilities()
	{
	}

	private static String stripQuotes(String s)
	{
		if (s.startsWith("\""))
			s = s.substring(1);
		if (s.endsWith("\""))
			s = s.substring(0, s.length() - 1);
		return s;
	}

	public static String escapeString(String s)
	{
		return escapeString(s, true);
	}

	private static String escapeString(String s, boolean flag)
	{
		StringBuffer stringbuffer = new StringBuffer();
		char ac[] = s.toCharArray();
		int i = ac.length;
		if (flag)
			stringbuffer.append('"');
		for (int j = 0; j < i; j++)
		{
			if (ac[j] == '"' || ac[j] == '\\' || ac[j] == '\n' || ac[j] == '\r' || ac[j] == '\t')
			{
				stringbuffer.append('\\');
				if (ac[j] == '\n')
				{
					stringbuffer.append('n');
					continue;
				}
				if (ac[j] == '\r')
				{
					stringbuffer.append('r');
					continue;
				}
				if (ac[j] == '\\')
				{
					stringbuffer.append('\\');
					continue;
				}
				if (ac[j] == '\t')
					stringbuffer.append('t');
				else
					stringbuffer.append(ac[j]);
				continue;
			}
			if (j == 0 && ac[j] == '!')
			{
				stringbuffer.append(' ');
				stringbuffer.append('!');
			} else
			{
				stringbuffer.append(ac[j]);
			}
		}

		if (flag)
			stringbuffer.append('"');
		return stringbuffer.toString();
	}
}
