/**
 * @file WebUtils.h
 * @author Thomas Saquet, Florent Poinsaut
 * @date 
 * @brief File containing example of doxygen usage for quick reference.
 *
 * Alert - GUI is a part of the Alert software
 * Copyright (C) 2013-2017
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 * 
 */

#ifndef WEBUTILS_H
#define	WEBUTILS_H

#include <algorithm>
#include <cstring>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <vector>
#include <boost/algorithm/string.hpp>

#include <Wt/WDllDefs.h>

#ifdef _MSC_VER
#include <float.h>
#endif

namespace Wt {

  class WString;
  class EscapeOStream;

  namespace Utils {

// appends the character to the string if it does not end with it
extern std::string append(const std::string& s, char c);
extern std::string prepend(const std::string& s, char c);

// in-place replace functions
extern std::string& replace(std::string& s, char c, const std::string& r);
extern std::string& replace(std::string& s, const std::string& c,
			    const std::string& r);

// lower case an UTF-8 string
extern std::string lowerCase(const std::string& s);

// sanitize unicode 
extern void sanitizeUnicode(EscapeOStream& sout, const std::string& text);

// word manipulation (for style class editing)
extern std::string eraseWord(const std::string& s, const std::string& w);
extern std::string addWord(const std::string& s, const std::string& w);

// Fast integer to string in given buffer
extern char *itoa(int value, char *result, int base = 10);
extern char *lltoa(long long value, char *result, int base = 10);

// Fast integer to string in given buffer, zero padded to length
extern char *pad_itoa(int value, int length, char *result);

// Fast (unsafe) comparison of first n characters
inline bool startsWith(const char *a, const char *b, int n) {
  return std::memcmp(a, b, n) == 0;
}

inline int length(const std::stringstream& s) {
#ifndef WT_CNOR
  return (int)s.rdbuf()->in_avail();
#else
  return 0;
#endif
}

// Finds an element in a vector. Returns the first reference to the
// element, or -1 if the element is not found.
template<typename T>
inline int indexOf(const std::vector<T>& v, const T& value)
{
  typename std::vector<T>::const_iterator i
    = std::find(v.begin(), v.end(), value);

  if (i == v.end())
    return -1;
  else
    return static_cast<int>(i - v.begin());
}

template<typename T>
inline bool erase(std::vector<T>& v, const T& value)
{
  typename std::vector<T>::iterator i = std::find(v.begin(), v.end(), value);

  if (i != v.end()) {
    v.erase(i);
    return true;
  } else
    return false;
}

template<typename K, typename V>
void eraseAndNext(std::map<K, V>& m, typename std::map<K, V>::iterator& i)
{
#ifndef WT_TARGET_JAVA
  m.erase(i++);
#endif // WT_TARGET_JAVA
}

template<typename T>
inline void insert(std::vector<T>& result, const std::vector<T>& elements)
{
#ifndef WT_CNOR
  result.insert(result.end(), elements.begin(), elements.end());
#endif // WT_CNOR
}

template<typename T>
inline void sort(std::vector<T>& result)
{
  std::sort(result.begin(), result.end());
}

template<typename T, typename Compare>
inline void sort(std::vector<T>& result, const Compare& compare)
{
  std::sort(result.begin(), result.end(), compare);
}

template<typename T>
inline void stable_sort(std::vector<T>& result)
{
  std::stable_sort(result.begin(), result.end());
}

template<typename T, typename Compare>
inline void stable_sort(std::vector<T>& result, const Compare& compare)
{
  std::stable_sort(result.begin(), result.end(), compare);
}

template <typename T, typename Compare>
inline unsigned insertion_point(const std::vector<T>& v, const T& item,
				Compare compare)
{
  return static_cast<unsigned>(
    std::lower_bound(v.begin(), v.end(), item, compare) - v.begin());
}

template <typename T>
inline unsigned lower_bound(const std::vector<T>& v, const T& item)
{
  return static_cast<unsigned>(
    std::lower_bound(v.begin(), v.end(), item) - v.begin());
}

template <typename T>
inline unsigned upper_bound(const std::vector<T>& v, const T& item)
{
  return static_cast<unsigned>(
    std::upper_bound(v.begin(), v.end(), item) - v.begin());
}

template <typename K, typename V, typename T>
inline V& access(std::map<K, V>& m, const T& key)
{
  return m[key];
}

template <typename K, typename V>
inline void insert(std::map<K, V>& m, const K& key, const V& value)
{
#ifndef WT_TARGET_JAVA
  m.insert(std::make_pair(key, value));
#endif // WT_TARGET_JAVA
}

template <typename T>
inline const T& first(const std::set<T>& s)
{
  return *s.begin();
}

template <typename T>
inline const T& last(const std::set<T>& s)
{
  return *s.rbegin();
}

// Fast round and format to string routine
extern char *round_str(double d, int digits, char *buf);

// Only for Java target
extern std::string toHexString(int i);

#ifndef WT_TARGET_JAVA
typedef boost::iterator_range<std::string::const_iterator> SplitEntry;
#else
typedef std::string SplitEntry;
#endif

typedef std::vector<SplitEntry> SplitVector;
typedef std::set<SplitEntry> SplitSet;

// Splits a string in a set of strings, on every given token
extern void split(SplitSet& tokens,
		  const std::string &in, const char *sep,
		  bool compress_adjacent_tokens);

// Replace all occurences of the 'from' char to the 'to' char in 'v'
extern void replaceAll(std::string& v, char from, char to);

extern WT_API std::string urlEncode(const std::string& url); // gkr

extern std::string dataUrlDecode(const std::string& url,
				 std::vector<unsigned char> &data);

extern std::string EncodeHttpHeaderField(const std::string &fieldname,
                                         const Wt::WString &fieldValue);

inline bool isNaN(double d) {
#ifdef _MSC_VER
  // received bug reports that on 64 bit windows, MSVC2005
  // generates wrong code for d != d.
  return _isnan(d) != 0;
#else
  return !(d == d);  
#endif
}

/*
 * These are workarounds for typ mismatches between Java and C++ port:
 * in C++ vector<string>, in Java string[], with respect to FormData
 * parameters
 */
template<typename T> inline bool isEmpty(const T& vector) {
#ifndef WT_TARGET_JAVA
  return vector.empty();
#else
  return false;
#endif // WT_TARGET_JAVA
}

template<typename T> inline int size(const T& vector) {
#ifndef WT_TARGET_JAVA
  return vector.size();
#else
  return 0;
#endif // WT_TARGET_JAVA
}

template<typename Map, typename K, typename V> 
inline void find(const Map& map, const K& key, V& result)
{
#ifndef WT_TARGET_JAVA
  std::pair<typename Map::const_iterator, typename Map::const_iterator> range 
    = map.equal_range(key);
  
  for (typename Map::const_iterator i = range.first; i != range.second; ++i)
    result.push_back(i->second);
#endif
}

#ifdef WT_TARGET_JAVA
extern void stringToDouble(const char *str, char **end, double &value);
#endif

extern std::string readFile(const std::string& fname);

extern WString formatFloat(const WString &format, double value);

  }
}

#endif	/* WEBUTILS_H */
