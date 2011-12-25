#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#endif

#include <gmp.h>

#include <cassert>
#include <cctype>
#include <cwctype>
#include <cwchar>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <climits>
#include <cmath>
#include <ctime>
#include <locale>
#include <iomanip>
#include <iosfwd>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include <map>
#include <memory>
#include <deque>
#include <queue>
#include <list>
#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <typeinfo>

#ifndef BOOST_ALL_DYN_LINK
#define BOOST_ALL_DYN_LINK
#endif
#include <boost/intrusive_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <boost/thread/thread.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/scoped_array.hpp>
#include <boost/asio.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/array.hpp>