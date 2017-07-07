#define HTauhTauhTree_cxx
#include "HTauhTauhTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <iostream>
#include <fstream>
#include <limits>

/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauhTauhTree::pairSelection(unsigned int iPair){

  ///Baseline+post sync selection as on
  ///https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorking2015#Baseline_mu_tau_h_AN1
  ///Indexes for multiplexed ID variables taken from   LLRHiggsTauTau/NtupleProducer/plugins/
  ///HTauTauNtuplizer.cc, MuFiller.cc, TauFiller.cc, EleFiller.cc

  if(!mothers_px->size()) return false;
  
  bool isSynch = false;

  int pdgIdLeg1 = PDGIdDaughters->at(indexDau1->at(iPair));
  int pdgIdLeg2 = PDGIdDaughters->at(indexDau2->at(iPair));
  if( std::abs(pdgIdLeg1)!=15 || std::abs(pdgIdLeg2)!=15 ) return 0;
  unsigned int indexLeg1 = indexDau1->at(iPair);
  unsigned int indexLeg2 = indexDau2->at(iPair);

  int tauIDmask = 0, tauIDmaskMedium = 0 , tauIDmaskLoose = 0;
  for(unsigned int iBit=0;iBit<ntauIds;iBit++){
    if(tauIDStrings[iBit]=="byTightIsolationMVArun2v1DBoldDMwLT")
      tauIDmask |= (1<<iBit);
    if(tauIDStrings[iBit]=="byMediumIsolationMVArun2v1DBoldDMwLT")
      tauIDmaskMedium |= (1<<iBit);
    if(tauIDStrings[iBit]=="byLooseIsolationMVArun2v1DBoldDMwLT")
      tauIDmaskLoose |= (1<<iBit);
    if(tauIDStrings[iBit]=="againstMuonLoose3") {
      tauIDmask |= (1<<iBit);
      tauIDmaskMedium |= (1<<iBit);
      tauIDmaskLoose |= (1<<iBit);
    }
    if(tauIDStrings[iBit]=="againstElectronVLooseMVA6") {
      tauIDmask |= (1<<iBit);
      tauIDmaskMedium |= (1<<iBit);
      tauIDmaskLoose |= (1<<iBit);
    }
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

  bool tauBaselineSelection1 = tau1P4.Pt()>50 && std::abs(tau1P4.Eta())<2.1 &&
                               daughters_decayModeFindingOldDMs->at(indexLeg1)>0.5 &&
                               std::abs(dz->at(indexLeg1))<0.2 &&
                               std::abs(daughters_charge->at(indexLeg1))==1;

  if(isSynch) tauBaselineSelection1 = tau1P4.Pt()>40 && std::abs(tau1P4.Eta())<2.1 &&
                               daughters_decayModeFindingOldDMs->at(indexLeg1)>0.5 &&
                               std::abs(dz->at(indexLeg1))<0.2 &&
                               std::abs(daughters_charge->at(indexLeg1))==1;
                               
  bool tauBaselineSelection2 = tau2P4.Pt()>40 && std::abs(tau2P4.Eta())<2.1 &&
                               daughters_decayModeFindingOldDMs->at(indexLeg2)>0.5 &&
                               std::abs(dz->at(indexLeg2))<0.2 &&
                               std::abs(daughters_charge->at(indexLeg2))==1;

  if(isSynch) tauBaselineSelection2 = tau2P4.Pt()>40 && std::abs(tau2P4.Eta())<2.1 &&
                               daughters_decayModeFindingOldDMs->at(indexLeg2)>0.5 &&
                               std::abs(dz->at(indexLeg2))<0.2 &&
                               std::abs(daughters_charge->at(indexLeg2))==1;

  bool baselinePair = tau1P4.DeltaR(tau2P4) > 0.5;
  bool postSynchTau1 = (tauID->at(indexLeg1) & tauIDmask) == tauIDmask;
  bool postSynchTau2 = (tauID->at(indexLeg2) & tauIDmask) == tauIDmask;
  ///
  bool postSynchLooseTau1 = (tauID->at(indexLeg1) & tauIDmaskLoose) == tauIDmaskLoose;
  bool postSynchLooseTau2 = (tauID->at(indexLeg2) & tauIDmaskLoose) == tauIDmaskLoose;
  bool postSynchMediumTau1 = (tauID->at(indexLeg1) & tauIDmaskMedium) == tauIDmaskMedium;
  bool postSynchMediumTau2 = (tauID->at(indexLeg2) & tauIDmaskMedium) == tauIDmaskMedium;

  httEvent->setSelectionBit(SelectionBitsEnum::muonBaselineSelection,tauBaselineSelection1);
  httEvent->setSelectionBit(SelectionBitsEnum::tauBaselineSelection,tauBaselineSelection2);
  httEvent->setSelectionBit(SelectionBitsEnum::baselinePair,baselinePair);
  httEvent->setSelectionBit(SelectionBitsEnum::postSynchMuon,postSynchTau1);
  httEvent->setSelectionBit(SelectionBitsEnum::postSynchTau,postSynchTau2);
  httEvent->setSelectionBit(SelectionBitsEnum::extraMuonVeto,thirdLeptonVeto(indexLeg1,indexLeg2,13));
  httEvent->setSelectionBit(SelectionBitsEnum::extraElectronVeto,thirdLeptonVeto(indexLeg1,indexLeg2,11));

  if(isSynch) return tauBaselineSelection1 && tauBaselineSelection2 && baselinePair;

  return tauBaselineSelection1 && tauBaselineSelection2 && baselinePair
    && ( (postSynchLooseTau1 && postSynchMediumTau2) || (postSynchLooseTau2 && postSynchMediumTau1) )
    && !thirdLeptonVeto(indexLeg1,indexLeg2,13)
    && !thirdLeptonVeto(indexLeg1,indexLeg2,11)
    && true;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
unsigned int HTauhTauhTree::bestPair(std::vector<unsigned int> &pairIndexes){

  unsigned int bestIndex = 9999;
  ///Pair are already sorted during the ntuple creation?
  double iso_1=std::numeric_limits<double>::infinity(), iso_2=std::numeric_limits<double>::infinity(), pt2_1=-1, pt2_2=-1;
  if(pairIndexes.size()) {
    //return pairIndexes[0];//MB
    for(unsigned int ii=0;ii<2*pairIndexes.size();++ii){
      unsigned int i=(ii<pairIndexes.size()?ii:ii-pairIndexes.size());
      unsigned int iPair = pairIndexes[i];
      unsigned int leg1Index = indexDau1->at(iPair);
      unsigned int leg2Index = indexDau2->at(iPair);
      if(ii>=pairIndexes.size()){//invert legs
	leg1Index = indexDau2->at(iPair);
	leg2Index = indexDau1->at(iPair);
      }
      double pt2_1_i = (daughters_px->at(leg1Index)*daughters_px->at(leg1Index)+
			daughters_py->at(leg1Index)*daughters_py->at(leg1Index));
      double pt2_2_i = (daughters_px->at(leg2Index)*daughters_px->at(leg2Index)+
			daughters_py->at(leg2Index)*daughters_py->at(leg2Index));
      //MB: More isolated for MVAIso means higher value so inverted here to keep standard convention in comparison
      double iso_1_i = -getProperty("daughters_byIsolationMVArun2v1DBoldDMwLTraw",leg1Index);
      double iso_2_i = -getProperty("daughters_byIsolationMVArun2v1DBoldDMwLTraw",leg2Index);

      if(iso_1_i>iso_1) continue;
      if(iso_1_i==iso_1 && pt2_1_i<pt2_1) continue;
      if(iso_2_i>iso_2) continue;
      if(iso_2_i==iso_2 && pt2_2_i<pt2_2) continue;
      bestIndex = iPair;
      iso_1 = iso_1_i;
      iso_2 = iso_2_i;
      pt2_1 = pt2_1_i;
      pt2_2 = pt2_2_i;
    }
  }
  /*
  if(pairIndexes.size() && bestIndex!=(Int_t)pairIndexes[0]){
    unsigned int iPair = pairIndexes[0];
    unsigned int leg1Index = indexDau1->at(iPair);
    unsigned int leg2Index = indexDau2->at(iPair);
    double pt2_1_i = (daughters_px->at(leg1Index)*daughters_px->at(leg1Index)+
		      daughters_py->at(leg1Index)*daughters_py->at(leg1Index));
    double pt2_2_i = (daughters_px->at(leg2Index)*daughters_px->at(leg2Index)+
		      daughters_py->at(leg2Index)*daughters_py->at(leg2Index));
    std::cout<<"Pair sorting: "<<std::endl
	     <<"best index = "<<bestIndex<<", index[0] = "<<pairIndexes[0]<<std::endl
	     <<"\tiso1[best]="<<-iso_1<<", iso1[0]="<<getProperty("daughters_byIsolationMVArun2v1DBoldDMwLTraw",leg1Index)<<std::endl
	     <<"\tpt1[best]="<<sqrt(pt2_1)<<", pt1[0]="<<sqrt(pt2_1_i)<<std::endl
	     <<"\tiso2[best]="<<-iso_2<<", iso1[0]="<<getProperty("daughters_byIsolationMVArun2v1DBoldDMwLTraw",leg2Index)<<std::endl
	     <<"\tpt2[best]="<<sqrt(pt2_2)<<", pt1[0]="<<sqrt(pt2_2_i)<<std::endl;
  }
  */

  return bestIndex;
};
/////////////////////////////////////////////////
/////////////////////////////////////////////////
