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

// GPFitnessFunction Class. GPFitnessFunction represents a fitness function
// valid for the CGP

#ifndef _RBT_GPFITNESSFUNCTION_H_
#define _RBT_GPFITNESSFUNCTION_H_

#include "RbtContext.h"
#include "RbtGPGenome.h"
#include "RbtGPTypes.h"
#include "RbtRand.h"
// #include <cmath>

namespace rxdock {

namespace geneticprogram {

class RbtGPFitnessFunction {
public:
  static std::string _CT;
  ///////////////////
  // Constructors
  ///////////////////
  RbtGPFitnessFunction();
  RbtGPFitnessFunction(const RbtGPFitnessFunction &);

  RbtGPFitnessFunction &operator=(const RbtGPFitnessFunction &);

  ///////////////////
  // Destructor
  //////////////////
  virtual ~RbtGPFitnessFunction();
  virtual void ReadTables(std::istream &, RbtReturnTypeArray &,
                          RbtReturnTypeArray &) = 0;
  double GetObjective() const;
  double GetFitness() const;
  void SetFitness(double);
  virtual double CalculateFitness(RbtGPGenomePtr, RbtReturnTypeArray &,
                                  RbtReturnTypeArray &, double, bool) = 0;
  virtual double CalculateFitness(RbtGPGenomePtr, RbtReturnTypeArray &,
                                  RbtReturnTypeArray &, bool) = 0;

protected:
  RbtRand &m_rand;
  double objective; // raw score
  double fitness;   // scaled score
  RbtReturnTypeArray inputTable, SFTable;

private:
};

// Useful typedefs
typedef SmartPtr<RbtGPFitnessFunction> RbtGPFitnessFunctionPtr; // Smart pointer
typedef std::vector<RbtGPFitnessFunctionPtr>
    RbtGPFitnessFunctionList; // Vector of smart pointers
typedef RbtGPFitnessFunctionList::iterator RbtGPFitnessFunctionListIter;
typedef RbtGPFitnessFunctionList::const_iterator
    RbtGPFitnessFunctionListConstIter;

} // namespace geneticprogram

} // namespace rxdock

#endif //_RbtGPFitnessFunction
