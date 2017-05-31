#!/usr/bin/env python

import os, re
import commands
import math, time

from analyzerMC import *
########################################################
########################################################
def submitJob(aFile, dataPath, back):
    #Update the CMSSW configuration
    process.source.fileNames =  cms.untracked.vstring()
    process.source.fileNames.append('file:'+ dataPath + aFile)    
    process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(50))
    ##Prepare working directory
    workdir = dataPath.split("Data/")[1] + aFile.split(".root")[0]    
    command = "mkdir -p "+workdir
    
    os.system(command)
    command = "cp job.sh "+workdir
    os.system(command)
    out = open(workdir+'/'+'tmpConfig.py','w')
    out.write(process.dumpPython())
    out.close()
    command = "cd "+workdir+"; ./job.sh >& a.out"
    if back==True:
        command+=" &"
    os.system(command)
########################################################
########################################################
dataPaths = ["/scratch/cms/akalinow/HiggsCP/Data/SUSYGluGluToHToTauTau_M-160_TuneCUETP8M1_13TeV-pythia8/"]

########################################################
for path in dataPaths:
    command = "ls "+path 
    fileList = commands.getoutput(command).split("\n")
    index = 0
    for aFile in fileList:
        command = "ps aux | grep cmsRun"
        while commands.getoutput(command).count("cmsRun tmpConfig.py")>=3:
            time.sleep(15)
        submitJob(aFile,dataPath=path, back=True)
        index+=1
########################################################


