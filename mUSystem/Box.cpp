#include "StdAfx.h"
#include <mU/Number.h>
#include <mU/Parser.h>
#include <mU/System.h>

namespace mU {
//////////////////////////////////////
void BoxPrint(Var x, wostream &f, size_t y) {
    switch (Type(x)) {
    case TYPE(obj):
    case TYPE(int):
    case TYPE(rat):
    case TYPE(flt):
    case TYPE(str): {
        f << L'\"';
        Print(x, f);
        f << L'\"';
    }
    break;
    case TYPE(sym): {
        f << L'\"';
        if (x == TAG(Infinity)) f << L"\\[Infinity]";
        else if (x == TAG(Pi)) f << L"\\[Pi]";
        else if (x == TAG(E)) f << L"\\[ExponentialE]";
        else if (x == TAG(I)) f << L"\\[ImaginaryI]";
        else Print(x, f);
        f << L'\"';
    }
    break;
    case TYPE(vec): {
        //RowBox[{"{", %(0), "}"}]
        vec_t::rep_t &rep = CVec(x);
        f << L"RowBox[{\"{\"";
        f << L',';
        size_t n = rep.size();
        if (n > 0) {
            BoxPrint(rep[0], f);
            for (size_t i = 1; i < n; ++i) {
                f << L',';
                f << L"\",\"";
                f << L',';
                BoxPrint(rep[i], f);
            }
        }
        f << L',';
        f << "\"}\"}]";
    }
    break;
    case TYPE(ex): {
        var h = Head(x);
        var b = Body(x);
        size_t n = Size(b);
        if (SymQ(h)) {
            if (h == TAG(Graphics2D) || h == TAG(Graphics3D)) {
                f << L"GraphicsBox[";
                f << L'\"';
                Print(x, f);
                f << L'\"';
                f << L']';
                return;
            }
            Var s = h;
            stdext::hash_map<Var, size_t>::const_iterator iter;
            if (n == 0) {
                if (h == TAG(Blank)) {
                    f << L'\"';
                    f << L'_';
                    f << L'\"';
                    return;
                } else if (h == TAG(BlankSequence)) {
                    f << L'\"';
                    f << L"__";
                    f << L'\"';
                    return;
                }
            } else if (n == 1) {
                if (h == TAG(Slot)) {
                    f << L"RowBox[{";
                    f << L'\"' << L'#' << L'\"';
                    f << L',';
                    BoxPrint(At(b, 0), f);
                    f << L"}]";
                    return;
                } else if (h == TAG(SlotSequence)) {
                    f << L"RowBox[{";
                    f << L'\"' << L"##" << L'\"';
                    f << L',';
                    BoxPrint(At(b, 0), f);
                    f << L"}]";
                    return;
                } else if (h == TAG(Blank)) {
                    f << L"RowBox[{";
                    f << L'\"' << L'_' << L'\"';
                    f << L',';
                    BoxPrint(At(b, 0), f);
                    f << L"}]";
                    return;
                } else if (h == TAG(BlankSequence)) {
                    f << L"RowBox[{";
                    f << L'\"' << L"__" << L'\"';
                    f << L',';
                    BoxPrint(At(b, 0), f);
                    f << L"}]";
                    return;
                } else if (h == TAG(Optional)) {
                    f << L"RowBox[{";
                    BoxPrint(At(b, 0), f);
                    f << L',';
                    f << L'\"' << L'.' << L'\"';
                    f << L"}]";
                    return;
                } else if (h == TAG(Sqrt)) {
                    f << L"SqrtBox[";
                    BoxPrint(At(b, 0), f);
                    f << L']';
                    return;
                }
				const parser::oper_t *op_ptr = parser::lookup_oper(s);
				if (op_ptr && op_ptr->postfix) {
                    const parser::oper_t &op = *op_ptr;
                    f << L"RowBox[{";
                    if (op.prec < y) {
                        f << L"\"(\",";
                        BoxPrint(At(b, 0), f, op.prec);
                        f << L',';
                        f << '\"' << op.show << '\"';
                        f << L',';
                        f << "\")\"";
                    } else {
                        BoxPrint(At(b, 0), f, op.prec);
                        f << L',';
                        f << '\"' << op.show << '\"';
                    }
                    f << L"}]";
                    return;
                }

				if (op_ptr && op_ptr->prefix) {
					const parser::oper_t &op = *op_ptr;
                    f << L"RowBox[{";
                    if (op.prec < y) {
                        f << L"\"(\",";
                        f << '\"' << op.show << '\"';
                        f << L',';
                        BoxPrint(At(b, 0), f, h == TAG(Minus) ? y : op.prec);
                        f << L',';
                        f << "\")\"";
                    } else {
                        f << '\"' << op.show << '\"';
                        f << L',';
                        BoxPrint(At(b, 0), f, h == TAG(Minus) ? y : op.prec);
                    }
                    f << L"}]";
                    return;
                }
            } else if (h == TAG(Part)) {
                //SubscriptBox[%(0), RowBox[{"\[LeftDoubleBracket]", %(1), "\[RightDoubleBracket]"}]]
                f << L"SubscriptBox[";
                BoxPrint(At(b, 0), f);
                f << L',';
                f << L"RowBox[{";
                f << L"\"\\[LeftDoubleBracket]\"";
                f << L',';
                BoxPrint(At(b, 1), f);
                f << L',';
                f << L"\"\\[RightDoubleBracket]\"";
                f << L"}]";
                f << L']';
                return;
            } else if (n == 2) {
                if (h == TAG(Pattern)) {
                    f << L"RowBox[{";
                    BoxPrint(At(b, 0), f);
                    if (!ExQ(At(b, 1)) ||
                            (Head(At(b, 1)) != TAG(Blank)
                             && Head(At(b, 1)) != TAG(BlankSequence))) {
                        f << L',';
                        f << L'\"' << L':' << L'\"';
                    }
                    f << L',';
                    BoxPrint(At(b, 1), f);
                    f << L"}]";
                    return;
                } else if (h == TAG(Property)) {
                    f << L"RowBox[{";
                    BoxPrint(At(b, 0), f);
                    f << L',';
                    f << L'\"' << L"::" << L'\"';
                    f << L',';
                    BoxPrint(At(b, 1), f);
                    f << L"}]";
                    return;
                }
                if (h == TAG(D)) {
                    //RowBox[{SubscriptBox["\[PartialD]", %(1)], " ", %(0)}]
                    f << L"RowBox[{SubscriptBox[\"\\[PartialD]\"",
                    f << L',';
                    BoxPrint(At(b, 1), f);
                    f << L']';
                    f << L',';
                    BoxPrint(At(b, 0), f);
                    f << L"}]";
                    return;
                } else if (h == TAG(Divide)) {
                    //FractionBox[%(0), %(1)]
                    f << L"FractionBox[";
                    BoxPrint(At(b, 0), f);
                    f << L',';
                    BoxPrint(At(b, 1), f);
                    f << L"]";
                    return;
                }
#define T(x,y)\
else if(h == tag_##x)\
{\
	f << L"RowBox[{";\
	BoxPrint(At(b,0),f);\
	f << L',';\
	f << L"\"\\[" L###y L"]\"";\
	f << L',';\
	BoxPrint(At(b,1),f);\
	f << L"}]";\
	return;\
}
                T(Equal, Equal)T(Unequal, NotEqual)T(GreaterEqual, GreaterSlantEqual)T(LessEqual, LessSlantEqual)
                else if (h == TAG(Integrate)) {
                    if (VecQ(At(b, 1))) {
                        //RowBox[{SubsuperscriptBox["\[Integral]", %(2), %(3)], RowBox[{%(0), RowBox[{"\[DifferentialD]", %(1)}]}]}]
                        f << L"RowBox[{";
                        f << L"SubsuperscriptBox[";
                        f << L"\"\\[Integral]\"";
                        f << L',';
                        BoxPrint(At(At(b, 1), 1), f);
                        f << L',';
                        BoxPrint(At(At(b, 1), 2), f);
                        f << L']';
                        f << L',';
                        f << L"RowBox[{";
                        BoxPrint(At(b, 0), f);
                        f << L',';
                        f << L"RowBox[{";
                        f << L"\"\\[DifferentialD]\"";
                        f << L',';
                        BoxPrint(At(At(b, 1), 0), f);
                        f << L"}]";
                        f << L"}]";
                        f << L"}]";
                        return;
                    } else {
                        //RowBox[{"\[Integral]", RowBox[{%(0), RowBox[{"\[DifferentialD]", %(1)}]}]}]
                        f << L"RowBox[{";
                        f << L"\"\\[Integral]\"";
                        f << L',';
                        f << L"RowBox[{";
                        BoxPrint(At(b, 0), f);
                        f << L',';
                        f << L"RowBox[{";
                        f << L"\"\\[DifferentialD]\"";
                        f << L',';
                        BoxPrint(At(b, 1), f);
                        f << L"}]";
                        f << L"}]";
                        f << L"}]";
                        return;
                    }
                } else if (h == TAG(Sum)) {
                    if (VecQ(At(b, 1))) {
                        //RowBox[{UnderoverscriptBox["\[Sum]", RowBox[{%(1), "=", %(2)}], %(3)], %(0)}]
                        f << L"RowBox[{";
                        f << L"UnderoverscriptBox[";
                        f << L"\"\\[Sum]\"";
                        f << L',';
                        f << L"RowBox[{";
                        BoxPrint(At(At(b, 1), 0), f);
                        f << L',';
                        f << L"\"=\"";
                        f << L',';
                        BoxPrint(At(At(b, 1), 1), f);
                        f << L"}]";
                        f << L',';
                        BoxPrint(At(At(b, 1), 2), f);
                        f << L']';
                        f << L',';
                        BoxPrint(At(b, 0), f);
                        f << L"}]";
                        return;
                    } else {
                        //RowBox[{UnderoverscriptBox["\[Sum]", %(1)], %(0)}]
                        f << L"RowBox[{";
                        f << L"UnderoverscriptBox[";
                        f << L"\"\\[Sum]\"";
                        f << L',';
                        BoxPrint(At(b, 1), f);
                        f << L']';
                        f << L',';
                        BoxPrint(At(b, 0), f);
                        f << L"}]";
                        return;
                    }
                } else if (h == TAG(Radical)) {
                    f << L"RadicalBox[";
                    BoxPrint(At(b, 0), f);
                    f << L',';
                    BoxPrint(At(b, 1), f);
                    f << L']';
                    return;
                }
				const parser::oper_t *op_ptr = parser::lookup_oper(s);
				if (op_ptr && !op_ptr->postfix && !op_ptr->prefix) {
                    const parser::oper_t &op = *op_ptr;
                    if (h == TAG(Power)) {
                        //SuperscriptBox[%(0), %(1)]
                        f << L"SuperscriptBox[";
                        BoxPrint(At(b, 0), f, op.prec);
                        f << L',';
                        BoxPrint(At(b, 1), f);
                        f << L']';
                        return;
                    }
                    f << L"RowBox[{";
                    if (op.prec < y) {
                        f << L"\"(\"";
                        f << ',';
                    }
                    BoxPrint(At(b, 0), f, op.rassoc ? op.prec : op.prec + 1);
                    if (h == TAG(Plus)) {
                        f << L',';
                        if (!ExQ(At(b, 1), TAG(Minus))) {
                            f << '\"' << L'+' << '\"';
                            f << L',';
                        }
                    } else if (h == TAG(Times)) {
                        f << L',';
                        if ((Type(At(b, 0)) == TYPE(sym) || Type(At(b, 0)) == TYPE(ex))
                                && (Type(At(b, 1)) == TYPE(sym) || Type(At(b, 1)) == TYPE(ex))) {
                            f << '\"' << L' ' << '\"';
                            f << L',';
                        }
                    } else {
                        f << L',';
                        f << '\"' << op.show << '\"';
                        f << L',';
                    }
                    BoxPrint(At(b, 1), f, op.rassoc ? op.prec + 1 : op.prec);
                    if (op.prec < y) {
                        f << ',';
                        f << L"\")\"";
                    }
                    f << L"}]";
                    return;
                }
            } else {
				const parser::oper_t *op_ptr = parser::lookup_oper(s);
				if (op_ptr && !op_ptr->postfix && !op_ptr->prefix) {
                    const parser::oper_t &op = *op_ptr;
                    f << L"RowBox[{";
                    if (op.prec < y) {
                        f << L"\"(\"";
                        f << L',';
                    }
                    BoxPrint(At(b, 0), f, op.prec);
                    if (h == TAG(Plus)) {
                        for (size_t i = 1; i < n; ++i) {
                            f << L',';
                            if (!ExQ(At(b, i), TAG(Minus))) {
                                f << '\"' << L'+' << '\"';
                                f << L',';
                            }
                            BoxPrint(At(b, i), f, op.prec);
                        }
                    } else if (h == TAG(Times)) {
                        for (size_t i = 1; i < n; ++i) {
                            f << L',';
                            if ((Type(At(b, i - 1)) == TYPE(sym) || Type(At(b, i - 1)) == TYPE(ex))
                                    && (Type(At(b, i)) == TYPE(sym) || Type(At(b, i)) == TYPE(ex))) {
                                f << '\"' << L' ' << '\"';
                                f << L',';
                            }
                            BoxPrint(At(b, i), f, op.prec);
                        }
                    } else {
                        for (size_t i = 1; i < n; ++i) {
                            f << L',';
                            f << '\"' << op.show << '\"';
                            f << L',';
                            BoxPrint(At(b, i), f, op.prec);
                        }
                    }
                    if (op.prec < y) {
                        f << L',';
                        f << "\")\"";
                    }
                    f << L"}]";
                    return;
                }
            }
        } else if (ExQ(h, TAG(Derivative)) && Size(Body(h)) == 1) {
            //SuperscriptBox[%(0), "\[Prime]"]
            var e = At(Body(h), 0);
            f << L"SuperscriptBox[";
            BoxPrint(At(b, 0), f,
                     parser::lookup_oper(TAG(Differential))->prec);
            f << L',';
            if (IntQ(e)) {
                int se = Z::SI(e);
                if (se == 1) {
                    f << L"\"\\[Prime]\"";
                    f << L']';
                    return;
                } else if (se == 2) {
                    f << L"\"\\[Prime]\\[Prime]\"";
                    f << L']';
                    return;
                }
            }
            f << L"RowBox[{";
            f << L"\"(\"";
            f << L',';
            BoxPrint(e, f);
            f << L',';
            f << L"\")\"";
            f << L"}]";
            f << L']';
            return;
        }
        f << L"RowBox[{";
        BoxPrint(h, f, parser::max_prec());
        f << L',';
        f << L"\"[\"";
        f << L',';
        if (n > 0) {
            BoxPrint(At(b, 0), f);
            for (size_t i = 1; i < n; ++i) {
                f << L',';
                f << L"\",\"";
                f << L',';
                BoxPrint(At(b, i), f);
            }
        }
        f << L',';
        f << L"\"]\"";
        f << L"}]";
    }
    break;
    }
}
//////////////////////////////////////
}

using namespace mU;

CAPI CPROC(System_Box) {
	wostringstream t;
	BoxPrint(Pretty(At(x,0)),t);
	return Str(t.str());
}