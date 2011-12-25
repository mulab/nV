// Decompiled by Jad v1.5.8e2. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://kpdus.tripod.com/jad.html
// Decompiler options: packimports(3) fieldsfirst ansi space 

package com.wolfram.mexpr;

import com.wolfram.mexpr.visitors.MExprFilter;

// Referenced classes of package com.wolfram.mexpr:
//			MNormal, MString, MSymbol, MInteger, 
//			MExprHeadVector, MExprVector, MExpr, ArgumentList, 
//			IMExprToken

public class MExprFactory
{

	MExprFilter filter;

	public MExprFactory()
	{
		filter = null;
	}

	public void setFilter(MExprFilter mexprfilter)
	{
		filter = mexprfilter;
	}

	public MExpr UnwindHead(MExpr mexpr, MExprHeadVector mexprheadvector)
	{
		if (mexprheadvector == null)
			return mexpr;
		int i = mexprheadvector.length();
		Object obj = mexpr;
		for (int j = 0; j < i; j++)
		{
			MExprVector mexprvector = mexprheadvector.getElem(j);
			if (mexprvector.isPart)
				obj = new MNormal(MExpr.PARTexpr, ((MExpr) (obj)), mexprvector);
			else
				obj = new MNormal(((MExpr) (obj)), mexprvector);
			((MExpr)obj).charStart = mexpr.charStart;  
			//edited by XG
			//former code is "obj.charStart = mexpr.charStart;"
			((MExpr)obj).charEnd = mexprvector.charEnd;
			//edited by XG
			//former code is "obj.charEnd = mexprvector.charEnd;"
		}

		if (filter == null)
			return ((MExpr) (obj));
		else
			return filter.endNode((MNormal)obj);
	}

	public MExpr appendExpr(MExpr mexpr, MExpr mexpr1, MExpr mexpr2)
	{
		if (mexpr1 instanceof MNormal)
		{
			MNormal mnormal = (MNormal)mexpr1;
			if (mexpr.equals(mnormal.getHead()))
			{
				mnormal.append(mexpr2);
				return mexpr1;
			}
		} else
		if (mexpr2 instanceof MNormal)
		{
			MNormal mnormal1 = (MNormal)mexpr2;
			if (mexpr.equals(mnormal1.getHead()))
			{
				mnormal1.prepend(mexpr1);
				return mexpr2;
			}
		}
		return new MNormal(mexpr, mexpr1, mexpr2);
	}

	public MExpr joinExpr(MExpr mexpr, MExpr mexpr1, MExpr mexpr2)
	{
		MExpr mexpr3 = mexpr1.head();
		MExpr mexpr4 = mexpr2.head();
		if (!mexpr.sameQ(mexpr3))
			mexpr1 = new MNormal(mexpr, mexpr1);
		if (!mexpr.sameQ(mexpr4))
			mexpr2 = new MNormal(mexpr, mexpr2);
		MExpr mexpr5 = mexpr1;
		for (int i = 1; i <= mexpr2.length(); i++)
			mexpr5 = appendExpr(mexpr, mexpr5, mexpr2.part(i));

		return mexpr5;
	}

	public MExpr appendInequality(MExpr mexpr, MExpr mexpr1, MExpr mexpr2)
	{
		if (mexpr1 instanceof MNormal)
		{
			MNormal mnormal = (MNormal)mexpr1;
			MExpr mexpr3 = mnormal.getHead();
			if (mexpr3.equals(MExpr.INEQUALITYexpr))
			{
				mnormal.append(mexpr);
				mnormal.append(mexpr2);
				return mnormal;
			}
			if (mexpr3.equals(MExpr.EQUALexpr) || mexpr3.equals(MExpr.UNEQUALexpr) || mexpr3.equals(MExpr.GREATERexpr) || mexpr3.equals(MExpr.LESSexpr) || mexpr3.equals(MExpr.GREATEREQUALexpr) || mexpr3.equals(MExpr.LESSEQUALexpr))
			{
				if (mexpr.equals(mexpr3))
				{
					mnormal.append(mexpr2);
					return mnormal;
				}
				MNormal mnormal1 = new MNormal(MExpr.INEQUALITYexpr);
				mnormal1.append(mnormal.getElem(0));
				for (int i = 1; i < mnormal.length(); i++)
				{
					mnormal1.append(mexpr3);
					mnormal1.append(mnormal.getElem(i));
				}

				mnormal1.append(mexpr);
				mnormal1.append(mexpr2);
				return mnormal1;
			}
		}
		return new MNormal(mexpr, mexpr1, mexpr2);
	}

	public MExpr BuildExpr(ArgumentList argumentlist, String s)
	{
		if (argumentlist.length() == 1)
			return argumentlist.getElem(0);
		else
			return null;
	}

	MExpr blankHead(int i)
	{
		MExpr mexpr;
		if (i == 1)
			mexpr = MExpr.BLANKexpr;
		else
		if (i == 2)
			mexpr = MExpr.BLANKSEQUENCEexpr;
		else
			mexpr = MExpr.BLANKNULLSEQUENCEexpr;
		return mexpr;
	}

	public MNormal makeBlank(int i, IMExprToken imexprtoken)
	{
		MExpr mexpr = blankHead(i);
		MNormal mnormal = new MNormal(mexpr);
		mnormal.setCharPositions(imexprtoken);
		return mnormal;
	}

	public MExpr makeBlankId(MExpr mexpr, int i, IMExprToken imexprtoken)
	{
		MNormal mnormal = makeBlank(i, imexprtoken);
		mnormal.append(mexpr);
		if (imexprtoken != null)
			mnormal.setCharStart(imexprtoken);
		mnormal.setCharEnd(mexpr.getCharEnd());
		return mnormal;
	}

	public MExpr makeStringChars(IMExprToken imexprtoken)
	{
		String s = fixStringChars(imexprtoken.getText());
		MString mstring = new MString(s, imexprtoken);
		mstring.setCharPositions(imexprtoken);
		return mstring;
	}

	public String fixStringChars(String s)
	{
		int i = 0;
		int j = s.length();
		if (s.startsWith("\""))
			i++;
		if (s.endsWith("\""))
			j--;
		char ac[] = s.toCharArray();
		StringBuffer stringbuffer = new StringBuffer();
		boolean flag = false;
		for (int k = i; k < j; k++)
		{
			if (flag)
			{
				switch (ac[k])
				{
				case 110: // 'n'
					stringbuffer.append('\n');
					break;

				case 114: // 'r'
					stringbuffer.append('\r');
					break;

				case 116: // 't'
					stringbuffer.append('\t');
					break;

				case 33: // '!'
				case 40: // '('
				case 41: // ')'
				case 42: // '*'
				case 91: // '['
					stringbuffer.append('\\');
					stringbuffer.append(ac[k]);
					break;

				default:
					stringbuffer.append(ac[k]);
					break;
				}
				flag = false;
				continue;
			}
			if (ac[k] == '\\')
			{
				flag = true;
			} else
			{
				stringbuffer.append(ac[k]);
				flag = false;
			}
		}

		return stringbuffer.toString();
	}

	public MExpr OptionalPattern(IMExprToken imexprtoken, MExpr mexpr)
	{
		MSymbol msymbol = new MSymbol(imexprtoken);
		if (!(mexpr instanceof MNormal))
			return makePattern(msymbol, mexpr);
		MNormal mnormal = (MNormal)mexpr;
		if (mnormal.length() != 2)
			return makePattern(msymbol, mexpr);
		MExpr mexpr1 = mnormal.getElem(0);
		MExpr mexpr2 = mnormal.getElem(1);
		MExpr mexpr3 = mnormal.getHead();
		if (mexpr3.sameQ(MExpr.PATTERNexpr))
		{
			if (!(mexpr2 instanceof MNormal))
				return makeOptionalPattern(msymbol, mexpr1, mexpr2);
			MExpr mexpr4 = ((MNormal)mexpr2).getHead();
			if (!mexpr4.sameQ(MExpr.BLANKexpr) && !mexpr4.sameQ(MExpr.BLANKSEQUENCEexpr) && !mexpr4.sameQ(MExpr.BLANKNULLSEQUENCEexpr))
				return makeOptionalPattern(msymbol, mexpr1, mexpr2);
		} else
		if (mexpr3.sameQ(MExpr.OPTIONALexpr))
		{
			if (!(mexpr1 instanceof MNormal))
				return makeOptionalPattern(msymbol, mexpr1, mexpr2);
			MExpr mexpr5 = ((MNormal)mexpr1).getHead();
			if (!mexpr5.sameQ(MExpr.PATTERNexpr))
				return makeOptionalPattern(msymbol, mexpr1, mexpr2);
		}
		return makePattern(msymbol, mexpr);
	}

	MExpr makeOptionalPattern(MExpr mexpr, MExpr mexpr1, MExpr mexpr2)
	{
		MNormal mnormal = new MNormal(MExpr.PATTERNexpr, mexpr, mexpr1);
		return new MNormal(MExpr.OPTIONALexpr, mnormal, mexpr2);
	}

	MExpr makePattern(MExpr mexpr, MExpr mexpr1)
	{
		return new MNormal(MExpr.PATTERNexpr, mexpr, mexpr1);
	}

	public MExpr getRule(String s)
	{
		if (s.charAt(0) == ':')
			return MExpr.RULEDELAYEDexpr;
		else
			return MExpr.RULEexpr;
	}

	public MExpr getSet(String s)
	{
		if (s.charAt(0) == ':')
			return MExpr.SETDELAYEDexpr;
		else
			return MExpr.SETexpr;
	}

	public MExpr makeSlot(IMExprToken imexprtoken)
	{
		String s = imexprtoken.getText();
		MNormal mnormal = null;
		if ("#".equals(s))
			mnormal = new MNormal(MExpr.SLOTexpr, new MInteger("1"));
		else
		if ("##".equals(s))
			mnormal = new MNormal(MExpr.SLOTSEQUENCEexpr, new MInteger("1"));
		else
		if (s.startsWith("##"))
			mnormal = new MNormal(MExpr.SLOTSEQUENCEexpr, new MInteger(s.substring(2)));
		else
		if (s.startsWith("#"))
			mnormal = new MNormal(MExpr.SLOTexpr, new MInteger(s.substring(1)));
		mnormal.setCharPositions(imexprtoken);
		return mnormal;
	}

	public void registerhead(MExpr mexpr)
	{
		if (filter == null)
		{
			return;
		} else
		{
			filter.startNode(mexpr);
			return;
		}
	}

	public MExpr makeOutNumber(IMExprToken imexprtoken)
	{
		MInteger minteger = new MInteger(imexprtoken.getText().substring(1));
		MNormal mnormal = new MNormal(MExpr.OUTexpr, minteger);
		mnormal.setCharPositions(imexprtoken);
		return mnormal;
	}
}
