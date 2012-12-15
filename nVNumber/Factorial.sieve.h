#pragma once
#include <vector>
#include <algorithm>
#include <boost/cstdint.hpp>

class sieve {
    std::vector<boost::uint32_t> v;
    void init(boost::uint32_t);
public:
    sieve(boost::uint32_t n) {
        init(n);
    }
    const boost::uint32_t* begin() const {
        return v.begin().operator ->();
    }
    const boost::uint32_t* end() const {
        return v.end().operator ->();
    }
    boost::uint32_t lower(boost::uint32_t c) const {
        return std::max<int>(std::upper_bound(v.begin(), v.end(), c) - v.begin() - 1, 0);
    }
    boost::uint32_t upper(boost::uint32_t c) const {
        return std::min<int>(std::lower_bound(v.begin(), v.end(), c) - v.begin(), v.size() - 1);
    }
    boost::uint32_t floor(boost::uint32_t c) const {
        return v[lower(c)];
    }
    boost::uint32_t ceiling(boost::uint32_t c) const {
        return v[upper(c)];
    }
    boost::uint32_t find(boost::uint32_t c) const {
        boost::uint32_t r = lower(c);
        return v[r] == c ? r : 0;
    }
    const boost::uint32_t& operator [](boost::uint32_t i) const {
        return v[i];
    }
    boost::uint32_t count(boost::uint32_t low, boost::uint32_t high) const {
        if (low > high) std::swap(low, high);
        boost::uint32_t r = upper(low), s = lower(high);
        return s + 1 - r + (v[r] == low) - (v[s] == high);
    }
    boost::uint32_t size() const {
        return v.size();
    }
    void reset(boost::uint32_t n) {
        init(n);
    }
};
