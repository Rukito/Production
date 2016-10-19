//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jul 15 12:43:55 2016 by ROOT version 6.02/13
// from TTree HTauTauTree/HTauTauTree
// found on file: HTauTauAnalysis.root
//////////////////////////////////////////////////////////

#ifndef HTauTauTreeBase_h
#define HTauTauTreeBase_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>

// Header file for the classes stored in the TTree if any.
#include "vector"

#include "HTTEvent.h"
#include "ScaleFactor.h"
#include <iostream>

class HTauTauTreeBase {
public :

/////////////////////////////////////////////////
  ///Added by AK
  virtual void initWawTree(TTree *tree, std::string prefix="WAW");

  void fillEvent();
  virtual void fillPairs(unsigned int bestPairIndex);
  virtual void fillJets(unsigned int bestPairIndex);
  virtual void fillLeptons();
  virtual void fillGenLeptons();
  virtual bool thirdLeptonVeto(unsigned int signalLeg1Index, unsigned int signalLeg2Index, int leptonPdg, double dRmin=-1);
  virtual bool extraMuonVeto(unsigned int signalLeg1Index, unsigned int signalLeg2Index, double dRmin=-1);
  virtual bool extraElectronVeto(unsigned int signalLeg1Index, unsigned int signalLeg2Index, double dRmin=-1);
  bool muonSelection(unsigned int index);
  bool electronSelection(unsigned int index);
  virtual bool pairSelection(unsigned int index);  
  bool jetSelection(unsigned int index, unsigned int bestPairIndex);
  int getMCMatching(unsigned int index);
  bool isGoodToMatch(unsigned int ind);
  TLorentzVector getGenComponentP4(unsigned int index, unsigned int iAbsCharge);

  template<typename T> T getBranchValue(char *branchAddress, unsigned int index);
  float getProperty(std::string name, unsigned int index);
  std::vector<float> getProperties(const std::vector<std::string> & propertiesList, unsigned int index);
  void writePropertiesHeader(const std::vector<std::string> & propertiesList);
  void writeTriggersHeader(const TH1F*);
  
  std::vector<HTTPair> httPairCollection;
  std::vector<HTTParticle> httJetCollection;
  std::vector<HTTParticle> httLeptonCollection;
  std::vector<HTTParticle> httGenLeptonCollection;
  TTree *warsawTree;
  TFile *warsawFile;
  HTTEvent *httEvent;
  TH1F* hStats;
  ScaleFactor myScaleFactor;
  
  unsigned int bestPairIndex_;

  std::vector<std::string> leptonPropertiesList, genLeptonPropertiesList;

  ///Copy from LLRHiggsTauTau/NtupleProducer/plugins/HTauTauNtuplizer.cc
   static const int ntauIds = 30;
   TString tauIDStrings[ntauIds] = {
     "byLoosePileupWeightedIsolation3Hits",
     "byMediumPileupWeightedIsolation3Hits",
     "byTightPileupWeightedIsolation3Hits",
     "byLooseCombinedIsolationDeltaBetaCorr3Hits",
     "byMediumCombinedIsolationDeltaBetaCorr3Hits",
     "byTightCombinedIsolationDeltaBetaCorr3Hits",
     "againstMuonLoose3",
     "againstMuonTight3",
     "againstElectronVLooseMVA6",
     "againstElectronLooseMVA6",
     "againstElectronMediumMVA6",
     "againstElectronTightMVA6",
     "againstElectronVTightMVA6",
     "byVLooseIsolationMVArun2v1DBoldDMwLT",
     "byLooseIsolationMVArun2v1DBoldDMwLT",
     "byMediumIsolationMVArun2v1DBoldDMwLT",
     "byTightIsolationMVArun2v1DBoldDMwLT",
     "byVTightIsolationMVArun2v1DBoldDMwLT",
     "byVLooseIsolationMVArun2v1DBnewDMwLT",
     "byLooseIsolationMVArun2v1DBnewDMwLT",
     "byMediumIsolationMVArun2v1DBnewDMwLT",
     "byTightIsolationMVArun2v1DBnewDMwLT",
     "byVTightIsolationMVArun2v1DBnewDMwLT",
     "byLooseCombinedIsolationDeltaBetaCorr3HitsdR03",
     "byMediumCombinedIsolationDeltaBetaCorr3HitsdR03",
     "byTightCombinedIsolationDeltaBetaCorr3HitsdR03",
     "byLooseIsolationMVArun2v1DBdR03oldDMwLT",
     "byMediumIsolationMVArun2v1DBdR03oldDMwLT",
     "byTightIsolationMVArun2v1DBdR03oldDMwLT",
     "byVTightIsolationMVArun2v1DBdR03oldDMwLT"
   };
   /////////////////////////////////////////////////
  
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   ULong64_t       EventNumber;
   Int_t           RunNumber;
   Int_t           lumi;
   Long64_t        triggerbit;
   Int_t           metfilterbit;
   Float_t         met;
   Float_t         metphi;
   Float_t         PUPPImet;
   Float_t         PUPPImetphi;
   Int_t           npv;
   Float_t         npu;
   Float_t         PUReweight;
   Float_t         rho;
   vector<float>   *mothers_px;
   vector<float>   *mothers_py;
   vector<float>   *mothers_pz;
   vector<float>   *mothers_e;
   vector<float>   *daughters_px;
   vector<float>   *daughters_py;
   vector<float>   *daughters_pz;
   vector<float>   *daughters_e;
   vector<int>     *daughters_charge;
   vector<float>   *daughters_charged_px;
   vector<float>   *daughters_charged_py;
   vector<float>   *daughters_charged_pz;
   vector<float>   *daughters_charged_e;
   vector<float>   *daughters_neutral_px;
   vector<float>   *daughters_neutral_py;
   vector<float>   *daughters_neutral_pz;
   vector<float>   *daughters_neutral_e;
   vector<int>     *daughters_TauUpExists;
   vector<float>   *daughters_px_TauUp;
   vector<float>   *daughters_py_TauUp;
   vector<float>   *daughters_pz_TauUp;
   vector<float>   *daughters_e_TauUp;
   vector<int>     *daughters_TauDownExists;
   vector<float>   *daughters_px_TauDown;
   vector<float>   *daughters_py_TauDown;
   vector<float>   *daughters_pz_TauDown;
   vector<float>   *daughters_e_TauDown;
   Int_t           PUNumInteractions;
   vector<int>     *daughters_genindex;
   Float_t         MC_weight;
   Float_t         lheHt;
   Int_t           lheNOutPartons;
   Float_t         aMCatNLOweight;
   vector<float>   *genpart_px;
   vector<float>   *genpart_py;
   vector<float>   *genpart_pz;
   vector<float>   *genpart_e;
   vector<float>   *genpart_pca_x;
   vector<float>   *genpart_pca_y;
   vector<float>   *genpart_pca_z;
   vector<int>     *genpart_pdg;
   vector<int>     *genpart_status;
   vector<int>     *genpart_HMothInd;
   vector<int>     *genpart_MSSMHMothInd;
   vector<int>     *genpart_TopMothInd;
   vector<int>     *genpart_TauMothInd;
   vector<int>     *genpart_ZMothInd;
   vector<int>     *genpart_WMothInd;
   vector<int>     *genpart_bMothInd;
   vector<int>     *genpart_HZDecayMode;
   vector<int>     *genpart_TopDecayMode;
   vector<int>     *genpart_WDecayMode;
   vector<int>     *genpart_TauGenDecayMode;
   vector<int>     *genpart_TauGenDetailedDecayMode;
   vector<int>     *genpart_flags;
   vector<float>   *genjet_px;
   vector<float>   *genjet_py;
   vector<float>   *genjet_pz;
   vector<float>   *genjet_e;
   vector<int>     *genjet_partonFlavour;
   vector<int>     *genjet_hadronFlavour;
   Int_t           NUP;
   vector<float>   *SVfitMass;
   vector<float>   *SVfitMassTauUp;
   vector<float>   *SVfitMassTauDown;
   vector<float>   *SVfitTransverseMass;
   vector<float>   *SVfitTransverseMassTauUp;
   vector<float>   *SVfitTransverseMassTauDown;
   vector<float>   *SVfit_pt;
   vector<float>   *SVfit_ptTauUp;
   vector<float>   *SVfit_ptTauDown;
   vector<float>   *SVfit_ptUnc;
   vector<float>   *SVfit_ptUncTauUp;
   vector<float>   *SVfit_ptUncTauDown;
   vector<float>   *SVfit_eta;
   vector<float>   *SVfit_etaTauUp;
   vector<float>   *SVfit_etaTauDown;
   vector<float>   *SVfit_etaUnc;
   vector<float>   *SVfit_etaUncTauUp;
   vector<float>   *SVfit_etaUncTauDown;
   vector<float>   *SVfit_phi;
   vector<float>   *SVfit_phiTauUp;
   vector<float>   *SVfit_phiTauDown;
   vector<float>   *SVfit_phiUnc;
   vector<float>   *SVfit_phiUncTauUp;
   vector<float>   *SVfit_phiUncTauDown;
   vector<float>   *SVfit_fitMETRho;
   vector<float>   *SVfit_fitMETRhoTauUp;
   vector<float>   *SVfit_fitMETRhoTauDown;
   vector<float>   *SVfit_fitMETPhi;
   vector<float>   *SVfit_fitMETPhiTauUp;
   vector<float>   *SVfit_fitMETPhiTauDown;
   vector<bool>    *isOSCand;
   vector<float>   *METx;
   vector<float>   *METy;
   vector<float>   *uncorrMETx;
   vector<float>   *uncorrMETy;
   vector<float>   *MET_cov00;
   vector<float>   *MET_cov01;
   vector<float>   *MET_cov10;
   vector<float>   *MET_cov11;
   vector<float>   *MET_significance;
   vector<float>   *mT_Dau1;
   vector<float>   *mT_Dau2;
   vector<int>     *PDGIdDaughters;
   vector<int>     *indexDau1;
   vector<int>     *indexDau2;
   vector<int>     *particleType;
   vector<float>   *discriminator;
   vector<int>     *daughters_muonID;
   vector<int>     *daughters_typeOfMuon;
   vector<float>   *dxy;
   vector<float>   *dz;
   vector<float>   *dxy_innerTrack;
   vector<float>   *dz_innerTrack;
   vector<float>   *daughters_rel_error_trackpt;
   vector<float>   *SIP;
   vector<bool>    *daughters_iseleBDT;
   vector<bool>    *daughters_iseleWP80;
   vector<bool>    *daughters_iseleWP90;
   vector<float>   *daughters_eleMVAnt;
   vector<bool>    *daughters_passConversionVeto;
   vector<int>     *daughters_eleMissingHits;
   vector<bool>    *daughters_iseleChargeConsistent;
   vector<int>     *daughters_eleCUTID;
   vector<int>     *decayMode;
   vector<Long64_t> *tauID;
   vector<float>   *combreliso;
   vector<float>   *daughters_IetaIeta;
   vector<float>   *daughters_hOverE;
   vector<float>   *daughters_deltaEtaSuperClusterTrackAtVtx;
   vector<float>   *daughters_deltaPhiSuperClusterTrackAtVtx;
   vector<float>   *daughters_IoEmIoP;
   vector<float>   *daughters_SCeta;
   vector<float>   *daughters_depositR03_tracker;
   vector<float>   *daughters_depositR03_ecal;
   vector<float>   *daughters_depositR03_hcal;
   vector<int>     *daughters_decayModeFindingOldDMs;
   vector<float>   *againstElectronMVA5category;
   vector<float>   *againstElectronMVA5raw;
   vector<float>   *byPileupWeightedIsolationRaw3Hits;
   vector<float>   *footprintCorrection;
   vector<float>   *neutralIsoPtSumWeight;
   vector<float>   *photonPtSumOutsideSignalCone;
   vector<int>     *daughters_decayModeFindingNewDMs;
   vector<float>   *daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits;
   vector<float>   *daughters_byIsolationMVA3oldDMwoLTraw;
   vector<float>   *daughters_byIsolationMVA3oldDMwLTraw;
   vector<float>   *daughters_byIsolationMVA3newDMwoLTraw;
   vector<float>   *daughters_byIsolationMVA3newDMwLTraw;
   vector<float>   *daughters_byIsolationMVArun2v1DBoldDMwLTraw;   
   vector<float>   *daughters_chargedIsoPtSum;
   vector<float>   *daughters_neutralIsoPtSum;
   vector<float>   *daughters_puCorrPtSum;
   vector<int>     *daughters_numChargedParticlesSignalCone;
   vector<int>     *daughters_numNeutralHadronsSignalCone;
   vector<int>     *daughters_numPhotonsSignalCone;
   vector<int>     *daughters_daughters_numParticlesSignalCone;
   vector<int>     *daughters_numChargedParticlesIsoCone;
   vector<int>     *daughters_numNeutralHadronsIsoCone;
   vector<int>     *daughters_numPhotonsIsoCone;
   vector<int>     *daughters_numParticlesIsoCone;
   vector<float>   *daughters_leadChargedParticlePt;
   vector<float>   *daughters_trackRefPt;
   vector<int>     *daughters_isLastTriggerObjectforPath;
   vector<int>     *daughters_isTriggerObjectforPath;
   vector<int>     *daughters_FilterFired;
   vector<int>     *daughters_isGoodTriggerType;
   vector<int>     *daughters_L3FilterFired;
   vector<int>     *daughters_L3FilterFiredLast;
   vector<float>   *daughters_HLTpt;
   vector<bool>    *daughters_isL1IsoTau28Matched;
   vector<int>     *daughters_jetNDauChargedMVASel;
   vector<float>   *daughters_miniRelIsoCharged;
   vector<float>   *daughters_miniRelIsoNeutral;
   vector<float>   *daughters_jetPtRel;
   vector<float>   *daughters_jetPtRatio;
   vector<float>   *daughters_jetBTagCSV;
   vector<float>   *daughters_lepMVA_mvaId;
   vector<float>   *daughters_pca_x;
   vector<float>   *daughters_pca_y;
   vector<float>   *daughters_pca_z;
   vector<float>   *daughters_pcaRefitPV_x;
   vector<float>   *daughters_pcaRefitPV_y;
   vector<float>   *daughters_pcaRefitPV_z;
   vector<float>   *daughters_pcaGenPV_x;
   vector<float>   *daughters_pcaGenPV_y;
   vector<float>   *daughters_pcaGenPV_z;   
   Int_t           JetsNumber;
   vector<float>   *jets_px;
   vector<float>   *jets_py;
   vector<float>   *jets_pz;
   vector<float>   *jets_e;
   vector<float>   *jets_rawPt;
   vector<float>   *jets_area;
   vector<float>   *jets_mT;
   vector<int>     *jets_Flavour;
   vector<int>     *jets_HadronFlavour;
   vector<int>     *jets_genjetIndex;
   vector<float>   *jets_PUJetID;
   vector<float>   *jets_PUJetIDupdated;
   vector<float>   *jets_vtxPt;
   vector<float>   *jets_vtxMass;
   vector<float>   *jets_vtx3dL;
   vector<float>   *jets_vtxNtrk;
   vector<float>   *jets_vtx3deL;
   vector<float>   *jets_leadTrackPt;
   vector<float>   *jets_leptonPtRel;
   vector<float>   *jets_leptonPt;
   vector<float>   *jets_leptonDeltaR;
   vector<float>   *jets_chEmEF;
   vector<float>   *jets_chHEF;
   vector<float>   *jets_nEmEF;
   vector<float>   *jets_nHEF;
   vector<int>     *jets_chMult;
   vector<float>   *jets_jecUnc;
   vector<float>   *bDiscriminator;
   vector<float>   *bCSVscore;
   vector<int>     *PFjetID;
   vector<float>   *jetRawf;
   vector<float>   *ak8jets_px;
   vector<float>   *ak8jets_py;
   vector<float>   *ak8jets_pz;
   vector<float>   *ak8jets_e;
   vector<float>   *ak8jets_SoftDropMass;
   vector<float>   *ak8jets_PrunedMass;
   vector<float>   *ak8jets_TrimmedMass;
   vector<float>   *ak8jets_FilteredMass;
   vector<float>   *ak8jets_tau1;
   vector<float>   *ak8jets_tau2;
   vector<float>   *ak8jets_tau3;
   vector<float>   *ak8jets_CSV;
   vector<int>     *ak8jets_nsubjets;
   vector<float>   *subjets_px;
   vector<float>   *subjets_py;
   vector<float>   *subjets_pz;
   vector<float>   *subjets_e;
   vector<float>   *subjets_CSV;
   vector<int>     *subjets_ak8MotherIdx;
   Float_t         pv_x;
   Float_t         pv_y;
   Float_t         pv_z;
   Float_t         pvRefit_x;
   Float_t         pvRefit_y;
   Float_t         pvRefit_z;
   Float_t         pvGen_x;
   Float_t         pvGen_y;
   Float_t         pvGen_z;
   Bool_t          isRefitPV;

   // List of branches
   TBranch        *b_EventNumber;   //!
   TBranch        *b_RunNumber;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_triggerbit;   //!
   TBranch        *b_metfilterbit;   //!
   TBranch        *b_met;   //!
   TBranch        *b_metphi;   //!
   TBranch        *b_PUPPImet;   //!
   TBranch        *b_PUPPImetphi;   //!
   TBranch        *b_npv;   //!
   TBranch        *b_npu;   //!
   TBranch        *b_PUReweight;   //!
   TBranch        *b_rho;   //!
   TBranch        *b_mothers_px;   //!
   TBranch        *b_mothers_py;   //!
   TBranch        *b_mothers_pz;   //!
   TBranch        *b_mothers_e;   //!
   TBranch        *b_daughters_px;   //!
   TBranch        *b_daughters_py;   //!
   TBranch        *b_daughters_pz;   //!
   TBranch        *b_daughters_e;   //!
   TBranch        *b_daughters_charge;   //!
   TBranch        *b_daughters_charged_px;   //!
   TBranch        *b_daughters_charged_py;   //!
   TBranch        *b_daughters_charged_pz;   //!
   TBranch        *b_daughters_charged_e;   //!
   TBranch        *b_daughters_neutral_px;   //!
   TBranch        *b_daughters_neutral_py;   //!
   TBranch        *b_daughters_neutral_pz;   //!
   TBranch        *b_daughters_neutral_e;   //!
   TBranch        *b_daughters_TauUpExists;   //!
   TBranch        *b_daughters_px_TauUp;   //!
   TBranch        *b_daughters_py_TauUp;   //!
   TBranch        *b_daughters_pz_TauUp;   //!
   TBranch        *b_daughters_e_TauUp;   //!
   TBranch        *b_daughters_TauDownExists;   //!
   TBranch        *b_daughters_px_TauDown;   //!
   TBranch        *b_daughters_py_TauDown;   //!
   TBranch        *b_daughters_pz_TauDown;   //!
   TBranch        *b_daughters_e_TauDown;   //!
   TBranch        *b_PUNumInteractions;   //!
   TBranch        *b_daughters_genindex;   //!
   TBranch        *b_MC_weight;   //!
   TBranch        *b_lheHt;   //!
   TBranch        *b_lheNOutPartons;   //!
   TBranch        *b_aMCatNLOweight;   //!
   TBranch        *b_genpart_px;   //!
   TBranch        *b_genpart_py;   //!
   TBranch        *b_genpart_pz;   //!
   TBranch        *b_genpart_e;   //!
   TBranch        *b_genpart_pca_x;   //!
   TBranch        *b_genpart_pca_y;   //!
   TBranch        *b_genpart_pca_z;   //!
   TBranch        *b_genpart_pdg;   //!
   TBranch        *b_genpart_status;   //!
   TBranch        *b_genpart_HMothInd;   //!
   TBranch        *b_genpart_MSSMHMothInd;   //!
   TBranch        *b_genpart_TopMothInd;   //!
   TBranch        *b_genpart_TauMothInd;   //!
   TBranch        *b_genpart_ZMothInd;   //!
   TBranch        *b_genpart_WMothInd;   //!
   TBranch        *b_genpart_bMothInd;   //!
   TBranch        *b_genpart_HZDecayMode;   //!
   TBranch        *b_genpart_TopDecayMode;   //!
   TBranch        *b_genpart_WDecayMode;   //!
   TBranch        *b_genpart_TauGenDecayMode;   //!
   TBranch        *b_genpart_TauGenDetailedDecayMode;   //!
   TBranch        *b_genpart_flags;   //!
   TBranch        *b_genjet_px;   //!
   TBranch        *b_genjet_py;   //!
   TBranch        *b_genjet_pz;   //!
   TBranch        *b_genjet_e;   //!
   TBranch        *b_genjet_partonFlavour;   //!
   TBranch        *b_genjet_hadronFlavour;   //!
   TBranch        *b_NUP;   //!
   TBranch        *b_SVfitMass;   //!
   TBranch        *b_SVfitMassTauUp;   //!
   TBranch        *b_SVfitMassTauDown;   //!
   TBranch        *b_SVfitTransverseMass;   //!
   TBranch        *b_SVfitTransverseMassTauUp;   //!
   TBranch        *b_SVfitTransverseMassTauDown;   //!
   TBranch        *b_SVfit_pt;   //!
   TBranch        *b_SVfit_ptTauUp;   //!
   TBranch        *b_SVfit_ptTauDown;   //!
   TBranch        *b_SVfit_ptUnc;   //!
   TBranch        *b_SVfit_ptUncTauUp;   //!
   TBranch        *b_SVfit_ptUncTauDown;   //!
   TBranch        *b_SVfit_eta;   //!
   TBranch        *b_SVfit_etaTauUp;   //!
   TBranch        *b_SVfit_etaTauDown;   //!
   TBranch        *b_SVfit_etaUnc;   //!
   TBranch        *b_SVfit_etaUncTauUp;   //!
   TBranch        *b_SVfit_etaUncTauDown;   //!
   TBranch        *b_SVfit_phi;   //!
   TBranch        *b_SVfit_phiTauUp;   //!
   TBranch        *b_SVfit_phiTauDown;   //!
   TBranch        *b_SVfit_phiUnc;   //!
   TBranch        *b_SVfit_phiUncTauUp;   //!
   TBranch        *b_SVfit_phiUncTauDown;   //!
   TBranch        *b_SVfit_fitMETRho;   //!
   TBranch        *b_SVfit_fitMETRhoTauUp;   //!
   TBranch        *b_SVfit_fitMETRhoTauDown;   //!
   TBranch        *b_SVfit_fitMETPhi;   //!
   TBranch        *b_SVfit_fitMETPhiTauUp;   //!
   TBranch        *b_SVfit_fitMETPhiTauDown;   //!
   TBranch        *b_isOSCand;   //!
   TBranch        *b_METx;   //!
   TBranch        *b_METy;   //!
   TBranch        *b_uncorrMETx;   //!
   TBranch        *b_uncorrMETy;   //!
   TBranch        *b_MET_cov00;   //!
   TBranch        *b_MET_cov01;   //!
   TBranch        *b_MET_cov10;   //!
   TBranch        *b_MET_cov11;   //!
   TBranch        *b_MET_significance;   //!
   TBranch        *b_mT_Dau1;   //!
   TBranch        *b_mT_Dau2;   //!
   TBranch        *b_PDGIdDaughters;   //!
   TBranch        *b_indexDau1;   //!
   TBranch        *b_indexDau2;   //!
   TBranch        *b_particleType;   //!
   TBranch        *b_discriminator;   //!
   TBranch        *b_daughters_muonID;   //!
   TBranch        *b_daughters_typeOfMuon;   //!
   TBranch        *b_dxy;   //!
   TBranch        *b_dz;   //!
   TBranch        *b_dxy_innerTrack;   //!
   TBranch        *b_dz_innerTrack;   //!
   TBranch        *b_daughters_rel_error_trackpt;   //!
   TBranch        *b_SIP;   //!
   TBranch        *b_daughters_iseleBDT;   //!
   TBranch        *b_daughters_iseleWP80;   //!
   TBranch        *b_daughters_iseleWP90;   //!
   TBranch        *b_daughters_eleMVAnt;   //!
   TBranch        *b_daughters_passConversionVeto;   //!
   TBranch        *b_daughters_eleMissingHits;   //!
   TBranch        *b_daughters_iseleChargeConsistent;   //!
   TBranch        *b_daughters_eleCUTID;   //!
   TBranch        *b_decayMode;   //!
   TBranch        *b_tauID;   //!
   TBranch        *b_combreliso;   //!
   TBranch        *b_daughters_IetaIeta;   //!
   TBranch        *b_daughters_hOverE;   //!
   TBranch        *b_daughters_deltaEtaSuperClusterTrackAtVtx;   //!
   TBranch        *b_daughters_deltaPhiSuperClusterTrackAtVtx;   //!
   TBranch        *b_daughters_IoEmIoP;   //!
   TBranch        *b_daughters_SCeta;   //!
   TBranch        *b_daughters_depositR03_tracker;   //!
   TBranch        *b_daughters_depositR03_ecal;   //!
   TBranch        *b_daughters_depositR03_hcal;   //!
   TBranch        *b_daughters_decayModeFindingOldDMs;   //!
   TBranch        *b_againstElectronMVA5category;   //!
   TBranch        *b_againstElectronMVA5raw;   //!
   TBranch        *b_byPileupWeightedIsolationRaw3Hits;   //!
   TBranch        *b_footprintCorrection;   //!
   TBranch        *b_neutralIsoPtSumWeight;   //!
   TBranch        *b_photonPtSumOutsideSignalCone;   //!
   TBranch        *b_daughters_decayModeFindingNewDMs;   //!
   TBranch        *b_daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits;   //!
   TBranch        *b_daughters_byIsolationMVA3oldDMwoLTraw;   //!
   TBranch        *b_daughters_byIsolationMVA3oldDMwLTraw;   //!
   TBranch        *b_daughters_byIsolationMVA3newDMwoLTraw;   //!
   TBranch        *b_daughters_byIsolationMVA3newDMwLTraw;   //!
   TBranch        *b_daughters_byIsolationMVArun2v1DBoldDMwLTraw;   //!
   TBranch        *b_daughters_chargedIsoPtSum;   //!
   TBranch        *b_daughters_neutralIsoPtSum;   //!
   TBranch        *b_daughters_puCorrPtSum;   //!
   TBranch        *b_daughters_numChargedParticlesSignalCone;   //!
   TBranch        *b_daughters_numNeutralHadronsSignalCone;   //!
   TBranch        *b_daughters_numPhotonsSignalCone;   //!
   TBranch        *b_daughters_daughters_numParticlesSignalCone;   //!
   TBranch        *b_daughters_numChargedParticlesIsoCone;   //!
   TBranch        *b_daughters_numNeutralHadronsIsoCone;   //!
   TBranch        *b_daughters_numPhotonsIsoCone;   //!
   TBranch        *b_daughters_numParticlesIsoCone;   //!
   TBranch        *b_daughters_leadChargedParticlePt;   //!
   TBranch        *b_daughters_trackRefPt;   //!
   TBranch        *b_daughters_isLastTriggerObjectforPath;   //!
   TBranch        *b_daughters_isTriggerObjectforPath;   //!
   TBranch        *b_daughters_FilterFired;   //!
   TBranch        *b_daughters_isGoodTriggerType;   //!
   TBranch        *b_daughters_L3FilterFired;   //!
   TBranch        *b_daughters_L3FilterFiredLast;   //!
   TBranch        *b_daughters_HLTpt;   //!
   TBranch        *b_daughters_isL1IsoTau28Matched;   //!
   TBranch        *b_daughters_jetNDauChargedMVASel;   //!
   TBranch        *b_daughters_miniRelIsoCharged;   //!
   TBranch        *b_daughters_miniRelIsoNeutral;   //!
   TBranch        *b_daughters_jetPtRel;   //!
   TBranch        *b_daughters_jetPtRatio;   //!
   TBranch        *b_daughters_jetBTagCSV;   //!
   TBranch        *b_daughters_lepMVA_mvaId;   //!
   TBranch        *b_daughters_pca_x;   //!
   TBranch        *b_daughters_pca_y;   //!
   TBranch        *b_daughters_pca_z;   //!
   TBranch        *b_daughters_pcaRefitPV_x;   //!
   TBranch        *b_daughters_pcaRefitPV_y;   //!
   TBranch        *b_daughters_pcaRefitPV_z;   //!
   TBranch        *b_daughters_pcaGenPV_x;   //!
   TBranch        *b_daughters_pcaGenPV_y;   //!
   TBranch        *b_daughters_pcaGenPV_z;   //!
   TBranch        *b_JetsNumber;   //!
   TBranch        *b_jets_px;   //!
   TBranch        *b_jets_py;   //!
   TBranch        *b_jets_pz;   //!
   TBranch        *b_jets_e;   //!
   TBranch        *b_jets_rawPt;   //!
   TBranch        *b_jets_area;   //!
   TBranch        *b_jets_mT;   //!
   TBranch        *b_jets_Flavour;   //!
   TBranch        *b_jets_HadronFlavour;   //!
   TBranch        *b_jets_genjetIndex;   //!
   TBranch        *b_jets_PUJetID;   //!
   TBranch        *b_jets_PUJetIDupdated;   //!
   TBranch        *b_jets_vtxPt;   //!
   TBranch        *b_jets_vtxMass;   //!
   TBranch        *b_jets_vtx3dL;   //!
   TBranch        *b_jets_vtxNtrk;   //!
   TBranch        *b_jets_vtx3deL;   //!
   TBranch        *b_jets_leadTrackPt;   //!
   TBranch        *b_jets_leptonPtRel;   //!
   TBranch        *b_jets_leptonPt;   //!
   TBranch        *b_jets_leptonDeltaR;   //!
   TBranch        *b_jets_chEmEF;   //!
   TBranch        *b_jets_chHEF;   //!
   TBranch        *b_jets_nEmEF;   //!
   TBranch        *b_jets_nHEF;   //!
   TBranch        *b_jets_chMult;   //!
   TBranch        *b_jets_jecUnc;   //!
   TBranch        *b_bDiscriminator;   //!
   TBranch        *b_bCSVscore;   //!
   TBranch        *b_PFjetID;   //!
   TBranch        *b_jetRawf;   //!
   TBranch        *b_ak8jets_px;   //!
   TBranch        *b_ak8jets_py;   //!
   TBranch        *b_ak8jets_pz;   //!
   TBranch        *b_ak8jets_e;   //!
   TBranch        *b_ak8jets_SoftDropMass;   //!
   TBranch        *b_ak8jets_PrunedMass;   //!
   TBranch        *b_ak8jets_TrimmedMass;   //!
   TBranch        *b_ak8jets_FilteredMass;   //!
   TBranch        *b_ak8jets_tau1;   //!
   TBranch        *b_ak8jets_tau2;   //!
   TBranch        *b_ak8jets_tau3;   //!
   TBranch        *b_ak8jets_CSV;   //!
   TBranch        *b_ak8jets_nsubjets;   //!
   TBranch        *b_subjets_px;   //!
   TBranch        *b_subjets_py;   //!
   TBranch        *b_subjets_pz;   //!
   TBranch        *b_subjets_e;   //!
   TBranch        *b_subjets_CSV;   //!
   TBranch        *b_subjets_ak8MotherIdx;   //!
   TBranch        *b_pv_x;   //!
   TBranch        *b_pv_y;   //!
   TBranch        *b_pv_z;   //!
   TBranch        *b_pvRefit_x;   //!
   TBranch        *b_pvRefit_y;   //!
   TBranch        *b_pvRefit_z;   //!
   TBranch        *b_pvGen_x;   //!
   TBranch        *b_pvGen_y;   //!
   TBranch        *b_pvGen_z;   //!
   TBranch        *b_isRefitPV;   //!

   HTauTauTreeBase(TTree *tree=0, std::string prefix="WAW");
   virtual ~HTauTauTreeBase();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif
