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

// Handles retrieval of atomic (coordinate) info
// from XPLOR PDB files (with segment name in last column).

#ifndef _RBTPDBFILESOURCE_H_
#define _RBTPDBFILESOURCE_H_

#include "RbtBaseMolecularFileSource.h"

namespace rxdock {

class RbtPdbFileSource : public RbtBaseMolecularFileSource {
public:
  // Constructors
  RbtPdbFileSource(const char *fileName);
  RbtPdbFileSource(const std::string fileName);

  // Default destructor
  virtual ~RbtPdbFileSource();

  ////////////////////////////////////////
  // Override public methods from RbtBaseMolecularDataSource
  virtual bool isTitleListSupported() { return true; }
  virtual bool isAtomListSupported() { return true; }
  virtual bool isCoordinatesSupported() { return true; }
  virtual bool isBondListSupported() { return false; }

protected:
  // Pure virtual in RbtBaseFileSource - needs to be defined here
  virtual void Parse();

private:
  // Private methods
  RbtPdbFileSource(); // Disable default constructor
  RbtPdbFileSource(
      const RbtPdbFileSource &); // Copy constructor disabled by default
  RbtPdbFileSource &
  operator=(const RbtPdbFileSource &); // Copy assignment disabled by default
};

// useful typedefs
typedef SmartPtr<RbtPdbFileSource> RbtPdbFileSourcePtr; // Smart pointer

} // namespace rxdock

#endif //_RBTPDBFILESOURCE_H_
