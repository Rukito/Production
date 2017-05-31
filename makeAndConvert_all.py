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
gSystem.CompileMacro('HTauhTauhTree.C')
gSystem.CompileMacro('HTauTauTree.C')
from ROOT import HTauhTauhTree
from ROOT import HTauTauTree

fileNames = [
'/store/user/akalinow/HTauTauAnalysis_TAUCUT_fix.root'
]
#aTree = TChain("HTauTauTree/HTauTauTree")
#for aFile in process.source.fileNames:
for aFile in fileNames:
    aFile = aFile.replace("Enriched_miniAOD","HTauTauAnalysis")
    #aFile = aFile.replace("/store","root://cms-xrd-global.cern.ch///store")
    aFile = aFile.replace("/store","root://se.cis.gov.pl:1094///store")
    print "Adding file: ",aFile
    #fileNames.append(aFile)
    #aTree.Add(aFile)
    aROOTFile = TFile.Open(aFile)
    aTree = aROOTFile.Get("HTauTauTree/HTauTauTree")
    print "TTree entries: ",aTree.GetEntries()
    print "Process TT..."
    HTauhTauhTree(aTree).Loop()
    print "done"
    # file and tree have to be opened again as they are closed by Dtor of analyzer
    bROOTFile = TFile.Open(aFile)
    bTree = bROOTFile.Get("HTauTauTree/HTauTauTree")
    print "TTree entries: ",bTree.GetEntries()
    print "Process MT..."
    HTauTauTree(bTree).Loop()
    print "done"

#print "TTree entries: ",aTree.GetEntries()
#HTauTauTree(aTree).Loop()

#Merge files.
#command = "hadd -f WAW_HTauTauAnalysis.root WAW_HTauTauAnalysis_*.root"
#os.system(command)

#print "Done!", "Processed ",len(process.source.fileNames), "files"
print "Done!", "Processed ",len(fileNames), "files"
