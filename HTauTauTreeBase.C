#define HTauTauTreeBase_cxx
#include "HTauTauTreeBase.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TSystem.h>
//#include "ScaleFactor.cc"
//#include "HTTEvent.cxx"


#include <iostream>
#include <fstream>

HTauTauTreeBase::HTauTauTreeBase(TTree *tree, std::string prefix) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
     TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("HTauTauAnalysis.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("HTauTauAnalysis.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("HTauTauAnalysis.root:/HTauTauTree");
      dir->GetObject("HTauTauTree",tree);

   }
   Init(tree);

    /////////////////////////////////////////////////
   ///Added by AK
   initWawTree(tree, prefix);
   ////////////////////////////////////////////////////////////
   
}

HTauTauTreeBase::~HTauTauTreeBase()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();

   ////Added by AK
   if(warsawFile){
     warsawFile->Write();
     delete warsawFile;
   }
   /////////////////////////
}

Int_t HTauTauTreeBase::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t HTauTauTreeBase::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void HTauTauTreeBase::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   mothers_px = 0;
   mothers_py = 0;
   mothers_pz = 0;
   mothers_e = 0;
   daughters_px = 0;
   daughters_py = 0;
   daughters_pz = 0;
   daughters_e = 0;
   daughters_charge = 0;
   daughters_charged_px = 0;
   daughters_charged_py = 0;
   daughters_charged_pz = 0;
   daughters_charged_e = 0;
   daughters_neutral_px = 0;
   daughters_neutral_py = 0;
   daughters_neutral_pz = 0;
   daughters_neutral_e = 0;
   daughters_TauUpExists = 0;
   daughters_px_TauUp = 0;
   daughters_py_TauUp = 0;
   daughters_pz_TauUp = 0;
   daughters_e_TauUp = 0;
   daughters_TauDownExists = 0;
   daughters_px_TauDown = 0;
   daughters_py_TauDown = 0;
   daughters_pz_TauDown = 0;
   daughters_e_TauDown = 0;
   daughters_genindex = 0;
   genpart_px = 0;
   genpart_py = 0;
   genpart_pz = 0;
   genpart_e = 0;
   genpart_pca_x = 0;
   genpart_pca_y = 0;
   genpart_pca_z = 0;
   genpart_pdg = 0;
   genpart_status = 0;
   genpart_HMothInd = 0;
   genpart_MSSMHMothInd = 0;
   genpart_TopMothInd = 0;
   genpart_TauMothInd = 0;
   genpart_ZMothInd = 0;
   genpart_WMothInd = 0;
   genpart_bMothInd = 0;
   genpart_HZDecayMode = 0;
   genpart_TopDecayMode = 0;
   genpart_WDecayMode = 0;
   genpart_TauGenDecayMode = 0;
   genpart_TauGenDetailedDecayMode = 0;
   genpart_flags = 0;
   genjet_px = 0;
   genjet_py = 0;
   genjet_pz = 0;
   genjet_e = 0;
   genjet_partonFlavour = 0;
   genjet_hadronFlavour = 0;
   SVfitMass = 0;
   SVfitMassTauUp = 0;
   SVfitMassTauDown = 0;
   SVfitTransverseMass = 0;
   SVfitTransverseMassTauUp = 0;
   SVfitTransverseMassTauDown = 0;
   SVfit_pt = 0;
   SVfit_ptTauUp = 0;
   SVfit_ptTauDown = 0;
   SVfit_ptUnc = 0;
   SVfit_ptUncTauUp = 0;
   SVfit_ptUncTauDown = 0;
   SVfit_eta = 0;
   SVfit_etaTauUp = 0;
   SVfit_etaTauDown = 0;
   SVfit_etaUnc = 0;
   SVfit_etaUncTauUp = 0;
   SVfit_etaUncTauDown = 0;
   SVfit_phi = 0;
   SVfit_phiTauUp = 0;
   SVfit_phiTauDown = 0;
   SVfit_phiUnc = 0;
   SVfit_phiUncTauUp = 0;
   SVfit_phiUncTauDown = 0;
   SVfit_fitMETRho = 0;
   SVfit_fitMETRhoTauUp = 0;
   SVfit_fitMETRhoTauDown = 0;
   SVfit_fitMETPhi = 0;
   SVfit_fitMETPhiTauUp = 0;
   SVfit_fitMETPhiTauDown = 0;
   isOSCand = 0;
   METx = 0;
   METy = 0;
   uncorrMETx = 0;
   uncorrMETy = 0;
   MET_cov00 = 0;
   MET_cov01 = 0;
   MET_cov10 = 0;
   MET_cov11 = 0;
   MET_significance = 0;
   mT_Dau1 = 0;
   mT_Dau2 = 0;
   PDGIdDaughters = 0;
   indexDau1 = 0;
   indexDau2 = 0;
   particleType = 0;
   discriminator = 0;
   daughters_muonID = 0;
   daughters_typeOfMuon = 0;
   dxy = 0;
   dz = 0;
   dxy_innerTrack = 0;
   dz_innerTrack = 0;
   daughters_rel_error_trackpt = 0;
   SIP = 0;
   daughters_iseleBDT = 0;
   daughters_iseleWP80 = 0;
   daughters_iseleWP90 = 0;
   daughters_eleMVAnt = 0;
   daughters_passConversionVeto = 0;
   daughters_eleMissingHits = 0;
   daughters_iseleChargeConsistent = 0;
   daughters_eleCUTID = 0;
   decayMode = 0;
   tauID = 0;
   combreliso = 0;
   daughters_IetaIeta = 0;
   daughters_hOverE = 0;
   daughters_deltaEtaSuperClusterTrackAtVtx = 0;
   daughters_deltaPhiSuperClusterTrackAtVtx = 0;
   daughters_IoEmIoP = 0;
   daughters_SCeta = 0;
   daughters_depositR03_tracker = 0;
   daughters_depositR03_ecal = 0;
   daughters_depositR03_hcal = 0;
   daughters_decayModeFindingOldDMs = 0;
   againstElectronMVA5category = 0;
   againstElectronMVA5raw = 0;
   byPileupWeightedIsolationRaw3Hits = 0;
   footprintCorrection = 0;
   neutralIsoPtSumWeight = 0;
   photonPtSumOutsideSignalCone = 0;
   daughters_decayModeFindingNewDMs = 0;
   daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits = 0;
   daughters_byIsolationMVA3oldDMwoLTraw = 0;
   daughters_byIsolationMVA3oldDMwLTraw = 0;
   daughters_byIsolationMVA3newDMwoLTraw = 0;
   daughters_byIsolationMVA3newDMwLTraw = 0;
   daughters_byIsolationMVArun2v1DBoldDMwLTraw = 0;
   daughters_chargedIsoPtSum = 0;
   daughters_neutralIsoPtSum = 0;
   daughters_puCorrPtSum = 0;
   daughters_numChargedParticlesSignalCone = 0;
   daughters_numNeutralHadronsSignalCone = 0;
   daughters_numPhotonsSignalCone = 0;
   daughters_daughters_numParticlesSignalCone = 0;
   daughters_numChargedParticlesIsoCone = 0;
   daughters_numNeutralHadronsIsoCone = 0;
   daughters_numPhotonsIsoCone = 0;
   daughters_numParticlesIsoCone = 0;
   daughters_leadChargedParticlePt = 0;
   daughters_trackRefPt = 0;
   daughters_isLastTriggerObjectforPath = 0;
   daughters_isTriggerObjectforPath = 0;
   daughters_FilterFired = 0;
   daughters_isGoodTriggerType = 0;
   daughters_L3FilterFired = 0;
   daughters_L3FilterFiredLast = 0;
   daughters_HLTpt = 0;
   daughters_isL1IsoTau28Matched = 0;
   daughters_jetNDauChargedMVASel = 0;
   daughters_miniRelIsoCharged = 0;
   daughters_miniRelIsoNeutral = 0;
   daughters_jetPtRel = 0;
   daughters_jetPtRatio = 0;
   daughters_jetBTagCSV = 0;
   daughters_lepMVA_mvaId = 0;
   daughters_pca_x = 0;
   daughters_pca_y = 0;
   daughters_pca_z = 0;
   daughters_pcaRefitPV_x = 0;
   daughters_pcaRefitPV_y = 0;
   daughters_pcaRefitPV_z = 0;
   daughters_pcaGenPV_x = 0;
   daughters_pcaGenPV_y = 0;
   daughters_pcaGenPV_z = 0;
   jets_px = 0;
   jets_py = 0;
   jets_pz = 0;
   jets_e = 0;
   jets_rawPt = 0;
   jets_area = 0;
   jets_mT = 0;
   jets_Flavour = 0;
   jets_HadronFlavour = 0;
   jets_genjetIndex = 0;
   jets_PUJetID = 0;
   jets_PUJetIDupdated = 0;
   jets_vtxPt = 0;
   jets_vtxMass = 0;
   jets_vtx3dL = 0;
   jets_vtxNtrk = 0;
   jets_vtx3deL = 0;
   jets_leadTrackPt = 0;
   jets_leptonPtRel = 0;
   jets_leptonPt = 0;
   jets_leptonDeltaR = 0;
   jets_chEmEF = 0;
   jets_chHEF = 0;
   jets_nEmEF = 0;
   jets_nHEF = 0;
   jets_chMult = 0;
   jets_jecUnc = 0;
   bDiscriminator = 0;
   bCSVscore = 0;
   PFjetID = 0;
   jetRawf = 0;
   ak8jets_px = 0;
   ak8jets_py = 0;
   ak8jets_pz = 0;
   ak8jets_e = 0;
   ak8jets_SoftDropMass = 0;
   ak8jets_PrunedMass = 0;
   ak8jets_TrimmedMass = 0;
   ak8jets_FilteredMass = 0;
   ak8jets_tau1 = 0;
   ak8jets_tau2 = 0;
   ak8jets_tau3 = 0;
   ak8jets_CSV = 0;
   ak8jets_nsubjets = 0;
   subjets_px = 0;
   subjets_py = 0;
   subjets_pz = 0;
   subjets_e = 0;
   subjets_CSV = 0;
   subjets_ak8MotherIdx = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber);
   fChain->SetBranchAddress("RunNumber", &RunNumber, &b_RunNumber);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("triggerbit", &triggerbit, &b_triggerbit);
   fChain->SetBranchAddress("metfilterbit", &metfilterbit, &b_metfilterbit);
   fChain->SetBranchAddress("met", &met, &b_met);
   fChain->SetBranchAddress("metphi", &metphi, &b_metphi);
   fChain->SetBranchAddress("PUPPImet", &PUPPImet, &b_PUPPImet);
   fChain->SetBranchAddress("PUPPImetphi", &PUPPImetphi, &b_PUPPImetphi);
   fChain->SetBranchAddress("npv", &npv, &b_npv);
   fChain->SetBranchAddress("npu", &npu, &b_npu);
   fChain->SetBranchAddress("PUReweight", &PUReweight, &b_PUReweight);
   fChain->SetBranchAddress("rho", &rho, &b_rho);
   fChain->SetBranchAddress("mothers_px", &mothers_px, &b_mothers_px);
   fChain->SetBranchAddress("mothers_py", &mothers_py, &b_mothers_py);
   fChain->SetBranchAddress("mothers_pz", &mothers_pz, &b_mothers_pz);
   fChain->SetBranchAddress("mothers_e", &mothers_e, &b_mothers_e);
   fChain->SetBranchAddress("daughters_px", &daughters_px, &b_daughters_px);
   fChain->SetBranchAddress("daughters_py", &daughters_py, &b_daughters_py);
   fChain->SetBranchAddress("daughters_pz", &daughters_pz, &b_daughters_pz);
   fChain->SetBranchAddress("daughters_e", &daughters_e, &b_daughters_e);
   fChain->SetBranchAddress("daughters_charge", &daughters_charge, &b_daughters_charge);
   fChain->SetBranchAddress("daughters_charged_px", &daughters_charged_px, &b_daughters_charged_px);
   fChain->SetBranchAddress("daughters_charged_py", &daughters_charged_py, &b_daughters_charged_py);
   fChain->SetBranchAddress("daughters_charged_pz", &daughters_charged_pz, &b_daughters_charged_pz);
   fChain->SetBranchAddress("daughters_charged_e", &daughters_charged_e, &b_daughters_charged_e);
   fChain->SetBranchAddress("daughters_neutral_px", &daughters_neutral_px, &b_daughters_neutral_px);
   fChain->SetBranchAddress("daughters_neutral_py", &daughters_neutral_py, &b_daughters_neutral_py);
   fChain->SetBranchAddress("daughters_neutral_pz", &daughters_neutral_pz, &b_daughters_neutral_pz);
   fChain->SetBranchAddress("daughters_neutral_e", &daughters_neutral_e, &b_daughters_neutral_e);
   fChain->SetBranchAddress("daughters_TauUpExists", &daughters_TauUpExists, &b_daughters_TauUpExists);
   fChain->SetBranchAddress("daughters_px_TauUp", &daughters_px_TauUp, &b_daughters_px_TauUp);
   fChain->SetBranchAddress("daughters_py_TauUp", &daughters_py_TauUp, &b_daughters_py_TauUp);
   fChain->SetBranchAddress("daughters_pz_TauUp", &daughters_pz_TauUp, &b_daughters_pz_TauUp);
   fChain->SetBranchAddress("daughters_e_TauUp", &daughters_e_TauUp, &b_daughters_e_TauUp);
   fChain->SetBranchAddress("daughters_TauDownExists", &daughters_TauDownExists, &b_daughters_TauDownExists);
   fChain->SetBranchAddress("daughters_px_TauDown", &daughters_px_TauDown, &b_daughters_px_TauDown);
   fChain->SetBranchAddress("daughters_py_TauDown", &daughters_py_TauDown, &b_daughters_py_TauDown);
   fChain->SetBranchAddress("daughters_pz_TauDown", &daughters_pz_TauDown, &b_daughters_pz_TauDown);
   fChain->SetBranchAddress("daughters_e_TauDown", &daughters_e_TauDown, &b_daughters_e_TauDown);
   fChain->SetBranchAddress("PUNumInteractions", &PUNumInteractions, &b_PUNumInteractions);
   fChain->SetBranchAddress("daughters_genindex", &daughters_genindex, &b_daughters_genindex);
   fChain->SetBranchAddress("MC_weight", &MC_weight, &b_MC_weight);
   fChain->SetBranchAddress("lheHt", &lheHt, &b_lheHt);
   fChain->SetBranchAddress("lheNOutPartons", &lheNOutPartons, &b_lheNOutPartons);
   fChain->SetBranchAddress("aMCatNLOweight", &aMCatNLOweight, &b_aMCatNLOweight);
   fChain->SetBranchAddress("genpart_px", &genpart_px, &b_genpart_px);
   fChain->SetBranchAddress("genpart_py", &genpart_py, &b_genpart_py);
   fChain->SetBranchAddress("genpart_pz", &genpart_pz, &b_genpart_pz);
   fChain->SetBranchAddress("genpart_e", &genpart_e, &b_genpart_e);
   fChain->SetBranchAddress("genpart_pca_x", &genpart_pca_x, &b_genpart_pca_x);
   fChain->SetBranchAddress("genpart_pca_y", &genpart_pca_y, &b_genpart_pca_y);
   fChain->SetBranchAddress("genpart_pca_z", &genpart_pca_z, &b_genpart_pca_z);
   fChain->SetBranchAddress("genpart_pdg", &genpart_pdg, &b_genpart_pdg);
   fChain->SetBranchAddress("genpart_status", &genpart_status, &b_genpart_status);
   fChain->SetBranchAddress("genpart_HMothInd", &genpart_HMothInd, &b_genpart_HMothInd);
   fChain->SetBranchAddress("genpart_MSSMHMothInd", &genpart_MSSMHMothInd, &b_genpart_MSSMHMothInd);
   fChain->SetBranchAddress("genpart_TopMothInd", &genpart_TopMothInd, &b_genpart_TopMothInd);
   fChain->SetBranchAddress("genpart_TauMothInd", &genpart_TauMothInd, &b_genpart_TauMothInd);
   fChain->SetBranchAddress("genpart_ZMothInd", &genpart_ZMothInd, &b_genpart_ZMothInd);
   fChain->SetBranchAddress("genpart_WMothInd", &genpart_WMothInd, &b_genpart_WMothInd);
   fChain->SetBranchAddress("genpart_bMothInd", &genpart_bMothInd, &b_genpart_bMothInd);
   fChain->SetBranchAddress("genpart_HZDecayMode", &genpart_HZDecayMode, &b_genpart_HZDecayMode);
   fChain->SetBranchAddress("genpart_TopDecayMode", &genpart_TopDecayMode, &b_genpart_TopDecayMode);
   fChain->SetBranchAddress("genpart_WDecayMode", &genpart_WDecayMode, &b_genpart_WDecayMode);
   fChain->SetBranchAddress("genpart_TauGenDecayMode", &genpart_TauGenDecayMode, &b_genpart_TauGenDecayMode);
   fChain->SetBranchAddress("genpart_TauGenDetailedDecayMode", &genpart_TauGenDetailedDecayMode, &b_genpart_TauGenDetailedDecayMode);
   fChain->SetBranchAddress("genpart_flags", &genpart_flags, &b_genpart_flags);
   fChain->SetBranchAddress("genjet_px", &genjet_px, &b_genjet_px);
   fChain->SetBranchAddress("genjet_py", &genjet_py, &b_genjet_py);
   fChain->SetBranchAddress("genjet_pz", &genjet_pz, &b_genjet_pz);
   fChain->SetBranchAddress("genjet_e", &genjet_e, &b_genjet_e);
   fChain->SetBranchAddress("genjet_partonFlavour", &genjet_partonFlavour, &b_genjet_partonFlavour);
   fChain->SetBranchAddress("genjet_hadronFlavour", &genjet_hadronFlavour, &b_genjet_hadronFlavour);
   fChain->SetBranchAddress("NUP", &NUP, &b_NUP);
   fChain->SetBranchAddress("SVfitMass", &SVfitMass, &b_SVfitMass);
   fChain->SetBranchAddress("SVfitMassTauUp", &SVfitMassTauUp, &b_SVfitMassTauUp);
   fChain->SetBranchAddress("SVfitMassTauDown", &SVfitMassTauDown, &b_SVfitMassTauDown);
   fChain->SetBranchAddress("SVfitTransverseMass", &SVfitTransverseMass, &b_SVfitTransverseMass);
   fChain->SetBranchAddress("SVfitTransverseMassTauUp", &SVfitTransverseMassTauUp, &b_SVfitTransverseMassTauUp);
   fChain->SetBranchAddress("SVfitTransverseMassTauDown", &SVfitTransverseMassTauDown, &b_SVfitTransverseMassTauDown);
   fChain->SetBranchAddress("SVfit_pt", &SVfit_pt, &b_SVfit_pt);
   fChain->SetBranchAddress("SVfit_ptTauUp", &SVfit_ptTauUp, &b_SVfit_ptTauUp);
   fChain->SetBranchAddress("SVfit_ptTauDown", &SVfit_ptTauDown, &b_SVfit_ptTauDown);
   fChain->SetBranchAddress("SVfit_ptUnc", &SVfit_ptUnc, &b_SVfit_ptUnc);
   fChain->SetBranchAddress("SVfit_ptUncTauUp", &SVfit_ptUncTauUp, &b_SVfit_ptUncTauUp);
   fChain->SetBranchAddress("SVfit_ptUncTauDown", &SVfit_ptUncTauDown, &b_SVfit_ptUncTauDown);
   fChain->SetBranchAddress("SVfit_eta", &SVfit_eta, &b_SVfit_eta);
   fChain->SetBranchAddress("SVfit_etaTauUp", &SVfit_etaTauUp, &b_SVfit_etaTauUp);
   fChain->SetBranchAddress("SVfit_etaTauDown", &SVfit_etaTauDown, &b_SVfit_etaTauDown);
   fChain->SetBranchAddress("SVfit_etaUnc", &SVfit_etaUnc, &b_SVfit_etaUnc);
   fChain->SetBranchAddress("SVfit_etaUncTauUp", &SVfit_etaUncTauUp, &b_SVfit_etaUncTauUp);
   fChain->SetBranchAddress("SVfit_etaUncTauDown", &SVfit_etaUncTauDown, &b_SVfit_etaUncTauDown);
   fChain->SetBranchAddress("SVfit_phi", &SVfit_phi, &b_SVfit_phi);
   fChain->SetBranchAddress("SVfit_phiTauUp", &SVfit_phiTauUp, &b_SVfit_phiTauUp);
   fChain->SetBranchAddress("SVfit_phiTauDown", &SVfit_phiTauDown, &b_SVfit_phiTauDown);
   fChain->SetBranchAddress("SVfit_phiUnc", &SVfit_phiUnc, &b_SVfit_phiUnc);
   fChain->SetBranchAddress("SVfit_phiUncTauUp", &SVfit_phiUncTauUp, &b_SVfit_phiUncTauUp);
   fChain->SetBranchAddress("SVfit_phiUncTauDown", &SVfit_phiUncTauDown, &b_SVfit_phiUncTauDown);
   fChain->SetBranchAddress("SVfit_fitMETRho", &SVfit_fitMETRho, &b_SVfit_fitMETRho);
   fChain->SetBranchAddress("SVfit_fitMETRhoTauUp", &SVfit_fitMETRhoTauUp, &b_SVfit_fitMETRhoTauUp);
   fChain->SetBranchAddress("SVfit_fitMETRhoTauDown", &SVfit_fitMETRhoTauDown, &b_SVfit_fitMETRhoTauDown);
   fChain->SetBranchAddress("SVfit_fitMETPhi", &SVfit_fitMETPhi, &b_SVfit_fitMETPhi);
   fChain->SetBranchAddress("SVfit_fitMETPhiTauUp", &SVfit_fitMETPhiTauUp, &b_SVfit_fitMETPhiTauUp);
   fChain->SetBranchAddress("SVfit_fitMETPhiTauDown", &SVfit_fitMETPhiTauDown, &b_SVfit_fitMETPhiTauDown);
   fChain->SetBranchAddress("isOSCand", &isOSCand, &b_isOSCand);
   fChain->SetBranchAddress("METx", &METx, &b_METx);
   fChain->SetBranchAddress("METy", &METy, &b_METy);
   fChain->SetBranchAddress("uncorrMETx", &uncorrMETx, &b_uncorrMETx);
   fChain->SetBranchAddress("uncorrMETy", &uncorrMETy, &b_uncorrMETy);
   fChain->SetBranchAddress("MET_cov00", &MET_cov00, &b_MET_cov00);
   fChain->SetBranchAddress("MET_cov01", &MET_cov01, &b_MET_cov01);
   fChain->SetBranchAddress("MET_cov10", &MET_cov10, &b_MET_cov10);
   fChain->SetBranchAddress("MET_cov11", &MET_cov11, &b_MET_cov11);
   fChain->SetBranchAddress("MET_significance", &MET_significance, &b_MET_significance);
   fChain->SetBranchAddress("mT_Dau1", &mT_Dau1, &b_mT_Dau1);
   fChain->SetBranchAddress("mT_Dau2", &mT_Dau2, &b_mT_Dau2);
   fChain->SetBranchAddress("PDGIdDaughters", &PDGIdDaughters, &b_PDGIdDaughters);
   fChain->SetBranchAddress("indexDau1", &indexDau1, &b_indexDau1);
   fChain->SetBranchAddress("indexDau2", &indexDau2, &b_indexDau2);
   fChain->SetBranchAddress("particleType", &particleType, &b_particleType);
   fChain->SetBranchAddress("discriminator", &discriminator, &b_discriminator);
   fChain->SetBranchAddress("daughters_muonID", &daughters_muonID, &b_daughters_muonID);
   fChain->SetBranchAddress("daughters_typeOfMuon", &daughters_typeOfMuon, &b_daughters_typeOfMuon);
   fChain->SetBranchAddress("dxy", &dxy, &b_dxy);
   fChain->SetBranchAddress("dz", &dz, &b_dz);
   fChain->SetBranchAddress("dxy_innerTrack", &dxy_innerTrack, &b_dxy_innerTrack);
   fChain->SetBranchAddress("dz_innerTrack", &dz_innerTrack, &b_dz_innerTrack);
   fChain->SetBranchAddress("daughters_rel_error_trackpt", &daughters_rel_error_trackpt, &b_daughters_rel_error_trackpt);
   fChain->SetBranchAddress("SIP", &SIP, &b_SIP);
   fChain->SetBranchAddress("daughters_iseleBDT", &daughters_iseleBDT, &b_daughters_iseleBDT);
   fChain->SetBranchAddress("daughters_iseleWP80", &daughters_iseleWP80, &b_daughters_iseleWP80);
   fChain->SetBranchAddress("daughters_iseleWP90", &daughters_iseleWP90, &b_daughters_iseleWP90);
   fChain->SetBranchAddress("daughters_eleMVAnt", &daughters_eleMVAnt, &b_daughters_eleMVAnt);
   fChain->SetBranchAddress("daughters_passConversionVeto", &daughters_passConversionVeto, &b_daughters_passConversionVeto);
   fChain->SetBranchAddress("daughters_eleMissingHits", &daughters_eleMissingHits, &b_daughters_eleMissingHits);
   fChain->SetBranchAddress("daughters_iseleChargeConsistent", &daughters_iseleChargeConsistent, &b_daughters_iseleChargeConsistent);
   fChain->SetBranchAddress("daughters_eleCUTID", &daughters_eleCUTID, &b_daughters_eleCUTID);
   fChain->SetBranchAddress("decayMode", &decayMode, &b_decayMode);
   fChain->SetBranchAddress("tauID", &tauID, &b_tauID);
   fChain->SetBranchAddress("combreliso", &combreliso, &b_combreliso);
   fChain->SetBranchAddress("daughters_IetaIeta", &daughters_IetaIeta, &b_daughters_IetaIeta);
   fChain->SetBranchAddress("daughters_hOverE", &daughters_hOverE, &b_daughters_hOverE);
   fChain->SetBranchAddress("daughters_deltaEtaSuperClusterTrackAtVtx", &daughters_deltaEtaSuperClusterTrackAtVtx, &b_daughters_deltaEtaSuperClusterTrackAtVtx);
   fChain->SetBranchAddress("daughters_deltaPhiSuperClusterTrackAtVtx", &daughters_deltaPhiSuperClusterTrackAtVtx, &b_daughters_deltaPhiSuperClusterTrackAtVtx);
   fChain->SetBranchAddress("daughters_IoEmIoP", &daughters_IoEmIoP, &b_daughters_IoEmIoP);
   fChain->SetBranchAddress("daughters_SCeta", &daughters_SCeta, &b_daughters_SCeta);
   fChain->SetBranchAddress("daughters_depositR03_tracker", &daughters_depositR03_tracker, &b_daughters_depositR03_tracker);
   fChain->SetBranchAddress("daughters_depositR03_ecal", &daughters_depositR03_ecal, &b_daughters_depositR03_ecal);
   fChain->SetBranchAddress("daughters_depositR03_hcal", &daughters_depositR03_hcal, &b_daughters_depositR03_hcal);
   fChain->SetBranchAddress("daughters_decayModeFindingOldDMs", &daughters_decayModeFindingOldDMs, &b_daughters_decayModeFindingOldDMs);
   fChain->SetBranchAddress("againstElectronMVA5category", &againstElectronMVA5category, &b_againstElectronMVA5category);
   fChain->SetBranchAddress("againstElectronMVA5raw", &againstElectronMVA5raw, &b_againstElectronMVA5raw);
   fChain->SetBranchAddress("byPileupWeightedIsolationRaw3Hits", &byPileupWeightedIsolationRaw3Hits, &b_byPileupWeightedIsolationRaw3Hits);
   fChain->SetBranchAddress("footprintCorrection", &footprintCorrection, &b_footprintCorrection);
   fChain->SetBranchAddress("neutralIsoPtSumWeight", &neutralIsoPtSumWeight, &b_neutralIsoPtSumWeight);
   fChain->SetBranchAddress("photonPtSumOutsideSignalCone", &photonPtSumOutsideSignalCone, &b_photonPtSumOutsideSignalCone);
   fChain->SetBranchAddress("daughters_decayModeFindingNewDMs", &daughters_decayModeFindingNewDMs, &b_daughters_decayModeFindingNewDMs);
   fChain->SetBranchAddress("daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits", &daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits, &b_daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits);
   fChain->SetBranchAddress("daughters_byIsolationMVA3oldDMwoLTraw", &daughters_byIsolationMVA3oldDMwoLTraw, &b_daughters_byIsolationMVA3oldDMwoLTraw);
   fChain->SetBranchAddress("daughters_byIsolationMVA3oldDMwLTraw", &daughters_byIsolationMVA3oldDMwLTraw, &b_daughters_byIsolationMVA3oldDMwLTraw);
   fChain->SetBranchAddress("daughters_byIsolationMVA3newDMwoLTraw", &daughters_byIsolationMVA3newDMwoLTraw, &b_daughters_byIsolationMVA3newDMwoLTraw);
   fChain->SetBranchAddress("daughters_byIsolationMVA3newDMwLTraw", &daughters_byIsolationMVA3newDMwLTraw, &b_daughters_byIsolationMVA3newDMwLTraw);
   fChain->SetBranchAddress("daughters_byIsolationMVArun2v1DBoldDMwLTraw", &daughters_byIsolationMVArun2v1DBoldDMwLTraw, &b_daughters_byIsolationMVArun2v1DBoldDMwLTraw);
   fChain->SetBranchAddress("daughters_chargedIsoPtSum", &daughters_chargedIsoPtSum, &b_daughters_chargedIsoPtSum);
   fChain->SetBranchAddress("daughters_neutralIsoPtSum", &daughters_neutralIsoPtSum, &b_daughters_neutralIsoPtSum);
   fChain->SetBranchAddress("daughters_puCorrPtSum", &daughters_puCorrPtSum, &b_daughters_puCorrPtSum);
   fChain->SetBranchAddress("daughters_numChargedParticlesSignalCone", &daughters_numChargedParticlesSignalCone, &b_daughters_numChargedParticlesSignalCone);
   fChain->SetBranchAddress("daughters_numNeutralHadronsSignalCone", &daughters_numNeutralHadronsSignalCone, &b_daughters_numNeutralHadronsSignalCone);
   fChain->SetBranchAddress("daughters_numPhotonsSignalCone", &daughters_numPhotonsSignalCone, &b_daughters_numPhotonsSignalCone);
   fChain->SetBranchAddress("daughters_daughters_numParticlesSignalCone", &daughters_daughters_numParticlesSignalCone, &b_daughters_daughters_numParticlesSignalCone);
   fChain->SetBranchAddress("daughters_numChargedParticlesIsoCone", &daughters_numChargedParticlesIsoCone, &b_daughters_numChargedParticlesIsoCone);
   fChain->SetBranchAddress("daughters_numNeutralHadronsIsoCone", &daughters_numNeutralHadronsIsoCone, &b_daughters_numNeutralHadronsIsoCone);
   fChain->SetBranchAddress("daughters_numPhotonsIsoCone", &daughters_numPhotonsIsoCone, &b_daughters_numPhotonsIsoCone);
   fChain->SetBranchAddress("daughters_numParticlesIsoCone", &daughters_numParticlesIsoCone, &b_daughters_numParticlesIsoCone);
   fChain->SetBranchAddress("daughters_leadChargedParticlePt", &daughters_leadChargedParticlePt, &b_daughters_leadChargedParticlePt);
   fChain->SetBranchAddress("daughters_trackRefPt", &daughters_trackRefPt, &b_daughters_trackRefPt);
   fChain->SetBranchAddress("daughters_isLastTriggerObjectforPath", &daughters_isLastTriggerObjectforPath, &b_daughters_isLastTriggerObjectforPath);
   fChain->SetBranchAddress("daughters_isTriggerObjectforPath", &daughters_isTriggerObjectforPath, &b_daughters_isTriggerObjectforPath);
   fChain->SetBranchAddress("daughters_FilterFired", &daughters_FilterFired, &b_daughters_FilterFired);
   fChain->SetBranchAddress("daughters_isGoodTriggerType", &daughters_isGoodTriggerType, &b_daughters_isGoodTriggerType);
   fChain->SetBranchAddress("daughters_L3FilterFired", &daughters_L3FilterFired, &b_daughters_L3FilterFired);
   fChain->SetBranchAddress("daughters_L3FilterFiredLast", &daughters_L3FilterFiredLast, &b_daughters_L3FilterFiredLast);
   fChain->SetBranchAddress("daughters_HLTpt", &daughters_HLTpt, &b_daughters_HLTpt);
   fChain->SetBranchAddress("daughters_isL1IsoTau28Matched", &daughters_isL1IsoTau28Matched, &b_daughters_isL1IsoTau28Matched);
   fChain->SetBranchAddress("daughters_jetNDauChargedMVASel", &daughters_jetNDauChargedMVASel, &b_daughters_jetNDauChargedMVASel);
   fChain->SetBranchAddress("daughters_miniRelIsoCharged", &daughters_miniRelIsoCharged, &b_daughters_miniRelIsoCharged);
   fChain->SetBranchAddress("daughters_miniRelIsoNeutral", &daughters_miniRelIsoNeutral, &b_daughters_miniRelIsoNeutral);
   fChain->SetBranchAddress("daughters_jetPtRel", &daughters_jetPtRel, &b_daughters_jetPtRel);
   fChain->SetBranchAddress("daughters_jetPtRatio", &daughters_jetPtRatio, &b_daughters_jetPtRatio);
   fChain->SetBranchAddress("daughters_jetBTagCSV", &daughters_jetBTagCSV, &b_daughters_jetBTagCSV);
   fChain->SetBranchAddress("daughters_lepMVA_mvaId", &daughters_lepMVA_mvaId, &b_daughters_lepMVA_mvaId);
   fChain->SetBranchAddress("daughters_pca_x", &daughters_pca_x, &b_daughters_pca_x);
   fChain->SetBranchAddress("daughters_pca_y", &daughters_pca_y, &b_daughters_pca_y);
   fChain->SetBranchAddress("daughters_pca_z", &daughters_pca_z, &b_daughters_pca_z);
   fChain->SetBranchAddress("daughters_pcaRefitPV_x", &daughters_pcaRefitPV_x, &b_daughters_pcaRefitPV_x);
   fChain->SetBranchAddress("daughters_pcaRefitPV_y", &daughters_pcaRefitPV_y, &b_daughters_pcaRefitPV_y);
   fChain->SetBranchAddress("daughters_pcaRefitPV_z", &daughters_pcaRefitPV_z, &b_daughters_pcaRefitPV_z);
   fChain->SetBranchAddress("daughters_pcaGenPV_x", &daughters_pcaGenPV_x, &b_daughters_pcaGenPV_x);
   fChain->SetBranchAddress("daughters_pcaGenPV_y", &daughters_pcaGenPV_y, &b_daughters_pcaGenPV_y);
   fChain->SetBranchAddress("daughters_pcaGenPV_z", &daughters_pcaGenPV_z, &b_daughters_pcaGenPV_z);
   fChain->SetBranchAddress("JetsNumber", &JetsNumber, &b_JetsNumber);
   fChain->SetBranchAddress("jets_px", &jets_px, &b_jets_px);
   fChain->SetBranchAddress("jets_py", &jets_py, &b_jets_py);
   fChain->SetBranchAddress("jets_pz", &jets_pz, &b_jets_pz);
   fChain->SetBranchAddress("jets_e", &jets_e, &b_jets_e);
   fChain->SetBranchAddress("jets_rawPt", &jets_rawPt, &b_jets_rawPt);
   fChain->SetBranchAddress("jets_area", &jets_area, &b_jets_area);
   fChain->SetBranchAddress("jets_mT", &jets_mT, &b_jets_mT);
   fChain->SetBranchAddress("jets_Flavour", &jets_Flavour, &b_jets_Flavour);
   fChain->SetBranchAddress("jets_HadronFlavour", &jets_HadronFlavour, &b_jets_HadronFlavour);
   fChain->SetBranchAddress("jets_genjetIndex", &jets_genjetIndex, &b_jets_genjetIndex);
   fChain->SetBranchAddress("jets_PUJetID", &jets_PUJetID, &b_jets_PUJetID);
   fChain->SetBranchAddress("jets_PUJetIDupdated", &jets_PUJetIDupdated, &b_jets_PUJetIDupdated);
   fChain->SetBranchAddress("jets_vtxPt", &jets_vtxPt, &b_jets_vtxPt);
   fChain->SetBranchAddress("jets_vtxMass", &jets_vtxMass, &b_jets_vtxMass);
   fChain->SetBranchAddress("jets_vtx3dL", &jets_vtx3dL, &b_jets_vtx3dL);
   fChain->SetBranchAddress("jets_vtxNtrk", &jets_vtxNtrk, &b_jets_vtxNtrk);
   fChain->SetBranchAddress("jets_vtx3deL", &jets_vtx3deL, &b_jets_vtx3deL);
   fChain->SetBranchAddress("jets_leadTrackPt", &jets_leadTrackPt, &b_jets_leadTrackPt);
   fChain->SetBranchAddress("jets_leptonPtRel", &jets_leptonPtRel, &b_jets_leptonPtRel);
   fChain->SetBranchAddress("jets_leptonPt", &jets_leptonPt, &b_jets_leptonPt);
   fChain->SetBranchAddress("jets_leptonDeltaR", &jets_leptonDeltaR, &b_jets_leptonDeltaR);
   fChain->SetBranchAddress("jets_chEmEF", &jets_chEmEF, &b_jets_chEmEF);
   fChain->SetBranchAddress("jets_chHEF", &jets_chHEF, &b_jets_chHEF);
   fChain->SetBranchAddress("jets_nEmEF", &jets_nEmEF, &b_jets_nEmEF);
   fChain->SetBranchAddress("jets_nHEF", &jets_nHEF, &b_jets_nHEF);
   fChain->SetBranchAddress("jets_chMult", &jets_chMult, &b_jets_chMult);
   fChain->SetBranchAddress("jets_jecUnc", &jets_jecUnc, &b_jets_jecUnc);
   fChain->SetBranchAddress("bDiscriminator", &bDiscriminator, &b_bDiscriminator);
   fChain->SetBranchAddress("bCSVscore", &bCSVscore, &b_bCSVscore);
   fChain->SetBranchAddress("PFjetID", &PFjetID, &b_PFjetID);
   fChain->SetBranchAddress("jetRawf", &jetRawf, &b_jetRawf);
   fChain->SetBranchAddress("ak8jets_px", &ak8jets_px, &b_ak8jets_px);
   fChain->SetBranchAddress("ak8jets_py", &ak8jets_py, &b_ak8jets_py);
   fChain->SetBranchAddress("ak8jets_pz", &ak8jets_pz, &b_ak8jets_pz);
   fChain->SetBranchAddress("ak8jets_e", &ak8jets_e, &b_ak8jets_e);
   fChain->SetBranchAddress("ak8jets_SoftDropMass", &ak8jets_SoftDropMass, &b_ak8jets_SoftDropMass);
   fChain->SetBranchAddress("ak8jets_PrunedMass", &ak8jets_PrunedMass, &b_ak8jets_PrunedMass);
   fChain->SetBranchAddress("ak8jets_TrimmedMass", &ak8jets_TrimmedMass, &b_ak8jets_TrimmedMass);
   fChain->SetBranchAddress("ak8jets_FilteredMass", &ak8jets_FilteredMass, &b_ak8jets_FilteredMass);
   fChain->SetBranchAddress("ak8jets_tau1", &ak8jets_tau1, &b_ak8jets_tau1);
   fChain->SetBranchAddress("ak8jets_tau2", &ak8jets_tau2, &b_ak8jets_tau2);
   fChain->SetBranchAddress("ak8jets_tau3", &ak8jets_tau3, &b_ak8jets_tau3);
   fChain->SetBranchAddress("ak8jets_CSV", &ak8jets_CSV, &b_ak8jets_CSV);
   fChain->SetBranchAddress("ak8jets_nsubjets", &ak8jets_nsubjets, &b_ak8jets_nsubjets);
   fChain->SetBranchAddress("subjets_px", &subjets_px, &b_subjets_px);
   fChain->SetBranchAddress("subjets_py", &subjets_py, &b_subjets_py);
   fChain->SetBranchAddress("subjets_pz", &subjets_pz, &b_subjets_pz);
   fChain->SetBranchAddress("subjets_e", &subjets_e, &b_subjets_e);
   fChain->SetBranchAddress("subjets_CSV", &subjets_CSV, &b_subjets_CSV);
   fChain->SetBranchAddress("subjets_ak8MotherIdx", &subjets_ak8MotherIdx, &b_subjets_ak8MotherIdx);   
   fChain->SetBranchAddress("pv_x", &pv_x, &b_pv_x);
   fChain->SetBranchAddress("pv_y", &pv_y, &b_pv_y);
   fChain->SetBranchAddress("pv_z", &pv_z, &b_pv_z);
   fChain->SetBranchAddress("pvRefit_x", &pvRefit_x, &b_pvRefit_x);
   fChain->SetBranchAddress("pvRefit_y", &pvRefit_y, &b_pvRefit_y);
   fChain->SetBranchAddress("pvRefit_z", &pvRefit_z, &b_pvRefit_z);
   fChain->SetBranchAddress("pvGen_x", &pvGen_x, &b_pvGen_x);
   fChain->SetBranchAddress("pvGen_y", &pvGen_y, &b_pvGen_y);
   fChain->SetBranchAddress("pvGen_z", &pvGen_z, &b_pvGen_z);
   fChain->SetBranchAddress("isRefitPV", &isRefitPV, &b_isRefitPV);
   Notify();
}

Bool_t HTauTauTreeBase::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void HTauTauTreeBase::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void HTauTauTreeBase::initWawTree(TTree *tree, std::string prefix){
  
  if(prefix=="") prefix="WAW";
  prefix += "_";
  std::string filePath(tree->GetCurrentFile()->GetName());   
  size_t location = filePath.find_last_of("/");
  if(location==std::string::npos) location = 0;
  else location+=1;
  std::string fileName = prefix+filePath.substr(location,filePath.size());   
  warsawFile = new TFile(fileName.c_str(),"RECREATE");
  httEvent = new HTTEvent();
  warsawTree = new TTree("HTauTauTree","");
  warsawTree->SetDirectory(warsawFile);
  TBranch *eventBranch = warsawTree->Branch("HTTEvent.",&httEvent);
  TBranch *pairBranch = warsawTree->Branch("HTTPairCollection",&httPairCollection);
  TBranch *jetBranch = warsawTree->Branch("HTTJetCollection",&httJetCollection);
  TBranch *leptonBranch = warsawTree->Branch("HTTLeptonCollection",&httLeptonCollection);
  TBranch *genLeptonBranch = warsawTree->Branch("HTTGenLeptonCollection",&httGenLeptonCollection);
  hStats = new TH1F("hStats","Bookkeeping histogram",11,-0.5,10.5);
  hStats->SetDirectory(warsawFile);
  
  myScaleFactor.init_ScaleFactor("Muon_IsoMu22_eff.root");
   
  leptonPropertiesList.push_back("PDGIdDaughters");
  leptonPropertiesList.push_back("daughters_charge");
  leptonPropertiesList.push_back("decayMode");
  leptonPropertiesList.push_back("discriminator");
  leptonPropertiesList.push_back("daughters_muonID");
  leptonPropertiesList.push_back("daughters_typeOfMuon");
  leptonPropertiesList.push_back("daughters_byCombinedIsolationDeltaBetaCorrRaw3Hits");
  leptonPropertiesList.push_back("daughters_byIsolationMVArun2v1DBoldDMwLTraw");
  leptonPropertiesList.push_back("againstElectronMVA5category");
  leptonPropertiesList.push_back("dxy");
  leptonPropertiesList.push_back("dz");
  leptonPropertiesList.push_back("SIP");
  leptonPropertiesList.push_back("tauID");
  leptonPropertiesList.push_back("combreliso");
  leptonPropertiesList.push_back("daughters_leadChargedParticlePt");
  leptonPropertiesList.push_back("daughters_isGoodTriggerType");
  leptonPropertiesList.push_back("daughters_FilterFired");
  leptonPropertiesList.push_back("daughters_L3FilterFired");
  leptonPropertiesList.push_back("daughters_L3FilterFiredLast");
  leptonPropertiesList.push_back("mc_match");
  
  leptonPropertiesList.push_back("jets_rawPt");
  leptonPropertiesList.push_back("jets_area");
  leptonPropertiesList.push_back("jets_PUJetID");
  leptonPropertiesList.push_back("bDiscriminator");
  leptonPropertiesList.push_back("bCSVscore");
  leptonPropertiesList.push_back("PFjetID");
  ////////////////////////////////////////////////////////////
  ///Gen Lepton properties MUST be synchronized with lepton properties
  ///since the branches name are not uniform, we need a second names vector.
  genLeptonPropertiesList.push_back("genpart_pdg");
  genLeptonPropertiesList.push_back("genpart_pdg");
  genLeptonPropertiesList.push_back("genpart_TauGenDetailedDecayMode");

  return;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void HTauTauTreeBase::Loop(){

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntries();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      unsigned int bestPairIndex = Cut(ientry);
     
      fillEvent();
      
      hStats->Fill(0);//Number of events analyzed
      hStats->Fill(1,httEvent->getMCWeight());//Sum of weights
      
      bestPairIndex_ = bestPairIndex;

      if(bestPairIndex<9999){
	fillJets(bestPairIndex);
	fillLeptons();
	fillGenLeptons();
	fillPairs(bestPairIndex);

	warsawTree->Fill();
	hStats->Fill(2);//Number of events saved to ntuple
	hStats->Fill(3,httEvent->getMCWeight());//Sum of weights saved to ntuple
      }
   }

   writePropertiesHeader(leptonPropertiesList);

   TFile *currentFile = fChain->GetCurrentFile();   
   TH1F* hLLRCounters = (TH1F*)currentFile->Get("HTauTauTree/Counters");
   if(!hLLRCounters) std::cout<<"Counters histogram not found!"<<std::endl;
   else writeTriggersHeader(hLLRCounters);

}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
Int_t HTauTauTreeBase::Cut(Long64_t entry){

  if(!mothers_px->size()) return 9999;

  vector<unsigned int> pairIndexes;
  for(unsigned int iPair=0;iPair<mothers_px->size();++iPair){
    if(pairSelection(iPair)) pairIndexes.push_back(iPair);
  }

  ///Pair are already sorted during the ntuple creation
  if(pairIndexes.size()) return pairIndexes[0];
  
  return 9999;
};
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauTauTreeBase::pairSelection(unsigned int iPair){

  ///Requires channel specific implementation based on the following
  ///Baseline+post sync selection as on
  ///https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorking2015#Baseline_mu_tau_h_AN1
  ///Indexes for multiplexed ID variables taken from   LLRHiggsTauTau/NtupleProducer/plugins/
  ///HTauTauNtuplizer.cc, MuFiller.cc, TauFiller.cc, EleFiller.cc

  return false;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauTauTreeBase::thirdLeptonVeto(unsigned int signalLeg1Index, unsigned int signalLeg2Index, int leptonPdg, double dRmin){

  TLorentzVector leg1P4(daughters_px->at(signalLeg1Index),
			daughters_py->at(signalLeg1Index),
			daughters_pz->at(signalLeg1Index),
			daughters_e->at(signalLeg1Index));
  TLorentzVector leg2P4(daughters_px->at(signalLeg2Index),
			daughters_py->at(signalLeg2Index),
			daughters_pz->at(signalLeg2Index),
			daughters_e->at(signalLeg2Index));
  
  for(unsigned int iLepton=0;iLepton<daughters_px->size();++iLepton){
    if(iLepton==signalLeg1Index || iLepton==signalLeg2Index) continue;
    TLorentzVector leptonP4(daughters_px->at(iLepton),
			    daughters_py->at(iLepton),
			    daughters_pz->at(iLepton),
			    daughters_e->at(iLepton));
    double dr = std::min(leg1P4.DeltaR(leptonP4),leg2P4.DeltaR(leptonP4));
    if(dr<dRmin) continue;
    if(leptonPdg == 13 && std::abs(PDGIdDaughters->at(iLepton))==leptonPdg && muonSelection(iLepton) && 
       (std::abs(PDGIdDaughters->at(signalLeg1Index))!=leptonPdg || muonSelection(signalLeg1Index)) &&
       (std::abs(PDGIdDaughters->at(signalLeg2Index))!=leptonPdg || muonSelection(signalLeg2Index)) ) return true;
    if(leptonPdg == 11 && std::abs(PDGIdDaughters->at(iLepton))==leptonPdg && electronSelection(iLepton) &&
       (std::abs(PDGIdDaughters->at(signalLeg1Index))!=leptonPdg || electronSelection(signalLeg1Index)) &&
       (std::abs(PDGIdDaughters->at(signalLeg2Index))!=leptonPdg || electronSelection(signalLeg2Index)) ) return true;
  }
  return false;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauTauTreeBase::extraMuonVeto(unsigned int signalLeg1Index, unsigned int signalLeg2Index, double dRmin){
  return thirdLeptonVeto(signalLeg1Index,signalLeg2Index,13,dRmin);
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauTauTreeBase::extraElectronVeto(unsigned int signalLeg1Index, unsigned int signalLeg2Index, double dRmin){
  return thirdLeptonVeto(signalLeg1Index,signalLeg2Index,11,dRmin);
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauTauTreeBase::muonSelection(unsigned int index){

  TLorentzVector aP4(daughters_px->at(index),
		     daughters_py->at(index),
		     daughters_pz->at(index),
		     daughters_e->at(index));
  
  bool passSelection = aP4.Pt()>10 && std::abs(aP4.Eta())<2.4 &&
                       std::abs(dz->at(index))<0.2 &&
                       std::abs(dxy->at(index))<0.045 &&
		       ((daughters_muonID->at(index) & (1<<6)) == (1<<6)) &&
		       combreliso->at(index)<0.3;

  return passSelection;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauTauTreeBase::electronSelection(unsigned int index){

  TLorentzVector aP4(daughters_px->at(index),
		     daughters_py->at(index),
		     daughters_pz->at(index),
		     daughters_e->at(index));

  bool passSelection = aP4.Pt()>10 && std::abs(aP4.Eta())<2.5 &&
		       std::abs(dz->at(index))<0.2 &&
		       std::abs(dxy->at(index))<0.045 &&
		       daughters_iseleWP90->at(index)>0.5 &&
                       daughters_passConversionVeto->at(index)>0.5 &&
                       daughters_eleMissingHits->at(index)<=1 &&
                       combreliso->at(index)<0.3;
		
  return passSelection;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauTauTreeBase::jetSelection(unsigned int index, unsigned int bestPairIndex){

  TLorentzVector aP4(jets_px->at(index),
		     jets_py->at(index),
		     jets_pz->at(index),
		     jets_e->at(index));
		     
  TLorentzVector leg1P4(daughters_px->at(indexDau1->at(bestPairIndex)),
			daughters_py->at(indexDau1->at(bestPairIndex)),
			daughters_pz->at(indexDau1->at(bestPairIndex)),
			daughters_e->at(indexDau1->at(bestPairIndex)));
		     
  TLorentzVector leg2P4(daughters_px->at(indexDau2->at(bestPairIndex)),
			daughters_py->at(indexDau2->at(bestPairIndex)),
			daughters_pz->at(indexDau2->at(bestPairIndex)),
			daughters_e->at(indexDau2->at(bestPairIndex)));

  bool passSelection = aP4.Pt()>20 && std::abs(aP4.Eta())<4.7 &&	      
  		       aP4.DeltaR(leg1P4) > 0.5 &&	      
  		       aP4.DeltaR(leg2P4) > 0.5 &&
		       PFjetID->at(index)>=1;

  return passSelection;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void HTauTauTreeBase::fillEvent(){

  httEvent->clear();
  httEvent->setRun(RunNumber);
  httEvent->setEvent(EventNumber);
  httEvent->setNPV(npv);

  httEvent->setAODPV(TVector3(pv_x,pv_y,pv_z));
  httEvent->setRefittedPV(TVector3(pvRefit_x,pvRefit_y,pvRefit_z));
  httEvent->setIsRefit(isRefitPV);
  
  TVector2 metPF;
  metPF.SetMagPhi(met, metphi);
  httEvent->setMET(metPF);

  if(genpart_pdg){
    httEvent->setNPU(npu);
    httEvent->setMCWeight(MC_weight);
    httEvent->setMCatNLOWeight(aMCatNLOweight);
    httEvent->setLHE_Ht(lheHt);
    httEvent->setLHEnOutPartons(lheNOutPartons);    
    httEvent->setGenPV(TVector3(pvGen_x,pvGen_y,pvGen_z));    
  }

  if(genpart_pdg){
    for(unsigned int iGenPart=0;iGenPart<genpart_pdg->size();++iGenPart){
      int absPDGId = std::abs(genpart_pdg->at(iGenPart));
      if(absPDGId == 25 || absPDGId == 23 || absPDGId == 36) httEvent->setDecayModeBoson(genpart_HZDecayMode->at(iGenPart));
      if(absPDGId == 24) httEvent->setDecayModeBoson(genpart_WDecayMode->at(iGenPart));
      if(genpart_pdg->at(iGenPart)==15) httEvent->setDecayModeMinus(genpart_TauGenDecayMode->at(iGenPart));
      if(genpart_pdg->at(iGenPart)==-15) httEvent->setDecayModePlus(genpart_TauGenDecayMode->at(iGenPart));
    }
  }

  std::string fileName(fChain->GetCurrentFile()->GetName());  
  HTTEvent::sampleTypeEnum aType = HTTEvent::DUMMY;
  if(fileName.find("Run20")!=std::string::npos) aType = HTTEvent::DATA;
  else if(fileName.find("DY")!=std::string::npos && fileName.find("JetsToLL")!=std::string::npos) aType =  HTTEvent::DY;
  else if(fileName.find("W")!=std::string::npos && fileName.find("JetsToLNu")!=std::string::npos) aType =  HTTEvent::WJets;
  else if(fileName.find("TT_")!=std::string::npos) aType =  HTTEvent::TTbar;
  else if(fileName.find("HToTauTau_M")!=std::string::npos) aType =  HTTEvent::H;
  else if(fileName.find("SUSYGluGluToHToTauTau")!=std::string::npos) aType =  HTTEvent::A;
  httEvent->setSampleType(aType);

}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void HTauTauTreeBase::fillJets(unsigned int bestPairIndex){

  httJetCollection.clear();

  for(unsigned int iJet=0;iJet<jets_px->size();++iJet){

    if(!jetSelection(iJet, bestPairIndex)) continue;
    HTTParticle aJet;
    
    TLorentzVector p4(jets_px->at(iJet), jets_py->at(iJet),
		      jets_pz->at(iJet), jets_e->at(iJet));

    std::vector<float> aProperties = getProperties(leptonPropertiesList, iJet);
    aJet.setProperties(aProperties);

    aJet.setP4(p4);
    aJet.setProperties(aProperties);
    httJetCollection.push_back(aJet);
  }    
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void HTauTauTreeBase::fillLeptons(){

  httLeptonCollection.clear();
  
  for(unsigned int iLepton=0;iLepton<daughters_px->size();++iLepton){
    
    HTTParticle aLepton;
    
    TLorentzVector p4(daughters_px->at(iLepton), daughters_py->at(iLepton),
		      daughters_pz->at(iLepton), daughters_e->at(iLepton));

    TLorentzVector p4Charged(daughters_charged_px->at(iLepton), daughters_charged_py->at(iLepton),
			     daughters_charged_pz->at(iLepton), daughters_charged_e->at(iLepton));

    TLorentzVector p4Neutral(daughters_neutral_px->at(iLepton), daughters_neutral_py->at(iLepton),
			     daughters_neutral_pz->at(iLepton), daughters_neutral_e->at(iLepton));
    
    TLorentzVector p4ScaleUp(daughters_px_TauUp->at(iLepton), daughters_py_TauUp->at(iLepton),
			     daughters_pz_TauUp->at(iLepton), daughters_e_TauUp->at(iLepton));
  
    TLorentzVector p4ScaleDown(daughters_px_TauDown->at(iLepton), daughters_py_TauDown->at(iLepton),
			       daughters_pz_TauDown->at(iLepton), daughters_e_TauDown->at(iLepton));

    TVector3 pca(daughters_pca_x->at(iLepton), daughters_pca_y->at(iLepton), daughters_pca_z->at(iLepton));    
    TVector3 pcaRefitPV(daughters_pcaRefitPV_x->at(iLepton), daughters_pcaRefitPV_y->at(iLepton), daughters_pcaRefitPV_z->at(iLepton));    
    TVector3 pcaGenPV(daughters_pcaGenPV_x->at(iLepton), daughters_pcaGenPV_y->at(iLepton), daughters_pcaGenPV_z->at(iLepton));    

    aLepton.setP4(p4);
    aLepton.setChargedP4(p4Charged);
    aLepton.setNeutralP4(p4Neutral);
    
    aLepton.setP4ScaleUp(p4ScaleUp);
    aLepton.setP4ScaleDown(p4ScaleDown);
    
    aLepton.setPCA(pca);
    aLepton.setPCARefitPV(pcaRefitPV);
    aLepton.setPCAGenPV(pcaGenPV);
    
    std::vector<float> aProperties = getProperties(leptonPropertiesList, iLepton);
    aLepton.setProperties(aProperties);
    
    aLepton.setP4(p4);
    
    aLepton.setProperties(aProperties);
    httLeptonCollection.push_back(aLepton);
  }  
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void HTauTauTreeBase::fillGenLeptons(){

  httGenLeptonCollection.clear();
  
  if(!fChain->FindBranch("genpart_pdg")) return;
  
  for(unsigned int iGenPart=0;iGenPart<genpart_px->size();++iGenPart){
    if(std::abs(genpart_pdg->at(iGenPart))!=15) continue;
    
    TLorentzVector p4(genpart_px->at(iGenPart), genpart_py->at(iGenPart),
		      genpart_pz->at(iGenPart), genpart_e->at(iGenPart));
    
    HTTParticle aLepton;
    

    TVector3 pca(genpart_pca_x->at(iGenPart), genpart_pca_y->at(iGenPart), genpart_pca_z->at(iGenPart));

    aLepton.setP4(p4);
    aLepton.setChargedP4(getGenComponentP4(iGenPart,1));    
    aLepton.setNeutralP4(getGenComponentP4(iGenPart,0));
    aLepton.setPCA(pca);

    std::vector<float> aProperties = getProperties(genLeptonPropertiesList, iGenPart);
    aLepton.setProperties(aProperties);

    httGenLeptonCollection.push_back(aLepton); 
  }
  //std::cout<<EventNumber<<std::endl; 
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
TLorentzVector HTauTauTreeBase::getGenComponentP4(unsigned int index, unsigned int iAbsCharge){

  TLorentzVector aNeutralP4, aChargedP4, aHadronicP4, aLeptonP4;
    
  for(unsigned int iGenPart=0;iGenPart<genpart_px->size();++iGenPart){
    
    if((unsigned int)genpart_TauMothInd->at(iGenPart)!=index) continue;

    if(std::abs(genpart_pdg->at(iGenPart))==11 || std::abs(genpart_pdg->at(iGenPart))==13) aLeptonP4 =TLorentzVector(genpart_px->at(iGenPart),
													   genpart_py->at(iGenPart),
													   genpart_pz->at(iGenPart),
													   genpart_e->at(iGenPart));
    
    if(std::abs(genpart_pdg->at(iGenPart))==77715) aNeutralP4 =TLorentzVector(genpart_px->at(iGenPart),
									 genpart_py->at(iGenPart),
									 genpart_pz->at(iGenPart),
									 genpart_e->at(iGenPart));

    if(std::abs(genpart_pdg->at(iGenPart))==66615) aHadronicP4 =TLorentzVector(genpart_px->at(iGenPart),
									  genpart_py->at(iGenPart),
									  genpart_pz->at(iGenPart),
									  genpart_e->at(iGenPart));									     
  }

  TLorentzVector aP4;
  if(iAbsCharge==0) aP4 = aNeutralP4;
  else if(aHadronicP4.E()>0) aP4 = aHadronicP4 - aNeutralP4;
  else if(aLeptonP4.E()>0) aP4 = aLeptonP4;

  return aP4;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void HTauTauTreeBase::fillPairs(unsigned int bestPairIndex){

  httPairCollection.clear();

  for(unsigned int iPair=0;iPair<mothers_px->size();++iPair){
    if(iPair!=bestPairIndex) continue;
    
    TLorentzVector p4(mothers_px->at(iPair), mothers_py->at(iPair),
		      mothers_pz->at(iPair), mothers_e->at(iPair));

    TLorentzVector p4SVFit;
    p4SVFit.SetPtEtaPhiM(SVfit_pt->at(iPair), SVfit_eta->at(iPair),
			 SVfit_phi->at(iPair),SVfitMass->at(iPair));
    
    TLorentzVector p4SVFitScaleUp;
    p4SVFitScaleUp.SetPtEtaPhiM(SVfit_ptTauUp->at(iPair), SVfit_etaTauUp->at(iPair),
				SVfit_phiTauUp->at(iPair),SVfitMassTauUp->at(iPair));
    
    TLorentzVector p4SVFitScaleDown;    
    p4SVFitScaleDown.SetPtEtaPhiM(SVfit_ptTauDown->at(iPair), SVfit_etaTauDown->at(iPair),
				  SVfit_phiTauDown->at(iPair),SVfitMassTauDown->at(iPair));
    
    TVector2 met(METx->at(iPair), METy->at(iPair));
    TVector2 metSVfit;
    metSVfit.SetMagPhi(SVfit_fitMETRho->at(iPair),
		       SVfit_fitMETPhi->at(iPair));

    float mTLeg1 = mT_Dau1->at(iPair);
    float mTLeg2 = mT_Dau2->at(iPair);

    HTTPair aHTTpair;
    aHTTpair.setP4(p4);
    aHTTpair.setP4SVFit(p4SVFit);
    aHTTpair.setP4SVFitScaleUp(p4SVFitScaleUp);
    aHTTpair.setP4SVFitScaleDown(p4SVFitScaleDown);
    aHTTpair.setMET(met);
    aHTTpair.setMETMatrix(MET_cov00->at(iPair), MET_cov01->at(iPair), MET_cov10->at(iPair), MET_cov11->at(iPair));
    aHTTpair.setMETSVfit(metSVfit);
    aHTTpair.setMTLeg1(mTLeg1);
    aHTTpair.setMTLeg2(mTLeg2);    
    aHTTpair.setLeg1(httLeptonCollection.at(indexDau1->at(iPair)));
    aHTTpair.setLeg2(httLeptonCollection.at(indexDau2->at(iPair)));

    //MB the following is finall state specific and should be FIXED
    TLorentzVector muonP4 = aHTTpair.getMuon().getP4();
    float scaleFactor = 1.0;//SF for IsoMu22 not yet ready myScaleFactor.get_ScaleFactor(muonP4.Pt(),muonP4.Eta());
    aHTTpair.setMuonTriggerSF(scaleFactor);

    httPairCollection.push_back(aHTTpair);
  }
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
template<class T> T HTauTauTreeBase::getBranchValue(char *branchAddress, unsigned int index){

  vector<T> *aVector = *(vector<T> **)(branchAddress);

  if(aVector->size()<=index){
    //std::cout<<"Index - size mismatch "<<std::endl;
    return 0;
  }
  return aVector->at(index);  
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
float HTauTauTreeBase::getProperty(std::string name, unsigned int index){

  if(name=="mc_match") return getMCMatching(index);
  TBranch *branch = fChain->GetBranch(name.c_str());
  if(!branch){
    std::cout<<"Branch: "<<name<<" not found in the TTree."<<std::endl;
    return 0;
  }

  char *branchAddress = branch->GetAddress();
  std::string branchClass(branch->GetClassName());

  if(branchClass=="vector<float>") return getBranchValue<float>(branchAddress, index);
  if(branchClass=="vector<int>") return getBranchValue<int>(branchAddress, index);
  if(branchClass=="vector<bool>") return getBranchValue<bool>(branchAddress, index);
  if(branchClass=="vector<Long64_t>") return getBranchValue<Long64_t>(branchAddress, index);
  
  return 0;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void HTauTauTreeBase::writeTriggersHeader(const TH1F* hLLRCounter){

  ofstream outputFile("TriggerEnum.h");

  outputFile<<"enum class TriggerEnum { ";
  for(unsigned int iBinX=4;iBinX<(unsigned int)(hLLRCounter->GetNbinsX()+1);++iBinX){
    std::string name = hLLRCounter->GetXaxis()->GetBinLabel(iBinX);
    std::string pattern = "_v";
    if(name.find(pattern)!=std::string::npos) name.erase(name.find(pattern), pattern.size());    
    outputFile<<name<<" = "<<iBinX<<", "<<std::endl;
  }
  outputFile<<"NONE"<<" = "<<hLLRCounter->GetNbinsX()+1<<std::endl;
  outputFile<<"};"<<std::endl;

  outputFile.close();
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void HTauTauTreeBase::writePropertiesHeader(const std::vector<std::string> & propertiesList){

  ofstream outputFile("PropertyEnum.h");

  outputFile<<"enum class PropertyEnum { ";
  for(unsigned int iItem=0;iItem<propertiesList.size();++iItem){
    std::string name = propertiesList[iItem];
    std::string pattern = "daughters_";
    if(name.find(pattern)!=std::string::npos) name.erase(name.find(pattern), pattern.size());
    pattern = "Daughters";
    if(name.find(pattern)!=std::string::npos) name.erase(name.find(pattern), pattern.size());
    pattern = "jets_";
    if(name.find(pattern)!=std::string::npos) name.erase(name.find(pattern), pattern.size());
    outputFile<<name<<" = "<<iItem<<", "<<std::endl;
  }
  outputFile<<"NONE"<<" = "<<propertiesList.size()<<std::endl;
  outputFile<<"};"<<std::endl;
  outputFile.close();
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
std::vector<float> HTauTauTreeBase::getProperties(const std::vector<std::string> & propertiesList,
					      unsigned int index){

  std::vector<float> aProperties;
 
  for(auto propertyName:propertiesList){
    aProperties.push_back(getProperty(propertyName,index));
  }
  
  return aProperties;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
int HTauTauTreeBase::getMCMatching(unsigned int index){

  if(!fChain->FindBranch("genpart_pdg")) return -999;
  float dR = 100;
  unsigned int gen_ind = -1;
  if(index>=daughters_px->size()) return -999;
  
  TLorentzVector p4_1(daughters_px->at(index), daughters_py->at(index),
		      daughters_pz->at(index), daughters_e->at(index));
  
  for(unsigned int ind = 0; ind < genpart_px->size(); ind++) {

    if(!isGoodToMatch(ind)) continue;
    TLorentzVector p4_tmp(genpart_px->at(ind), genpart_py->at(ind),
		          genpart_pz->at(ind), genpart_e->at(ind));
    if(dR > p4_1.DeltaR(p4_tmp)) {dR = p4_1.DeltaR(p4_tmp); gen_ind = ind;}	
  }
  
  if((int)gen_ind == -1) return 6;
  
  TLorentzVector p4_2(genpart_px->at(gen_ind), genpart_py->at(gen_ind),
		      genpart_pz->at(gen_ind), genpart_e->at(gen_ind));
		      		      
  if(dR > 0.2) return 6;
  int genFlags = genpart_flags->at(gen_ind);
  int absPdgId = std::abs(genpart_pdg->at(gen_ind));
  if(absPdgId==66615){
    int motherTau_ind = genpart_TauMothInd->at(gen_ind);
    genFlags = genpart_flags->at(motherTau_ind);
  }
		      
  if(absPdgId == 11 && p4_2.Pt() > 8 && (genFlags & (1<<0)) == (1<<0)) return 1;
  if(absPdgId == 13 && p4_2.Pt() > 8 && (genFlags & (1<<0)) == (1<<0)) return 2;
  if(absPdgId == 11 && p4_2.Pt() > 8 && (genFlags & (1<<5)) == (1<<5)) return 3;
  if(absPdgId == 13 && p4_2.Pt() > 8 && (genFlags & (1<<5)) == (1<<5)) return 4;
  if(absPdgId == 66615 && p4_2.Pt() > 15 && (genFlags & (1<<0)) == (1<<0)) return 5;
  return 6;
  
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauTauTreeBase::isGoodToMatch(unsigned int ind){
	
	if(!(std::abs(genpart_pdg->at(ind))==11 || std::abs(genpart_pdg->at(ind))==13 || std::abs(genpart_pdg->at(ind))==66615)) return 0;
	if(std::abs(genpart_px->at(ind))<1E-3 && std::abs(genpart_py->at(ind))<1E-3) return 0;
	
	int genFlagsTmp = genpart_flags->at(ind);
	int absPdgIdTmp = std::abs(genpart_pdg->at(ind));
	
	if(absPdgIdTmp==66615){
	  int motherTau_indTmp = genpart_TauMothInd->at(ind);
	  genFlagsTmp = genpart_flags->at(motherTau_indTmp);
	}
	
	TLorentzVector p4_tmp(genpart_px->at(ind), genpart_py->at(ind),
				genpart_pz->at(ind), genpart_e->at(ind));
				
	bool isCat1 = absPdgIdTmp == 11 && p4_tmp.Pt() > 8 && (genFlagsTmp & (1<<0)) == (1<<0);
	bool isCat2 = absPdgIdTmp == 13 && p4_tmp.Pt() > 8 && (genFlagsTmp & (1<<0)) == (1<<0);
	bool isCat3 = absPdgIdTmp == 11 && p4_tmp.Pt() > 8 && (genFlagsTmp & (1<<5)) == (1<<5);
	bool isCat4 = absPdgIdTmp == 13 && p4_tmp.Pt() > 8 && (genFlagsTmp & (1<<5)) == (1<<5);
	bool isCat5 = absPdgIdTmp == 66615 && p4_tmp.Pt() > 15 && (genFlagsTmp & (1<<0)) == (1<<0);
	
	if(!(isCat1 || isCat2 || isCat3 || isCat4 || isCat5)) return 0;
	
	return 1;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
