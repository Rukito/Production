#!/usr/bin/env python

import os

from ROOT import gSystem, TChain, TSystem, TFile
from PSet import process

#Produce framework report required by CRAB
command = "cmsRun -j FrameworkJobReport.xml -p PSet.py"
os.system(command)

gSystem.CompileMacro('HTTEvent.cxx')
gSystem.CompileMacro('ScaleFactor.cc')
gSystem.CompileMacro('HTauTauTreeBase.C')
gSystem.CompileMacro('HTauTauTree.C')
from ROOT import HTauTauTree

fileNames = []
#aFile = "file:///scratch/cms/akalinow/CMS/HiggsCP/Prod/Crab/Production/SUSYGluGluToHToTauTau_M-160_TuneCUETP8M1_13TeV-pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/HTauTauAnalysis_TAUCUT_fix.root"
#aFile = "file:///export/cms/akalinow/CMS/HiggsCP/Prod/Crab/Production/SUSYGluGluToHToTauTau_M-160_TuneCUETP8M1_13TeV-pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/HTauTauAnalysis_MVAMET_fix.root"
aFile = "file:///export/cms/akalinow/CMS/HiggsCP/Prod/Crab/Production/VBFHToTauTau_M125_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM/HTauTauAnalysis_2.root"
fileNames.append(aFile)

print "Adding file: ",aFile
aROOTFile = TFile.Open(aFile)
aTree = aROOTFile.Get("HTauTauTree/HTauTauTree")
print "TTree entries: ",aTree.GetEntries()
HTauTauTree(aTree).Loop()
