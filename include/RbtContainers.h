/***********************************************************************
 * The rDock program was developed from 1998 - 2006 by the software team
 * at RiboTargets (subsequently Vernalis (R&D) Ltd).
 * In 2006, the software was licensed to the University of York for
 * maintenance and distribution.
 * In 2012, Vernalis and the University of York agreed to release the
 * program as Open Source software.
 * This version is licensed under GNU-LGPL version 3.0 with support from
 * the University of Barcelona.
 * http://rdock.sourceforge.net/
 ***********************************************************************/

// Miscellaneous container typedefs etc for Rbt framework

#ifndef _RBTCONTAINERS_H_
#define _RBTCONTAINERS_H_

#include <vector>
using std::vector;

#include <map>
using std::make_pair;
using std::map;
using std::multimap;
using std::pair;

#include <list>
using std::list;

#include <set>
using std::set;

#include <algorithm>
// DM 9 Dec 1998 - add copy_if template to std namespace (not included in STL)
#if (defined(__clang__) && (__clang_major__ < 3 ||                             \
                            (__clang_major__ == 3 && __clang_minor__ < 3))) || \
    (!defined(__clang__) && defined(__GNUC__) &&                               \
     (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 8)))
namespace std {
template <class InputIterator, class OutputIterator, class Predicate>
OutputIterator copy_if(InputIterator first, InputIterator last,
                       OutputIterator result, Predicate pred) {
  for (; first != last; ++first)
    if (pred(*first)) {
      *result = *first;
      ++result;
    }
  return result;
}
} // namespace std
#endif //(defined(__clang__) ...) || (!defined(__clang__) && defined(__GNUC__)
       //...)

// Container Typedefs

// double
typedef vector<double> RbtDoubleList;
typedef RbtDoubleList::iterator RbtDoubleListIter;
typedef RbtDoubleList::const_iterator RbtDoubleListConstIter;

// int
typedef vector<int> RbtIntList;
typedef RbtIntList::iterator RbtIntListIter;
typedef RbtIntList::const_iterator RbtIntListConstIter;

// uint
typedef vector<unsigned int> RbtUIntList;
typedef RbtUIntList::iterator RbtUIntListIter;
typedef RbtUIntList::const_iterator RbtUIntListConstIter;

// string
typedef vector<std::string> RbtStringList;
typedef RbtStringList::iterator RbtStringListIter;
typedef RbtStringList::const_iterator RbtStringListConstIter;

// bool
typedef vector<bool> RbtBoolVec; // vector of bools
typedef RbtBoolVec::iterator RbtBoolVecIter;
typedef RbtBoolVec::const_iterator RbtBoolVecConstIter;

// Segment is a named part of an RbtModel (usually an intact molecule)
// For now, a segment is defined as just an RbtString
// RbtSegmentMap holds a map of (key=unique segment name, value=number of atoms
// in segment)
typedef std::string RbtSegment;
typedef map<RbtSegment, unsigned int> RbtSegmentMap;
typedef RbtSegmentMap::iterator RbtSegmentMapIter;
typedef RbtSegmentMap::const_iterator RbtSegmentMapConstIter;

// Map of (key=string, value=integer)
typedef map<std::string, int> RbtStringIntMap;
typedef RbtStringIntMap::iterator RbtStringIntMapIter;
typedef RbtStringIntMap::const_iterator RbtStringIntMapConstIter;

// Map of (key=string, value=double)
typedef map<std::string, double> RbtStringDoubleMap;
typedef RbtStringDoubleMap::iterator RbtStringDoubleMapIter;
typedef RbtStringDoubleMap::const_iterator RbtStringDoubleMapConstIter;

// Added by DM, 3 Dec 1998
// Map of (key=string, value=bool)
typedef map<std::string, bool> RbtStringBoolMap;
typedef RbtStringBoolMap::iterator RbtStringBoolMapIter;
typedef RbtStringBoolMap::const_iterator RbtStringBoolMapConstIter;

// Map of (key=int, value=string)
typedef map<int, std::string> RbtIntStringMap;
typedef RbtIntStringMap::iterator RbtIntStringMapIter;
typedef RbtIntStringMap::const_iterator RbtIntStringMapConstIter;

// Set of (key=uint)
typedef set<unsigned int> RbtUIntSet;
typedef RbtUIntSet::iterator RbtUIntSetIter;
typedef RbtUIntSet::const_iterator RbtUIntSetConstIter;

#endif //_RBTCONTAINERS_H_
