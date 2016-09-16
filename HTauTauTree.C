#define HTauTauTree_cxx
#include "HTauTauTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TSystem.h>
#include "ScaleFactor.cc"
#include "HTTEvent.cxx"


#include <iostream>
#include <fstream>

void HTauTauTree::Loop(){

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

      if(bestPairIndex<9999){
	fillJets();
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
Int_t HTauTauTree::Cut(Long64_t entry){

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
bool HTauTauTree::pairSelection(unsigned int iPair){

  ///Baseline+post sync selection as on
  ///https://twiki.cern.ch/twiki/bin/view/CMS/HiggsToTauTauWorking2015#Baseline_mu_tau_h_AN1
  ///Indexes for multiplexed ID variables taken from   LLRHiggsTauTau/NtupleProducer/plugins/
  ///HTauTauNtuplizer.cc, MuFiller.cc, TauFiller.cc, EleFiller.cc

  if(!mothers_px->size()) return false;

  int pdgIdLeg1 = PDGIdDaughters->at(indexDau1->at(iPair));
  int pdgIdLeg2 = PDGIdDaughters->at(indexDau2->at(iPair));
  unsigned int indexMuonLeg = -1;
  if(abs(pdgIdLeg1)==13) indexMuonLeg = indexDau1->at(iPair);
  else if(abs(pdgIdLeg2)==13) indexMuonLeg = indexDau2->at(iPair);
  else return 0;

  unsigned int indexTauLeg = -1;
  if(abs(pdgIdLeg1)==15) indexTauLeg = indexDau1->at(iPair);
  else if(abs(pdgIdLeg2)==15) indexTauLeg = indexDau2->at(iPair);
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
			
  
  bool muonBaselineSelection =  muonP4.Perp()>23 && fabs(muonP4.Eta())<2.1 &&		//another condition for pt added because of https://github.com/CMS-HTT/2016-sync/blob/master/KIT/SUSYGluGluToHToTauTauM160_mt_RunIISpring16MiniAODv2_13TeV_MINIAOD.txt

			    fabs(dz->at(indexMuonLeg))<0.2 &&
			    fabs(dxy->at(indexMuonLeg))<0.045 &&
                            ((daughters_muonID->at(indexMuonLeg) & (1<<2)) == (1<<2));
                                			

  bool tauBaselineSelection = tauP4.Perp()>20 && fabs(tauP4.Eta())<2.3 &&
			      daughters_decayModeFindingOldDMs->at(indexTauLeg)>0.5 &&
                              fabs(dz->at(indexTauLeg))<0.2 && 
                              abs(daughters_charge->at(indexTauLeg))==1;			
                              				//another condition for pt added, because of: https://github.com/CMS-HTT/2016-sync/blob/master/KIT/SUSYGluGluToHToTauTauM160_mt_RunIISpring16MiniAODv2_13TeV_MINIAOD.txt
                              				//charge condition added

  bool baselinePair = muonP4.DeltaR(tauP4) > 0.5;								     
  bool postSynchMuon = combreliso->at(indexMuonLeg)<0.15;
  bool loosePostSynchMuon = combreliso->at(indexMuonLeg)<0.3;
  bool postSynchTau = (tauID->at(indexTauLeg) & tauIDmask) == tauIDmask;

  httEvent->setSelectionBit(SelectionBitsEnum::muonBaselineSelection,muonBaselineSelection);
  httEvent->setSelectionBit(SelectionBitsEnum::tauBaselineSelection,tauBaselineSelection);
  httEvent->setSelectionBit(SelectionBitsEnum::baselinePair,baselinePair);
  httEvent->setSelectionBit(SelectionBitsEnum::postSynchMuon,postSynchMuon);
  httEvent->setSelectionBit(SelectionBitsEnum::postSynchTau,postSynchTau);
  httEvent->setSelectionBit(SelectionBitsEnum::diMuonVeto,diMuonVeto());
  httEvent->setSelectionBit(SelectionBitsEnum::thirdLeptonVeto,thirdLeptonVeto(indexMuonLeg));
  
  /*
  std::cout<<" muonBaselineSelection: "<<muonBaselineSelection
	   <<" tauBaselineSelection: "<<tauBaselineSelection
	   <<" passBaselinePair: "<<passBaselinePair
	   <<" passPostSynchMuon: "<<passPostSynchMuon
	   <<" passPostSynchTau: "<<passPostSynchTau
	   <<" diMuonVeto(): "<<diMuonVeto()
	   <<" thirdLeptonVeto(indexMuonLeg): "<<thirdLeptonVeto(indexMuonLeg)
	   <<std::endl;
  */
  return muonBaselineSelection && tauBaselineSelection && baselinePair
    && postSynchTau && loosePostSynchMuon
    && diMuonVeto() && thirdLeptonVeto(indexMuonLeg)
    && true;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauTauTree::diMuonVeto(){
 
  std::vector<int> muonIndexes;
  for(unsigned int iLepton=0;iLepton<daughters_px->size();++iLepton){
    if(abs(PDGIdDaughters->at(iLepton))!=13) continue;

       TLorentzVector muonP4(daughters_px->at(iLepton),
			     daughters_py->at(iLepton),
			     daughters_pz->at(iLepton),
			     daughters_e->at(iLepton));

       bool passLepton = muonP4.Perp()> 15 && fabs(muonP4.Eta())<2.4 &&
			 fabs(dz->at(iLepton))<0.2 &&
		         fabs(dxy->at(iLepton))<0.045 && 
       combreliso->at(iLepton)<0.3 &&
       (daughters_typeOfMuon->at(iLepton) & ((1<<0) + (1<<1) + (1<<2)));

       if(passLepton) muonIndexes.push_back(iLepton);
  }

  if(muonIndexes.size()<2) return true;
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
	   deltaR>0.15) return false;
      }
    }
  }
  return true;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauTauTree::thirdLeptonVeto(unsigned int signalLeptonIndex){

  for(unsigned int iLepton=0;iLepton<daughters_px->size();++iLepton){
    if(iLepton==signalLeptonIndex) continue;
    if(abs(PDGIdDaughters->at(iLepton))==11 && electronSelection(iLepton)) return false;
    if(abs(PDGIdDaughters->at(iLepton))==13 && muonSelection(iLepton)) return false;
  }
  return true;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauTauTree::muonSelection(unsigned int index){

  TLorentzVector aP4(daughters_px->at(index),
		     daughters_py->at(index),
		     daughters_pz->at(index),
		     daughters_e->at(index));

  bool passSelection = aP4.Perp()>10 && fabs(aP4.Eta())<2.4 &&
		       fabs(dz->at(index))<0.2 &&
		       fabs(dxy->at(index))<0.045 &&
		      (daughters_muonID->at(index) & (1<<2)) && 	      
		       combreliso->at(index)<0.3;

  return passSelection;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool HTauTauTree::electronSelection(unsigned int index){

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
void HTauTauTree::fillEvent(){

  httEvent->clear();
  httEvent->setRun(RunNumber);
  httEvent->setEvent(EventNumber);
  httEvent->setNPV(npv);

  httEvent->setAODPV(TVector3(pv_x,pv_y,pv_z));
  httEvent->setRefittedPV(TVector3(pvRefit_x,pvRefit_y,pvRefit_z));
  httEvent->setIsRefit(isRefitPV);

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
  else if(fileName.find("DY")!=std::string::npos && fileName.find("JetsToLNu")!=std::string::npos) aType =  HTTEvent::DY;
  else if(fileName.find("W")!=std::string::npos && fileName.find("JetsToLNu")!=std::string::npos) aType =  HTTEvent::WJets;
  else if(fileName.find("TT_")!=std::string::npos) aType =  HTTEvent::TTbar;
  else if(fileName.find("HToTauTau_M")!=std::string::npos) aType =  HTTEvent::H;
  else if(fileName.find("SUSYGluGluToHToTauTau")!=std::string::npos) aType =  HTTEvent::A;    
  httEvent->setSampleType(aType);

}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void HTauTauTree::fillJets(){

  httJetCollection.clear();

  for(unsigned int iJet=0;iJet<jets_px->size();++iJet){

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
void HTauTauTree::fillLeptons(){

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
void HTauTauTree::fillGenLeptons(){

  httGenLeptonCollection.clear();
  
  if(!fChain->FindBranch("genpart_pdg")) return;
  
  for(unsigned int iGenPart=0;iGenPart<genpart_px->size();++iGenPart){
    if(abs(genpart_pdg->at(iGenPart))!=15) continue;
    
    HTTParticle aLepton;
    
    TLorentzVector p4(genpart_px->at(iGenPart), genpart_py->at(iGenPart),
		      genpart_pz->at(iGenPart), genpart_e->at(iGenPart));

    TVector3 pca(genpart_pca_x->at(iGenPart), genpart_pca_y->at(iGenPart), genpart_pca_z->at(iGenPart));

    aLepton.setP4(p4);
    aLepton.setChargedP4(getGenComponentP4(iGenPart,1));    
    aLepton.setNeutralP4(getGenComponentP4(iGenPart,0));
    aLepton.setPCA(pca);

    std::vector<float> aProperties = getProperties(genLeptonPropertiesList, iGenPart);
    aLepton.setProperties(aProperties);

    httGenLeptonCollection.push_back(aLepton); 
  } 
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
TLorentzVector HTauTauTree::getGenComponentP4(unsigned int index, unsigned int iAbsCharge){

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
void HTauTauTree::fillPairs(unsigned int bestPairIndex){

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
    aHTTpair.setMETSVfit(metSVfit);
    aHTTpair.setMTLeg1(mTLeg1);
    aHTTpair.setMTLeg2(mTLeg2);    
    aHTTpair.setLeg1(httLeptonCollection.at(indexDau1->at(iPair)));
    aHTTpair.setLeg2(httLeptonCollection.at(indexDau2->at(iPair)));

    TLorentzVector muonP4 = aHTTpair.getMuon().getP4();
    float scaleFactor = 1.0;//SF for IsoMu22 not yet ready myScaleFactor.get_ScaleFactor(muonP4.Pt(),muonP4.Eta());
    aHTTpair.setMuonTriggerSF(scaleFactor);

    httPairCollection.push_back(aHTTpair);
  }
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
template<class T> T HTauTauTree::getBranchValue(char *branchAddress, unsigned int index){

  vector<T> *aVector = *(vector<T> **)(branchAddress);

  if(aVector->size()<=index){
    //std::cout<<"Index - size mismatch "<<std::endl;
    return 0;
  }
  return aVector->at(index);  
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
float HTauTauTree::getProperty(std::string name, unsigned int index){

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
void HTauTauTree::writeTriggersHeader(const TH1F* hLLRCounter){

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
void HTauTauTree::writePropertiesHeader(const std::vector<std::string> & propertiesList){

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
std::vector<float> HTauTauTree::getProperties(const std::vector<std::string> & propertiesList,
					      unsigned int index){

  std::vector<float> aProperties;
 
  for(auto propertyName:propertiesList){
    aProperties.push_back(getProperty(propertyName,index));
  }
  
  return aProperties;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////

