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

#include "VdwGridSF.h"
#include "FileError.h"
#include "WorkSpace.h"

using namespace rxdock;

// Static data members
std::string VdwGridSF::_CT("VdwGridSF");
std::string VdwGridSF::_GRID("GRID");
std::string VdwGridSF::_SMOOTHED("SMOOTHED");

std::string &VdwGridSF::GetCt() { return _CT; }

// NB - Virtual base class constructor (BaseSF) gets called first,
// implicit constructor for BaseInterSF is called second
VdwGridSF::VdwGridSF(const std::string &strName)
    : BaseSF(_CT, strName), m_bSmoothed(true) {
  // Add parameters
  AddParameter(_GRID, ".grd");
  AddParameter(_SMOOTHED, m_bSmoothed);
#ifdef _DEBUG
  std::cout << _CT << " parameterised constructor" << std::endl;
#endif //_DEBUG
  _RBTOBJECTCOUNTER_CONSTR_(_CT);
}

VdwGridSF::~VdwGridSF() {
#ifdef _DEBUG
  std::cout << _CT << " destructor" << std::endl;
#endif //_DEBUG
  _RBTOBJECTCOUNTER_DESTR_(_CT);
}

void VdwGridSF::SetupReceptor() {
  m_grids.clear();
  if (GetReceptor().Null())
    return;

  // Trap multiple receptor conformations and flexible OH/NH3 here: this SF does
  // not support them yet
  bool bEnsemble = (GetReceptor()->GetNumSavedCoords() > 1);
  bool bFlexRec = GetReceptor()->isFlexible();
  if (bEnsemble || bFlexRec) {
    std::string message("Vdw grid scoring function does not support multiple "
                        "receptor conformations\n");
    message += "or flexible OH/NH3 groups yet";
    throw InvalidRequest(_WHERE_, message);
  }

  // Read grids
  // File names are composed of workspace name + grid suffix
  // Reasonably safe to assume that GetWorkSpace is not nullptr,
  // as otherwise SetupReceptor would not have been called
  std::string strWSName = GetWorkSpace()->GetName();

  std::string strSuffix = GetParameter(_GRID);
  std::string strFile = GetDataFileName("data/grids", strWSName + strSuffix);
  std::ifstream file(strFile.c_str());
  json vdwGrids;
  file >> vdwGrids;
  file.close();
  ReadGrids(vdwGrids.at("vdw-grids"));
}

void VdwGridSF::SetupLigand() {
  m_ligAtomList.clear();
  m_ligAtomTypes.clear();
  if (GetLigand().Null())
    return;

  AtomList tmpList = GetLigand()->GetAtomList();
  // Strip off the smart pointers
  std::copy(tmpList.begin(), tmpList.end(), std::back_inserter(m_ligAtomList));
}

void VdwGridSF::SetupSolvent() {
  ModelList solvent = GetSolvent();
  if (!solvent.empty()) {
    std::string message(
        "Vdw grid scoring function does not support explicit solvent yet\n");
    throw InvalidRequest(_WHERE_, message);
  }
}

void VdwGridSF::SetupScore() {
  // Determine probe grid type for each atom, based on comparing Tripos atom
  // type with probe atom types This needs to be in SetupScore as it is
  // dependent on both the ligand and receptor grid data
  m_ligAtomTypes.clear();
  if (m_ligAtomList.empty())
    return;

  int iTrace = GetTrace();
  m_ligAtomTypes.reserve(m_ligAtomList.size());
  // Check if we have a grid for the UNDEFINED type:
  // If so, we can use it if a particular atom type grid is missing
  // If not, then we have to throw an error if a particular atom type grid is
  // missing
  bool bHasUndefined = !m_grids[TriposAtomType::UNDEFINED].Null();
  TriposAtomType triposType;

  if (iTrace > 1) {
    if (bHasUndefined) {
      std::cout
          << "The grid for the UNDEFINED atom type will be used if a grid is "
             "missing for any atom type"
          << std::endl;
    } else {
      std::cout
          << "There is no grid for the UNDEFINED atom type. An error will be "
             "thrown if a grid is missing for any atom type"
          << std::endl;
    }
  }

  for (AtomRListConstIter iter = m_ligAtomList.begin();
       iter != m_ligAtomList.end(); iter++) {
    TriposAtomType::eType aType = (*iter)->GetTriposType();

    // If there is no grid for this atom type, revert to using the UNDEFINED
    // grid if available else throw an error
    if (m_grids[aType].Null()) {
      std::string strError = "No vdw grid available for " +
                             (*iter)->GetFullAtomName() + " (type " +
                             triposType.Type2Str(aType) + ")";
      if (iTrace > 1) {
        std::cout << strError << std::endl;
      }
      if (bHasUndefined) {
        aType = TriposAtomType::UNDEFINED;
      } else {
        throw FileError(_WHERE_, strError);
      }
    }

    m_ligAtomTypes.push_back(aType);
    if (iTrace > 1) {
      std::cout << "Using grid #" << aType << " for "
                << (*iter)->GetFullAtomName() << std::endl;
    }
  }
}

double VdwGridSF::RawScore() const {
  double score = 0.0;

  // Check grids are defined
  if (m_grids.empty())
    return score;

  // Loop over all ligand atoms
  AtomRListConstIter aIter = m_ligAtomList.begin();
  TriposAtomTypeListConstIter tIter = m_ligAtomTypes.begin();
  if (m_bSmoothed) {
    for (; aIter != m_ligAtomList.end(); aIter++, tIter++) {
      score += m_grids[*tIter]->GetSmoothedValue((*aIter)->GetCoords());
    }
  } else {
    for (; aIter != m_ligAtomList.end(); aIter++, tIter++) {
      score += m_grids[*tIter]->GetValue((*aIter)->GetCoords());
    }
  }
  return score;
}

// Read grids from input stream, checking that header string matches
// VdwGridSF
void VdwGridSF::ReadGrids(json vdwGrids) {
  m_grids.clear();
  int iTrace = GetTrace();

  // Now read number of grids
  if (iTrace > 0) {
    std::cout << _CT << ": reading " << vdwGrids.size() << " grids..."
              << std::endl;
  }

  // We actually create a vector of size TriposAtomType::MAXTYPES
  // Each grid is in the file is prefixed by the atom type string (e.g. C.2)
  // This string is converted to the corresponding TriposAtomType::eType
  // and the grid stored at m_grids[eType]
  // This is slightly more transferable as it means grids can be read correctly
  // even if the atom type enums change (as long as the atom type string stay
  // the same) It also means we do not have to have a grid for each and every
  // atom type if we don't want to
  m_grids = RealGridList(TriposAtomType::MAXTYPES);
  for (int i = 0; i < vdwGrids.size(); i++) {
    // Read the atom type string
    std::string strType;
    vdwGrids.at(i).at("tripos-type").get_to(strType);
    TriposAtomType triposType;
    TriposAtomType::eType aType = triposType.Str2Type(strType);
    if (iTrace > 0) {
      std::cout << "Grid# " << i << "\t"
                << "atom type=" << strType << " (type #" << aType << ")"
                << std::endl;
    }
    // Now we can read the grid
    RealGridPtr spGrid(new RealGrid(vdwGrids.at(i).at("real-grid")));
    m_grids[aType] = spGrid;
  }
}

// DM 25 Oct 2000 - track changes to parameter values in local data members
// ParameterUpdated is invoked by ParamHandler::SetParameter
void VdwGridSF::ParameterUpdated(const std::string &strName) {
  // DM 25 Oct 2000 - heavily used params
  if (strName == _SMOOTHED) {
    m_bSmoothed = GetParameter(_SMOOTHED);
  } else {
    BaseSF::ParameterUpdated(strName);
  }
}
