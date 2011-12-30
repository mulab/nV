#pragma once
#include "var.h"
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>

namespace nV {
struct Enum {
    API static bool Inited;
    API static void Init();

    sym PrimaryName[Primary::TypeSize + 1];
    sym
    Null, Object, Key, Symbol, Tuple;

    boost::unordered_set<sym> Attributes;
    sym
    Constant,
    Flat,
    HoldAll,
    HoldAllComplete,
    HoldFirst,
    HoldRest,
    Listable,
    Locked,
    NHoldAll,
    NHoldFirst,
    NHoldRest,
    NumericFunction,
    OneIdentity,
    Orderless,
    Protected,
    ReadProtected,
    SequenceHold,
    Stub,
    Temporary;

    boost::unordered_set<sym> Patterns;
    sym
    Alternatives,
    Blank,
    Condition,
    Optional,
    Pattern,
    PatternTest,
    Production,
    Rule,
    RuleDelayed;

    boost::unordered_set<sym> Objects;
	boost::unordered_set<sym> Numbers;
    sym
    Integer, Rational, Real, String, Array, Complex, RealI;

    sym
    Assign,
    Break,
    Continue,
    Delayed,
    Fail,
    False,
    Function,
    Infinity,
    Interface,
    List,
    Map,
    Match,
    Method,
    Minus,
	NaN,
    Plus,
    Power,
	Print,
    Radical,
    Return,
    Self,
    Sequence,
    Serial,
    Set,
    Slot,
    Sqrt,
    Times,
    True,
    Value;
} API extern $;
}
