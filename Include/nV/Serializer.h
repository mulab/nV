#pragma once
#include "Common.h"
#include <vector>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

namespace nV {
class Serializer {
// 	enum Type {
// 		Null = -1,
// 		Object,
// 		Key,
// 		Symbol,
// 		Tuple,
// 		Integer,
// 		Rational,
// 		Real,
// 		String
// 	};
	struct field_head {
		//uint primary : Primary::TypeBits;
		//uint object_type : sizeof(uint) * 8 - Primary::TypeBits;
		wchar primary;
		wchar index;
		union {
			uint size;
			sint ssize;
			uint int_value;
		} value;
	};
	struct Saving {
		Saving() : field_index(0) {}
		uint field_index;
		boost::unordered_map<var, uint> var_index;//Pool;
		uint save(const var&, ostream&);
		uint save(const Object&, ostream&);
		uint save(const sym, ostream&);
		uint save(const Key&, ostream&);
		uint save(const Tuple&, ostream&);
		uint save(const __mpz_struct*, ostream&);
		uint save(const __mpq_struct*, ostream&);
		uint save(const __mpf_struct*, ostream&);
		uint save(const String&, ostream&);
	};
	struct Loading {
		std::vector<var> index_var;
		var load(istream&);
		Object* load_object(istream&, field_head&);
		sym load_symbol(istream&, field_head&);
		Key* load_key(istream&, field_head&);
		Tuple* load_tuple(istream&, field_head&);
	};
public:
	uint save(const var&, ostream&);
	var load(istream&);
} API extern serializer;
}
