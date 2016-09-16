#!/usr/bin/env python

import os, re
import commands
import math
import urllib

from crab3 import *
from mergeROOTFiles import *
#########################################
#########################################
def prepareCrabCfg(dataset,
                   crabCfgName,
                   eventsPerJob,
                   jsonFile,
                   storage_element, 
                   publish_data_suffix):

    workdir = publish_data_suffix
    shortName = dataset.split("/")[1]
    if dataset.split("/")[2].find("Run201")!=-1:
        shortName += "_"+dataset.split("/")[2]

    shortName = shortName.replace("-","_")
    shortName = shortName.split("_")[0]+shortName.split("_")[1]+shortName.split("_")[2]
    shortName+="_"+publish_data_suffix
    if dataset.find("ext")!=-1:
        shortName+= "_"+dataset[dataset.find("ext"):dataset.find("ext")+4]

    if dataset.find("part")!=-1:
        shortName+= "_"+dataset[dataset.find("part"):dataset.find("part")+6]
        
    shortName = shortName.rstrip("-")

    ##Modify CRAB3 configuration
    config.JobType.psetName = 'analyzerMC.py'
    if dataset.split("/")[2].find("JetsToLL")!=-1 or dataset.split("/")[2].find("JetsToLNu")!=-1:
        config.JobType.psetName = 'analyzerMC_METCORR.py'

    config.JobType.disableAutomaticOutputCollection = True
    config.JobType.scriptExe = 'makeAndConvert.py'
    config.JobType.outputFiles = ['WAW_HTauTauAnalysis.root']
    config.JobType.inputFiles = ['HTauTauTree.C', 'HTauTauTree.h', 'HTTEvent.cxx', 'HTTEvent.h', 'PropertyEnum.h', 'TriggerEnum.h', 'SelectionBitsEnum.h', 'ScaleFactor.h','ScaleFactor.cc', 'Muon_IsoMu22_eff.root']
    
    config.Site.storageSite = storage_element
    config.General.requestName = shortName

    config.Data.inputDataset = dataset
    config.Data.outLFNDirBase = '/store/user/apyskir/'+publish_data_suffix+"/"
    config.Data.outputDatasetTag = shortName
    config.Data.inputDBS = 'global'
    config.Data.splitting = 'EventAwareLumiBased'
    config.Data.unitsPerJob = eventsPerJob
    config.Data.totalUnits =  -1
    config.Data.lumiMask=""
    if dataset.split("/")[2].find("Run201")!=-1:
        command = "wget "+jsonFile
        os.system(command)
        config.Data.lumiMask=jsonFile.split("/")[-1]
        config.JobType.psetName = 'analyzerData.py'
    out = open('crabTmp.py','w')
    out.write(config.pythonise_())
    out.close()        
    os.system("crab submit -c crabTmp.py")
    os.system("rm -f "+jsonFile.split("/")[-1])
#########################################
#########################################
eventsPerJob = 200000
eventsPerJob = 100000 #Wjets

datasets = [
    #Data
    "/SingleMuon/Run2016B-PromptReco-v2/MINIAOD",
    "/SingleMuon/Run2016C-PromptReco-v2/MINIAOD",
    "/SingleMuon/Run2016D-PromptReco-v2/MINIAOD",
    "/SingleMuon/Run2016E-PromptReco-v2/MINIAOD",
    #Signal SM
    "/GluGluHToTauTau_M125_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM",
    "/VBFHToTauTau_M125_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM",
    #Signal MSSM
    #"/SUSYGluGluToHToTauTau_M-90_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM",
    #"/SUSYGluGluToHToTauTau_M-120_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM",
    #"/SUSYGluGluToHToTauTau_M-130_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM",
    #"/SUSYGluGluToHToTauTau_M-160_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM",
    #DY
    "/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14_ext1-v1/MINIAODSIM",
    "/DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM",

    "/DY1JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM",
    "/DY2JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM",
    "/DY3JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM",
    "/DY4JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM",
    #Wjets
    "/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14_ext1-v1/MINIAODSIM",
    "/WJetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM",
    "/W1JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM",   
    "/W2JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM",
    "/W3JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM",
    "/W4JetsToLNu_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/MINIAODSIM",
    #TT
    "/TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14_ext3-v1/MINIAODSIM", 
    ##   
]
##TEST
datasets = ["/SUSYGluGluToHToTauTau_M-160_TuneCUETP8M1_13TeV-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM"]

##DATA
#datasets = [ "/SingleMuon/Run2016B-PromptReco-v2/MINIAOD",
#    "/SingleMuon/Run2016C-PromptReco-v2/MINIAOD",
#    "/SingleMuon/Run2016D-PromptReco-v2/MINIAOD",
#    "/SingleMuon/Run2016E-PromptReco-v2/MINIAOD"]
###############

jsonFile2016 = "https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/Cert_271036-277148_13TeV_PromptReco_Collisions16_JSON.txt"
########################################################

for dataset in datasets:
    prepareCrabCfg(crabCfgName="crab3.py",
                   dataset=dataset,
                   eventsPerJob=eventsPerJob,
                   jsonFile=jsonFile2016,
                   storage_element="T2_PL_Swierk",
                   publish_data_suffix = "sync5")

########################################################
########################################################
## Merge output ROOT files.
########################################################
'''
for dataset in datasets:
        mergeDataset(dataset=dataset, publish_data_suffix = "v17",
                                      outputDir="/home/akalinow/scratch/CMS/HiggsCP/Data/NTUPLES_09_09_2016/")
'''

