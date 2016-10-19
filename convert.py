#!/usr/bin/env python

import os

from ROOT import gSystem, TChain, TSystem, TFile
from PSet import process

gSystem.CompileMacro('HTTEvent.cxx')
gSystem.CompileMacro('ScaleFactor.cc')
gSystem.CompileMacro('HTauTauTreeBase.C')
gSystem.CompileMacro('HTauTauTree.C')

from ROOT import HTauTauTree

fileNames = []
for aFile in process.source.fileNames:
    aFile = aFile.replace("Enriched_miniAOD","HTauTauAnalysis")
    aFile = aFile.replace("/store","root://cms-xrd-global.cern.ch///store")
    fileNames.append(aFile)
    print "Adding file: ",aFile
    aROOTFile = TFile.Open(aFile)
    aTree = aROOTFile.Get("HTauTauTree/HTauTauTree")
    print "TTree entries: ",aTree.GetEntries()
    HTauTauTree(aTree).Loop()

#Merge files.
command = "hadd -f WAW_HTauTauAnalysis.root WAW_HTauTauAnalysis_*.root"
#os.system(command)

#Produce framework report required by CRAB
command = "cmsRun -j FrameworkJobReport.xml -p PSet.py"
os.system(command)
