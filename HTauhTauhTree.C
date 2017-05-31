#define HTauhTauhTree_cxx
#include "HTauhTauhTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TSystem.h>
//#include "ScaleFactor.cc"
//#include "HTTEvent.cxx"
//#include "HTauTauTreeBase.C"

#include <iostream>
#include <fstream>

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauhTauhTree::pairSelection(unsigned int iPair){

  ///Baseline+post sync selection as on
  ///https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorking2015#Baseline_mu_tau_h_AN1
  ///Indexes for multiplexed ID variables taken from   LLRHiggsTauTau/NtupleProducer/plugins/
  ///HTauTauNtuplizer.cc, MuFiller.cc, TauFiller.cc, EleFiller.cc

  if(!mothers_px->size()) return false;

  int pdgIdLeg1 = PDGIdDaughters->at(indexDau1->at(iPair));
  int pdgIdLeg2 = PDGIdDaughters->at(indexDau2->at(iPair));
  if( std::abs(pdgIdLeg1)!=15 || std::abs(pdgIdLeg2)!=15 ) return 0;
  unsigned int indexLeg1 = indexDau1->at(iPair);
  unsigned int indexLeg2 = indexDau2->at(iPair);

  int tauIDmask = 0;
  for(unsigned int iBit=0;iBit<ntauIds;iBit++){
    if(tauIDStrings[iBit]=="byVTightIsolationMVArun2v1DBoldDMwLT") tauIDmask |= (1<<iBit);
    if(tauIDStrings[iBit]=="againstMuonLoose3") tauIDmask |= (1<<iBit);
    if(tauIDStrings[iBit]=="againstElectronVLooseMVA6") tauIDmask |= (1<<iBit);
  }
  //MB sort taus within the pair
  double pt2_1 = (daughters_px->at(indexLeg1)*daughters_px->at(indexLeg1)+
		  daughters_py->at(indexLeg1)*daughters_py->at(indexLeg1));
  double pt2_2 = (daughters_px->at(indexLeg2)*daughters_px->at(indexLeg2)+
		  daughters_py->at(indexLeg2)*daughters_py->at(indexLeg2));
  if(pt2_2>pt2_1){//tau with higher-Pt first
    unsigned int indexLegTmp = indexLeg1;
    indexLeg1 = indexLeg2;
    indexLeg2 = indexLegTmp;    
  }
  TLorentzVector tau1P4(daughters_px->at(indexLeg1),
			daughters_py->at(indexLeg1),
			daughters_pz->at(indexLeg1),
			daughters_e->at(indexLeg1));


  TLorentzVector tau2P4(daughters_px->at(indexLeg2),
			daughters_py->at(indexLeg2),
			daughters_pz->at(indexLeg2),
			daughters_e->at(indexLeg2));

  bool tauBaselineSelection1 = tau1P4.Pt()>40 && std::abs(tau1P4.Eta())<2.1 &&
                               daughters_decayModeFindingOldDMs->at(indexLeg1)>0.5 &&
                               std::abs(dz->at(indexLeg1))<0.2 && 
                               std::abs(daughters_charge->at(indexLeg1))==1;			
  bool tauBaselineSelection2 = tau2P4.Pt()>40 && std::abs(tau2P4.Eta())<2.1 &&
                               daughters_decayModeFindingOldDMs->at(indexLeg2)>0.5 &&
                               std::abs(dz->at(indexLeg2))<0.2 && 
                               std::abs(daughters_charge->at(indexLeg2))==1;			
  
  bool baselinePair = tau1P4.DeltaR(tau2P4) > 0.5;							     
  bool postSynchTau1 = (tauID->at(indexLeg1) & tauIDmask) == tauIDmask;
  bool postSynchTau2 = (tauID->at(indexLeg2) & tauIDmask) == tauIDmask;
  ///
  bool triggerSelection = (triggerbit & 1<<0) == (1<<0);//MB FIXME
  
  httEvent->setSelectionBit(SelectionBitsEnum::muonBaselineSelection,tauBaselineSelection1);
  httEvent->setSelectionBit(SelectionBitsEnum::tauBaselineSelection,tauBaselineSelection2);
  httEvent->setSelectionBit(SelectionBitsEnum::baselinePair,baselinePair);
  httEvent->setSelectionBit(SelectionBitsEnum::postSynchMuon,postSynchTau1);
  httEvent->setSelectionBit(SelectionBitsEnum::postSynchTau,postSynchTau2);
  httEvent->setSelectionBit(SelectionBitsEnum::extraMuonVeto,thirdLeptonVeto(indexLeg1,indexLeg2,13));
  httEvent->setSelectionBit(SelectionBitsEnum::extraElectronVeto,thirdLeptonVeto(indexLeg1,indexLeg2,11));
  
  /*
  std::cout<<" tauBaselineSelection1: "<<tauBaselineSelection1
	   <<" tauBaselineSelection2: "<<tauBaselineSelection2
	   <<" passBaselinePair: "<<passBaselinePair
	   <<" passPostSynchTau1: "<<passPostSynchTau1
	   <<" passPostSynchTau2: "<<passPostSynchTau2
	   <<" thirdLeptonVeto(leg1,leg2): "<<thirdLeptonVeto(indexLeg1,indexLeg2)
	   <<std::endl;
  */
  return tauBaselineSelection1 && tauBaselineSelection2 && baselinePair
    //&& postSynchTau1 && postSynchTau2
    //&& !diMuonVeto() && !thirdLeptonVeto(indexMuonLeg) //FIXME
    //&& triggerSelection		//this is for the SM baseline selection for the VBF sample
    && true;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
