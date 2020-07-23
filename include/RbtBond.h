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

// Handles all the properties of a bond

#ifndef _RBTBOND_H_
#define _RBTBOND_H_

#include "RbtAtom.h"
#include "RbtConfig.h"

namespace rxdock {

class RbtBond {
public:
  ////////////////////////////////////////
  // Constructors/destructors
  ////////////////////////////////////////

  // Default constructor
  RbtBond();

  // Cconstructor supplying all parameters
  RbtBond(int nBondId, RbtAtomPtr &spAtom1, RbtAtomPtr &spAtom2,
          int nFormalBondOrder = 1);

  virtual ~RbtBond(); // Default destructor

  RbtBond(const RbtBond &bond); // Copy constructor

  RbtBond &operator=(const RbtBond &bond); // Copy assignment

  ///////////////////////////////////////////////
  // Stream functions
  ///////////////////////////////////////////////

  // Insertion operator (primarily for debugging)
  // Note: needs to be friend so can access bond private data
  // without using the accessor functions
  RBTDLL_EXPORT friend std::ostream &operator<<(std::ostream &s,
                                                const RbtBond &bond);

  ////////////////////////////////////////
  // Public accessor functions
  ////////////////

  // Bond ID
  int GetBondId() const { return m_nBondId; }
  void SetBondId(const int nBondId) { m_nBondId = nBondId; }

  // Atom pointers
  RbtAtomPtr GetAtom1Ptr() const { return m_spAtom1; }
  RbtAtomPtr GetAtom2Ptr() const { return m_spAtom2; }
  void SetAtom1Ptr(RbtAtomPtr &spAtom1) { m_spAtom1 = spAtom1; }
  void SetAtom2Ptr(RbtAtomPtr &spAtom2) { m_spAtom2 = spAtom2; }

  // Formal Bond order
  int GetFormalBondOrder() const { return m_nFormalBondOrder; }
  void SetFormalBondOrder(const int nFormalBondOrder) {
    m_nFormalBondOrder = nFormalBondOrder;
  }

  // Partial Bond order
  double GetPartialBondOrder() const { return m_dPartialBondOrder; }
  void SetPartialBondOrder(const double dPartialBondOrder) {
    m_dPartialBondOrder = dPartialBondOrder;
  }

  // CyclicFlag - flag to indicate bond is in a ring (set by RbtModel::FindRing)
  bool GetCyclicFlag() const { return m_bCyclic; }
  void SetCyclicFlag(bool bCyclic = true) { m_bCyclic = bCyclic; }

  // SelectionFlag - general purpose flag can be set/cleared by various search
  // algorithms (e.g. FindRings)
  bool GetSelectionFlag() const { return m_bSelected; }
  void SetSelectionFlag(bool bSelected = true) { m_bSelected = bSelected; }

  ////////////////////////////////////////
  // Public methods
  ////////////////
  // Returns bond length
  double Length() const;

protected:
  ////////////////////////////////////////
  // Protected methods
  ///////////////////

private:
  ////////////////////////////////////////
  // Private methods
  /////////////////

protected:
  ////////////////////////////////////////
  // Protected data
  ////////////////

private:
  ////////////////////////////////////////
  // Private data
  //////////////
  int m_nBondId;              // Original bond ID in PSF file
  RbtAtomPtr m_spAtom1;       // Smart pointer to atom 1
  RbtAtomPtr m_spAtom2;       // Smart pointer to atom 2
  int m_nFormalBondOrder;     // Formal bond order (1,2,3, no aromatic bond
                              // orders of 1.5)
  double m_dPartialBondOrder; // Partial bond order (1.0, 1.5, 2.0, 3.0 etc)
  bool m_bCyclic;             // Is the bond in a ring ?
  bool m_bSelected; // Can be set/cleared by various search algorithms (e.g.
                    // FindRings)
};

// Useful typedefs
typedef SmartPtr<RbtBond> RbtBondPtr;        // Smart pointer
typedef std::vector<RbtBondPtr> RbtBondList; // Vector of smart pointers
typedef RbtBondList::iterator RbtBondListIter;
typedef RbtBondList::const_iterator RbtBondListConstIter;

///////////////////////////////////////////////
// Non-member functions (in rxdock namespace)
//////////////////////////////////////////

std::ostream &operator<<(std::ostream &s, const RbtBond &bond);

////////////////////////////////////////////
// Predicates (for use by STL algorithms)
////////////////////////////////////////////
// DM 08 May 2002 - convert to regular RbtBond* functions
// More universal as RbtBondPtr parameters will be automatically degraded to
// regular pointers anyway

// Is bond selected ?
class isBondSelected : public std::function<bool(RbtBond *)> {
public:
  explicit isBondSelected() = default;
  bool operator()(RbtBond *pBond) const { return pBond->GetSelectionFlag(); }
};

// Is bond cyclic ?
class isBondCyclic : public std::function<bool(RbtBond *)> {
public:
  explicit isBondCyclic() = default;
  bool operator()(RbtBond *pBond) const { return pBond->GetCyclicFlag(); }
};

// Is bond rotatable ?
class isBondRotatable : public std::function<bool(RbtBond *)> {
public:
  explicit isBondRotatable() = default;
  RBTDLL_EXPORT bool operator()(RbtBond *) const;
};

// Is bond to a terminal NH3+ group?
class isBondToNH3 : public std::function<bool(RbtBond *)> {
public:
  explicit isBondToNH3() = default;
  RBTDLL_EXPORT bool operator()(RbtBond *) const;
};

// Is bond to a terminal OH group?
class isBondToOH : public std::function<bool(RbtBond *)> {
public:
  explicit isBondToOH() = default;
  RBTDLL_EXPORT bool operator()(RbtBond *pBond) const;
};

// DM 1 April 1999
// Is bond2 equal to bond1 (checks if underlying regular pointers match)
// Note: this is a binary rather than unary predicate
// class isBondPtr_eq : public std::function<bool(RbtBond *, RbtBond *)> {
// public:
//  explicit isBondPtr_eq() {}
//  RbtBool operator() (RbtBondPtr spBond1, RbtBondPtr spBond2) const {return
//  spBond1.Ptr() == spBond2.Ptr();}
//};

// Is bond.Ptr() equal to RbtBond* (checks if underlying regular pointers match)
class isBondPtr_eq : public std::function<bool(RbtBond *)> {
  RbtBond *p;

public:
  explicit isBondPtr_eq(RbtBond *pp) : p(pp) {}
  bool operator()(RbtBond *pBond) const { return pBond == p; }
};

// DM 2 Aug 1999
// Is bond2 equal to bond1 (checks if bond ID, atom1 and atom2 match)
class isBond_eq : public std::function<bool(RbtBond *)> {
  RbtBond *p;
  isAtom_eq bIsAtomEqual;

public:
  explicit isBond_eq(RbtBond *pp) : p(pp) {}
  bool operator()(RbtBond *pBond) const {
    return ((pBond->GetBondId() == p->GetBondId()) &&
            (bIsAtomEqual(pBond->GetAtom1Ptr(), p->GetAtom1Ptr())) &&
            (bIsAtomEqual(pBond->GetAtom2Ptr(), p->GetAtom2Ptr())));
  }
};

// DM 7 June 1999
// Is bond an amide bond?
class isBondAmide : public std::function<bool(RbtBond *)> {
public:
  explicit isBondAmide() = default;
  RBTDLL_EXPORT bool operator()(RbtBond *) const;
};

////////////////////////////////////////////
// Bond list functions (implemented as STL algorithms)
////////////////////////////////////////////

// Generic template version of GetNumBonds, passing in your own predicate
template <class Predicate>
unsigned int GetNumBondsWithPredicate(const RbtBondList &bondList,
                                      const Predicate &pred) {
  return static_cast<unsigned int>(
      std::count_if(bondList.begin(), bondList.end(), pred));
}

// Generic template version of GetBondList, passing in your own predicate
template <class Predicate>
RbtBondList GetBondListWithPredicate(const RbtBondList &bondList,
                                     const Predicate &pred) {
  RbtBondList newBondList;
  std::copy_if(bondList.begin(), bondList.end(),
               std::back_inserter(newBondList), pred);
  return newBondList;
}

// Generic template version of FindBond, passing in your own predicate
template <class Predicate>
RbtBondListIter FindBond(RbtBondList &bondList, const Predicate &pred) {
  return std::find_if(bondList.begin(), bondList.end(), pred);
}

// Selected bonds
void SetBondSelectionFlagsInList(RbtBondList &bondList, bool bSelected = true);
inline unsigned int GetNumSelectedBondsInList(const RbtBondList &bondList) {
  return GetNumBondsWithPredicate(bondList, isBondSelected());
}

inline RbtBondList GetSelectedBondsFromList(const RbtBondList &bondList) {
  return GetBondListWithPredicate(bondList, isBondSelected());
}

// Cyclic bonds
void SetBondCyclicFlagsInList(RbtBondList &bondList, bool bCyclic = true);
inline unsigned int GetNumCyclicBondsInList(const RbtBondList &bondList) {
  return GetNumBondsWithPredicate(bondList, isBondCyclic());
}
inline RbtBondList GetCyclicBondsFromList(const RbtBondList &bondList) {
  return GetBondListWithPredicate(bondList, isBondCyclic());
}

// Rotatable bonds
inline unsigned int GetNumRotatableBonds(const RbtBondList &bondList) {
  return GetNumBondsWithPredicate(bondList, isBondRotatable());
}
inline RbtBondList GetRotatableBondList(const RbtBondList &bondList) {
  return GetBondListWithPredicate(bondList, isBondRotatable());
}

} // namespace rxdock

#endif //_RBTBOND_H_
