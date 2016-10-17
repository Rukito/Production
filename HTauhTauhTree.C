#define HTauhTauhTree_cxx
#include "HTauhTauhTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TSystem.h>
#include "ScaleFactor.cc"
#include "HTTEvent.cxx"


#include <iostream>
#include <fstream>

void HTauhTauhTree::Loop(){

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
Int_t HTauhTauhTree::Cut(Long64_t entry){

  if(!mothers_px->size()) return 9999;

  vector<unsigned int> pairIndexes;
  for(unsigned int iPair=0;iPair<mothers_px->size();++iPair){
    if(pairSelection(iPair)) pairIndexes.push_back(iPair);
    //MB can break loop after 1st pair found as anyway the first is always taken (see below)
  }

  ///Pair are already sorted during the ntuple creation
  if(pairIndexes.size()) return pairIndexes[0];
  
  return 9999;
};
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
  if( abs(pdgIdLeg1)!=15 || abs(pdgIdLeg2)!=15 ) return 0;
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

  bool tauBaselineSelection1 = tau1P4.Pt()>40 && fabs(tau1P4.Eta())<2.1 &&
                               daughters_decayModeFindingOldDMs->at(indexLeg1)>0.5 &&
                               fabs(dz->at(indexLeg1))<0.2 && 
                               abs(daughters_charge->at(indexLeg1))==1;			
  bool tauBaselineSelection2 = tau2P4.Pt()>40 && fabs(tau2P4.Eta())<2.1 &&
                               daughters_decayModeFindingOldDMs->at(indexLeg2)>0.5 &&
                               fabs(dz->at(indexLeg2))<0.2 && 
                               abs(daughters_charge->at(indexLeg2))==1;			
  
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
/*bool HTauhTauhTree::diMuonVeto(){
  return false;
  }*/
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauhTauhTree::thirdLeptonVeto(unsigned int signalMuonIndex, unsigned int signalTauIndex, int leptonPdg, double dRmin){

  TLorentzVector muonP4(daughters_px->at(signalMuonIndex),
			daughters_py->at(signalMuonIndex),
			daughters_pz->at(signalMuonIndex),
			daughters_e->at(signalMuonIndex));
  TLorentzVector tauP4(daughters_px->at(signalTauIndex),
		       daughters_py->at(signalTauIndex),
		       daughters_pz->at(signalTauIndex),
		       daughters_e->at(signalTauIndex));
      
  for(unsigned int iLepton=0;iLepton<daughters_px->size();++iLepton){
    if(iLepton==signalMuonIndex || iLepton==signalTauIndex) continue;
    TLorentzVector leptonP4(daughters_px->at(iLepton),
			    daughters_py->at(iLepton),
			    daughters_pz->at(iLepton),
			    daughters_e->at(iLepton));
    double dr = std::min(tauP4.DeltaR(leptonP4),muonP4.DeltaR(leptonP4));
    if(dr<dRmin) continue;
    if(leptonPdg == 13 && abs(PDGIdDaughters->at(iLepton))==leptonPdg && muonSelection(iLepton) && 
       (abs(PDGIdDaughters->at(signalMuonIndex))!=leptonPdg || muonSelection(signalMuonIndex)) &&
       (abs(PDGIdDaughters->at(signalTauIndex))!=leptonPdg || muonSelection(signalTauIndex)) ) return true;
    if(leptonPdg == 11 && abs(PDGIdDaughters->at(iLepton))==leptonPdg && electronSelection(iLepton) &&
       (abs(PDGIdDaughters->at(signalMuonIndex))!=leptonPdg || electronSelection(signalMuonIndex)) &&
       (abs(PDGIdDaughters->at(signalTauIndex))!=leptonPdg || electronSelection(signalMuonIndex)) ) return true;
  }
  return false;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauhTauhTree::extraMuonVeto(unsigned int signalMuonIndex, unsigned int signalTauIndex, double dRmin){
  return thirdLeptonVeto(signalMuonIndex,signalTauIndex,13,dRmin);
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauhTauhTree::extraElectronVeto(unsigned int signalMuonIndex, unsigned int signalTauIndex, double dRmin){
  return thirdLeptonVeto(signalMuonIndex,signalTauIndex,11,dRmin);
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauhTauhTree::muonSelection(unsigned int index){

  TLorentzVector aP4(daughters_px->at(index),
		     daughters_py->at(index),
		     daughters_pz->at(index),
		     daughters_e->at(index));

  bool passSelection = aP4.Perp()>10 && fabs(aP4.Eta())<2.4 &&
		       fabs(dz->at(index))<0.2 &&
		       fabs(dxy->at(index))<0.045 &&
		       ((daughters_muonID->at(index) & (1<<6)) == (1<<6)) && 	      
		       combreliso->at(index)<0.3;

  return passSelection;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauhTauhTree::electronSelection(unsigned int index){

  TLorentzVector aP4(daughters_px->at(index),
		     daughters_py->at(index),
		     daughters_pz->at(index),
		     daughters_e->at(index));

  bool passSelection = aP4.Perp()>10 && fabs(aP4.Eta())<2.5 &&
		       fabs(dz->at(index))<0.2 &&
		       fabs(dxy->at(index))<0.045 &&
		       daughters_iseleWP90->at(index)>0.5 &&
                       daughters_passConversionVeto->at(index)>0.5 &&
                       daughters_eleMissingHits->at(index)<=1 &&
                       combreliso->at(index)<0.3;
		
  return passSelection;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauhTauhTree::jetSelection(unsigned int index, unsigned int bestPairIndex){

  TLorentzVector aP4(jets_px->at(index),
		     jets_py->at(index),
		     jets_pz->at(index),
		     jets_e->at(index));
		     
  TLorentzVector muP4(daughters_px->at(indexDau1->at(bestPairIndex)),
		     daughters_py->at(indexDau1->at(bestPairIndex)),
		     daughters_pz->at(indexDau1->at(bestPairIndex)),
		     daughters_e->at(indexDau1->at(bestPairIndex)));
		     
  TLorentzVector tauP4(daughters_px->at(indexDau2->at(bestPairIndex)),
		     daughters_py->at(indexDau2->at(bestPairIndex)),
		     daughters_pz->at(indexDau2->at(bestPairIndex)),
		     daughters_e->at(indexDau2->at(bestPairIndex)));

  bool passSelection = aP4.Perp()>20 && fabs(aP4.Eta())<4.7 &&	      
  		       aP4.DeltaR(muP4) > 0.5 &&	      
  		       aP4.DeltaR(tauP4) > 0.5 &&
		       PFjetID->at(index)>=1;

  return passSelection;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void HTauhTauhTree::fillEvent(){

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
      int absPDGId = abs(genpart_pdg->at(iGenPart));
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
void HTauhTauhTree::fillJets(unsigned int bestPairIndex){

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
void HTauhTauhTree::fillLeptons(){

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
void HTauhTauhTree::fillGenLeptons(){

  httGenLeptonCollection.clear();
  
  if(!fChain->FindBranch("genpart_pdg")) return;
  
  for(unsigned int iGenPart=0;iGenPart<genpart_px->size();++iGenPart){
    if(abs(genpart_pdg->at(iGenPart))!=15) continue;
    
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
TLorentzVector HTauhTauhTree::getGenComponentP4(unsigned int index, unsigned int iAbsCharge){

  TLorentzVector aNeutralP4, aChargedP4, aHadronicP4, aLeptonP4;
    
  for(unsigned int iGenPart=0;iGenPart<genpart_px->size();++iGenPart){
    
    if((unsigned int)genpart_TauMothInd->at(iGenPart)!=index) continue;

    if(abs(genpart_pdg->at(iGenPart))==11 || abs(genpart_pdg->at(iGenPart))==13) aLeptonP4 =TLorentzVector(genpart_px->at(iGenPart),
													   genpart_py->at(iGenPart),
													   genpart_pz->at(iGenPart),
													   genpart_e->at(iGenPart));
    
    if(abs(genpart_pdg->at(iGenPart))==77715) aNeutralP4 =TLorentzVector(genpart_px->at(iGenPart),
									 genpart_py->at(iGenPart),
									 genpart_pz->at(iGenPart),
									 genpart_e->at(iGenPart));

    if(abs(genpart_pdg->at(iGenPart))==66615) aHadronicP4 =TLorentzVector(genpart_px->at(iGenPart),
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
void HTauhTauhTree::fillPairs(unsigned int bestPairIndex){

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
    
    double pt2_1 = (daughters_px->at(indexDau1->at(iPair))*daughters_px->at(indexDau1->at(iPair))+
		    daughters_py->at(indexDau1->at(iPair))*daughters_py->at(indexDau1->at(iPair)));
    double pt2_2 = (daughters_px->at(indexDau2->at(iPair))*daughters_px->at(indexDau2->at(iPair))+
		    daughters_py->at(indexDau2->at(iPair))*daughters_py->at(indexDau2->at(iPair)));
    float mTLeg1 = mT_Dau1->at(iPair);
    float mTLeg2 = mT_Dau2->at(iPair);
    unsigned int leg1Index = indexDau1->at(iPair);
    unsigned int leg2Index = indexDau2->at(iPair);
    if(pt2_2>pt2_1){//tau with higher-Pt first
      mTLeg1 = mT_Dau2->at(iPair);
      mTLeg2 = mT_Dau1->at(iPair);
      leg1Index = indexDau2->at(iPair);
      leg2Index = indexDau1->at(iPair);
    }
    
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
    aHTTpair.setLeg1(httLeptonCollection.at(leg1Index));
    aHTTpair.setLeg2(httLeptonCollection.at(leg2Index));
    
    TLorentzVector muonP4 = aHTTpair.getLeg1().getP4();
    float scaleFactor = 1.0;//SF for IsoMu22 not yet ready myScaleFactor.get_ScaleFactor(muonP4.Pt(),muonP4.Eta());//MB FIXME: set it for both taus requires changes in HTTPair definition 
    aHTTpair.setMuonTriggerSF(scaleFactor);
    
    httPairCollection.push_back(aHTTpair);
  }
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
template<class T> T HTauhTauhTree::getBranchValue(char *branchAddress, unsigned int index){

  vector<T> *aVector = *(vector<T> **)(branchAddress);

  if(aVector->size()<=index){
    //std::cout<<"Index - size mismatch "<<std::endl;
    return 0;
  }
  return aVector->at(index);  
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
float HTauhTauhTree::getProperty(std::string name, unsigned int index){

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
void HTauhTauhTree::writeTriggersHeader(const TH1F* hLLRCounter){

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
void HTauhTauhTree::writePropertiesHeader(const std::vector<std::string> & propertiesList){

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
std::vector<float> HTauhTauhTree::getProperties(const std::vector<std::string> & propertiesList,
					      unsigned int index){

  std::vector<float> aProperties;
 
  for(auto propertyName:propertiesList){
    aProperties.push_back(getProperty(propertyName,index));
  }
  
  return aProperties;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
int HTauhTauhTree::getMCMatching(unsigned int index){

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
  int absPdgId = abs(genpart_pdg->at(gen_ind));
  if(absPdgId==66615){
    int motherTau_ind = genpart_TauMothInd->at(gen_ind);
    genFlags = genpart_flags->at(motherTau_ind);
  }
		      
  if(absPdgId == 11 && p4_2.Perp() > 8 && (genFlags & (1<<0)) == (1<<0)) return 1;
  if(absPdgId == 13 && p4_2.Perp() > 8 && (genFlags & (1<<0)) == (1<<0)) return 2;
  if(absPdgId == 11 && p4_2.Perp() > 8 && (genFlags & (1<<5)) == (1<<5)) return 3;
  if(absPdgId == 13 && p4_2.Perp() > 8 && (genFlags & (1<<5)) == (1<<5)) return 4;
  if(absPdgId == 66615 && p4_2.Perp() > 15 && (genFlags & (1<<0)) == (1<<0)) return 5;
  return 6;
  
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauhTauhTree::isGoodToMatch(unsigned int ind){
	
	if(!(abs(genpart_pdg->at(ind))==11 || abs(genpart_pdg->at(ind))==13 || abs(genpart_pdg->at(ind))==66615)) return 0;
	if(fabs(genpart_px->at(ind))<1E-3 && fabs(genpart_py->at(ind))<1E-3) return 0;
	
	int genFlagsTmp = genpart_flags->at(ind);
	int absPdgIdTmp = abs(genpart_pdg->at(ind));
	
	if(absPdgIdTmp==66615){
	  int motherTau_indTmp = genpart_TauMothInd->at(ind);
	  genFlagsTmp = genpart_flags->at(motherTau_indTmp);
	}
	
	TLorentzVector p4_tmp(genpart_px->at(ind), genpart_py->at(ind),
				genpart_pz->at(ind), genpart_e->at(ind));
				
	bool isCat1 = absPdgIdTmp == 11 && p4_tmp.Perp() > 8 && (genFlagsTmp & (1<<0)) == (1<<0);
	bool isCat2 = absPdgIdTmp == 13 && p4_tmp.Perp() > 8 && (genFlagsTmp & (1<<0)) == (1<<0);
	bool isCat3 = absPdgIdTmp == 11 && p4_tmp.Perp() > 8 && (genFlagsTmp & (1<<5)) == (1<<5);
	bool isCat4 = absPdgIdTmp == 13 && p4_tmp.Perp() > 8 && (genFlagsTmp & (1<<5)) == (1<<5);
	bool isCat5 = absPdgIdTmp == 66615 && p4_tmp.Perp() > 15 && (genFlagsTmp & (1<<0)) == (1<<0);
	
	if(!(isCat1 || isCat2 || isCat3 || isCat4 || isCat5)) return 0;
	
	return 1;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
