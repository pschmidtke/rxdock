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

#include "RbtGPFitnessFunction.h"
#include "RbtDebug.h"
#include "RbtGPGenome.h"
#include "RbtGPParser.h"
#include <fstream>
#include <sstream>

using namespace rxdock;
using namespace rxdock::geneticprogram;

std::string RbtGPFitnessFunction::_CT("RbtGPFitnessFunction");

// Constructors
RbtGPFitnessFunction::RbtGPFitnessFunction()
    : m_rand(GetRbtRand()), objective(0.0), fitness(0.0) {
  _RBTOBJECTCOUNTER_CONSTR_(_CT);
}

RbtGPFitnessFunction::RbtGPFitnessFunction(const RbtGPFitnessFunction &g)
    : m_rand(GetRbtRand()), objective(g.objective), fitness(g.fitness) {
  _RBTOBJECTCOUNTER_COPYCONSTR_(_CT);
}

RbtGPFitnessFunction &
RbtGPFitnessFunction::operator=(const RbtGPFitnessFunction &g) {
  if (this != &g) {
    objective = g.objective;
    fitness = g.fitness;
  }
  return *this;
}

///////////////////
// Destructor
//////////////////
RbtGPFitnessFunction::~RbtGPFitnessFunction() { _RBTOBJECTCOUNTER_DESTR_(_CT); }

double RbtGPFitnessFunction::GetObjective() const { return objective; }

double RbtGPFitnessFunction::GetFitness() const { return fitness; }

void RbtGPFitnessFunction::SetFitness(double f) { fitness = f; }
