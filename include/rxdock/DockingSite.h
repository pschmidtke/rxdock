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

// The role of DockingSite is to manage the
// list of cavities generated by the site mapper, and to store a 3-D grid of
// precalculated distances to the nearest cavity coordinate. This grid can be
// used by scoring function classes to lookup the list of atoms required to
// calculate the score component.

#ifndef _RBTDOCKINGSITE_H_
#define _RBTDOCKINGSITE_H_

#include "Atom.h"
#include "Cavity.h"
#include "Config.h"
#include "Error.h"
#include "RealGrid.h"

namespace rxdock {

class DockingSite {
public:
  // STL predicate for selecting atoms within a defined distance range from
  // nearest cavity coords Uses precalculated distance grid
  class isAtomInRange : public std::function<bool(AtomPtr)> {
  public:
    explicit isAtomInRange(RealGrid *pGrid, double minDist, double maxDist)
        : m_pGrid(pGrid), m_minDist(minDist), m_maxDist(maxDist) {}
    bool operator()(Atom *pAtom) const;

  private:
    RealGrid *m_pGrid;
    double m_minDist;
    double m_maxDist;
  };

public:
  // Class type string
  static std::string _CT;

  RBTDLL_EXPORT DockingSite(const CavityList &cavList, double border);
  RBTDLL_EXPORT DockingSite(json j);

  // Destructor
  virtual ~DockingSite();

  // Insertion operator
  RBTDLL_EXPORT friend std::ostream &operator<<(std::ostream &s,
                                                const DockingSite &site);

  RBTDLL_EXPORT friend void to_json(json &j, const DockingSite &site);
  friend void from_json(const json &j, DockingSite &site);

  // Virtual function for dumping docking site details to an output stream
  // Derived classes can override if required
  virtual void Print(std::ostream &s) const;

  // Public methods
  RBTDLL_EXPORT RealGridPtr GetGrid();
  double GetBorder() const { return m_border; }
  Coord GetMinCoord() const { return m_minCoord; }
  Coord GetMaxCoord() const { return m_maxCoord; }
  CavityList GetCavityList() const { return m_cavityList; }
  int GetNumCavities() const { return m_cavityList.size(); }
  RBTDLL_EXPORT double
  GetVolume() const; // returns total volume of all cavities in A^3

  // Returns the combined coord lists of all the cavities
  void GetCoordList(CoordList &retVal) const;

  // Filters an atom list according to distance from the cavity coords
  // Only returns atoms within minDist and maxDist from cavity
  RBTDLL_EXPORT AtomList GetAtomList(const AtomList &atomList, double minDist,
                                     double maxDist);
  // Filters an atom list according to distance from the cavity coords
  // Only returns atoms within maxDist from cavity
  // This version does not require the cavity grid
  RBTDLL_EXPORT AtomList GetAtomList(const AtomList &atomList, double maxDist);
  // Returns the count of atoms within minDist and maxDist from cavity
  unsigned int GetNumAtoms(const AtomList &atomList, double minDist,
                           double maxDist);

  // private methods
private:
  DockingSite();                    // default constructor disabled
  DockingSite(const DockingSite &); // Copy constructor disabled by
                                    // default
  DockingSite &
  operator=(const DockingSite &); // Copy assignment disabled by default

  void CreateGrid();

  // Private data
private:
  CavityList m_cavityList; // Cavity list
  Coord m_minCoord;
  Coord m_maxCoord;
  RealGridPtr m_spGrid;
  double m_border;
};

// Useful typedefs
typedef SmartPtr<DockingSite> DockingSitePtr; // Smart pointer

std::ostream &operator<<(std::ostream &s, const DockingSite &site);

RBTDLL_EXPORT void to_json(json &j, const DockingSite &site);
void from_json(const json &j, DockingSite &site);

} // namespace rxdock

#endif //_RBTDOCKINGSITE_H_