#pragma once
#include <boost/scoped_array.hpp>
#include <boost/logic/tribool.hpp>
#include <mU/Kernel.h>

namespace mU
{
	class LevelSpec
	{
	private:
		int min_level;
		int max_level;
		int min_depth;
		int max_depth;

		static int get_int(Var e, Var spec);
		void init(Var spec);
	public:
		LevelSpec(int level = 1);
		LevelSpec(Var spec);
	
		bool filter(size_t level, size_t depth) const;
	};

	class SequenceSpec
	{
	private:
		// TODO: precompile the sequence spec rather than during the initialization of LevelIterator
		boost::scoped_array<var> spec_;
		size_t spec_length_;
	public:
		SequenceSpec(Var spec, size_t start);

		size_t spec_length() const { return spec_length_; }

		class LevelIterator
		{
		private:
			sint min_;
			sint max_;
			sint step_;
			sint current_;
		public:
			LevelIterator(const SequenceSpec &spec, size_t dim, size_t level);

			bool has_next() const;
			size_t next();
		};

		friend class LevelIterator;
	};
}