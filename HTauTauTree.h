#ifndef HTauTauTree_h
#define HTauTauTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>

// Header file for the classes stored in the TTree if any.
#include "vector"

#include "HTTEvent.h"
#include "ScaleFactor.h"
#include <iostream>

// Header of the base class
#include "HTauTauTreeBase.h"

class HTauTauTree : public HTauTauTreeBase {
 public :

  /////////////////////////////////////////////////
  /// MT final state specific
  bool diMuonVeto();
  bool pairSelection(unsigned int index);  
  /////////////////////////////////////////////////
  
  HTauTauTree(TTree *tree=0, std::string prefix="WAWMT");
  virtual ~HTauTauTree();
  
};

#endif

#ifdef HTauTauTree_cxx
HTauTauTree::HTauTauTree(TTree *tree, std::string prefix) : HTauTauTreeBase(tree, prefix) 
{}

HTauTauTree::~HTauTauTree()
{}

#endif // #ifdef HTauTauTree_cxx
