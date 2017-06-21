#define HTauTauTree_cxx
#include "HTauTauTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TSystem.h>

#include <iostream>
#include <fstream>

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauTauTree::pairSelection(unsigned int iPair){

  ///Baseline+post sync selection as on
  ///https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorking2015#Baseline_mu_tau_h_AN1
  ///Indexes for multiplexed ID variables taken from   LLRHiggsTauTau/NtupleProducer/plugins/
  ///HTauTauNtuplizer.cc, MuFiller.cc, TauFiller.cc, EleFiller.cc

  if(!mothers_px->size()) return false;
  
  bool isSynch = true;

  int pdgIdLeg1 = PDGIdDaughters->at(indexDau1->at(iPair));
  int pdgIdLeg2 = PDGIdDaughters->at(indexDau2->at(iPair));
  unsigned int indexMuonLeg = -1;
  if(std::abs(pdgIdLeg1)==13) indexMuonLeg = indexDau1->at(iPair);
  else if(std::abs(pdgIdLeg2)==13) indexMuonLeg = indexDau2->at(iPair);
  else return 0;

  unsigned int indexTauLeg = -1;
  if(std::abs(pdgIdLeg1)==15) indexTauLeg = indexDau1->at(iPair);
  else if(std::abs(pdgIdLeg2)==15) indexTauLeg = indexDau2->at(iPair);
  else return 0;

  int tauIDmask = 0;
  for(unsigned int iBit=0;iBit<ntauIds;iBit++){
    if(tauIDStrings[iBit]=="byTightIsolationMVArun2v1DBoldDMwLT") tauIDmask |= (1<<iBit);
    if(tauIDStrings[iBit]=="againstMuonTight3") tauIDmask |= (1<<iBit);
    if(tauIDStrings[iBit]=="againstElectronVLooseMVA6") tauIDmask |= (1<<iBit);
  }
  TLorentzVector muonP4(daughters_px->at(indexMuonLeg),
			daughters_py->at(indexMuonLeg),
			daughters_pz->at(indexMuonLeg),
			daughters_e->at(indexMuonLeg));

  TLorentzVector tauP4(daughters_px->at(indexTauLeg),
		       daughters_py->at(indexTauLeg),
		       daughters_pz->at(indexTauLeg),
		       daughters_e->at(indexTauLeg));

  int muonIdBit = 7;//Standard Medium ID
  if(RunNumber<278808 && RunNumber>100000) muonIdBit = 6;//ICHEP Medium MuonID

  bool muonBaselineSelection =  muonP4.Pt()>19 && std::abs(muonP4.Eta())<2.1 &&
				std::abs(dz->at(indexMuonLeg))<0.2 &&
				std::abs(dxy->at(indexMuonLeg))<0.045 &&
			       ((daughters_muonID->at(indexMuonLeg) & (1<<muonIdBit)) == (1<<muonIdBit));

  if(isSynch) muonBaselineSelection =  muonP4.Pt()>23 && std::abs(muonP4.Eta())<2.4 &&
				std::abs(dz->at(indexMuonLeg))<0.2 &&
				std::abs(dxy->at(indexMuonLeg))<0.045 &&
			       ((daughters_muonID->at(indexMuonLeg) & (1<<muonIdBit)) == (1<<muonIdBit));

  bool tauBaselineSelection = tauP4.Pt()>20 && std::abs(tauP4.Eta())<2.3 &&
			      daughters_decayModeFindingOldDMs->at(indexTauLeg)>0.5 &&
                              std::abs(dz->at(indexTauLeg))<0.2 &&
                              std::abs(daughters_charge->at(indexTauLeg))==1;

  bool baselinePair = muonP4.DeltaR(tauP4) > 0.5;
  bool postSynchMuon = combreliso->at(indexMuonLeg)<0.15;
  bool loosePostSynchMuon = combreliso->at(indexMuonLeg)<0.3;
  bool postSynchTau = (tauID->at(indexTauLeg) & tauIDmask) == tauIDmask;

  ///SUSY synch selection
  //muonBaselineSelection &= muonP4.Pt()>23 && std::abs(muonP4.Eta())<2.1;
  //tauBaselineSelection &= tauP4.Pt()>30 && std::abs(tauP4.Eta())<2.3;
  ///////////////////////

  httEvent->clearSelectionWord();
  httEvent->setSelectionBit(SelectionBitsEnum::muonBaselineSelection,muonBaselineSelection);
  httEvent->setSelectionBit(SelectionBitsEnum::tauBaselineSelection,tauBaselineSelection);
  httEvent->setSelectionBit(SelectionBitsEnum::baselinePair,baselinePair);
  httEvent->setSelectionBit(SelectionBitsEnum::postSynchMuon,postSynchMuon);
  httEvent->setSelectionBit(SelectionBitsEnum::postSynchTau,postSynchTau);
  httEvent->setSelectionBit(SelectionBitsEnum::diMuonVeto,diMuonVeto());
  httEvent->setSelectionBit(SelectionBitsEnum::extraMuonVeto,thirdLeptonVeto(indexMuonLeg, indexTauLeg, 13));
  httEvent->setSelectionBit(SelectionBitsEnum::extraElectronVeto,thirdLeptonVeto(indexMuonLeg, indexTauLeg, 11));

  if(isSynch) return muonBaselineSelection && tauBaselineSelection && baselinePair;
  
  return muonBaselineSelection && tauBaselineSelection && baselinePair
    && postSynchTau && loosePostSynchMuon
    && !diMuonVeto() && !thirdLeptonVeto(indexMuonLeg, indexTauLeg, 13) && !thirdLeptonVeto(indexMuonLeg, indexTauLeg, 11)
    && true;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauTauTree::diMuonVeto(){

  std::vector<int> muonIndexes;
  for(unsigned int iLepton=0;iLepton<daughters_px->size();++iLepton){

    if(std::abs(PDGIdDaughters->at(iLepton))!=13) continue;
       TLorentzVector muonP4(daughters_px->at(iLepton),
			     daughters_py->at(iLepton),
			     daughters_pz->at(iLepton),
			     daughters_e->at(iLepton));

       bool passLepton = muonP4.Pt()> 15 && std::abs(muonP4.Eta())<2.4 &&
			 std::abs(dz->at(iLepton))<0.2 &&
		         std::abs(dxy->at(iLepton))<0.045 &&
       combreliso->at(iLepton)<0.3 &&
       ((daughters_typeOfMuon->at(iLepton) & ((1<<0) + (1<<1) + (1<<2))) == ((1<<0) + (1<<1) + (1<<2)));

       if(passLepton) muonIndexes.push_back(iLepton);
  }

  if(muonIndexes.size()<2) return false;

  else{
    for(unsigned int iMuon1=0;iMuon1<muonIndexes.size();++iMuon1){
      TLorentzVector muon1P4(daughters_px->at(iMuon1),
			     daughters_py->at(iMuon1),
			     daughters_pz->at(iMuon1),
			     daughters_e->at(iMuon1));
      int muon1Charge = daughters_charge->at(iMuon1);

      for(unsigned int iMuon2=0;iMuon2<muonIndexes.size();++iMuon2){
	TLorentzVector muon2P4(daughters_px->at(iMuon2),
			       daughters_py->at(iMuon2),
			       daughters_pz->at(iMuon2),
			       daughters_e->at(iMuon2));
	int muon2Charge = daughters_charge->at(iMuon2);
	float deltaR = muon1P4.DeltaR(muon2P4);
	if(muon2Charge*muon1Charge==-1 &&
	   deltaR>0.15) return true;
      }
    }
  }
  return false;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
