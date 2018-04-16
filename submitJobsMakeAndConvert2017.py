#!/usr/bin/env python

import os, re
import commands
import math
import urllib

from crab3 import *
from mergeROOTFiles import *
from analyzerMC import *


submitJobs = True
mergeJobs = not submitJobs
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

    if dataset.find("PromptReco-v")!=-1:
        shortName+= "_v"+dataset[dataset.find("PromptReco-v")+12:dataset.find("PromptReco-v")+13]

    if dataset.find("23Sep2016-v")!=-1:
        shortName+= "_v"+dataset[dataset.find("23Sep2016-v")+11:dataset.find("23Sep2016-v")+12]

    if dataset.find("03Feb2017")!=-1:
        patternEnd = dataset.find("/MINIAOD")
        shortName+= dataset[dataset.find("03Feb2017")+9:patternEnd]

    if dataset.find("ext")!=-1:
        shortName+= "_"+dataset[dataset.find("ext"):dataset.find("ext")+4]

    if dataset.find("part")!=-1:
        shortName+= "_"+dataset[dataset.find("part"):dataset.find("part")+6]

    if dataset.find("t-channel")!=-1:
        shortName+= "_"+dataset[dataset.find("channel")+7:dataset.find("channel")+15]

    shortName = shortName.rstrip("-")
    shortName+="_"+publish_data_suffix

    ##Modify CRAB3 configuration
    config.JobType.psetName = 'DUMMY'
    isWZH = False
    if dataset.split("/")[2].find("JetsToLL")!=-1 or dataset.split("/")[2].find("JetsToLNu")!=-1 or dataset.split("/")[2].find("HToTauTau")!=-1:
        isWZH = True
    if isWZH:
        #config.JobType.psetName = 'analyzerMC.py'
        config.JobType.psetName = 'analyzerMC_noMETCorr.py' #do not apply it as not estimated yet
    else:
        config.JobType.psetName = 'analyzerMC_noMETCorr.py'

    config.JobType.disableAutomaticOutputCollection = True
    config.JobType.scriptExe = 'makeAndConvert.py'
    config.JobType.outputFiles = ['WAWMT_HTauTauAnalysis.root', 'WAWTT_HTauTauAnalysis.root', 'WAWMM_HTauTauAnalysis.root']
    #config.JobType.outputFiles = ['WAWMT_HTauTauAnalysis.root']
    config.JobType.inputFiles = ['HTauTauTreeBase.C', 'HTauTauTreeBase.h', 'HTauhTauhTree.C', 'HTauhTauhTree.h','HTauMuTauHTree.C', 'HTauMuTauHTree.h','HMuMuTree.C', 'HMuMuTree.h', 'HTTEvent.cxx', 'HTTEvent.h', 'AnalysisEnums.h', 'PropertyEnum.h', 'TriggerEnum.h', 'SelectionBitsEnum.h', 'zpt_weights_summer2016.root', 'zpt_weights_2016_BtoH.root']

    config.Site.storageSite = storage_element
    config.General.requestName = shortName

    config.Data.inputDataset = dataset
    config.Data.outLFNDirBase = '/store/user/trudnick/WAWNTuple/'+publish_data_suffix+"/"
    config.Data.outputDatasetTag = shortName
    config.Data.inputDBS = 'global'
    config.Data.splitting = 'EventAwareLumiBased'
    config.Data.unitsPerJob = eventsPerJob

    #DYJets
    if dataset.split("/")[2].find("Jets")!=-1:
        eventsPerJob = 40000
    #DY and W 3,4 Jets
    if dataset.split("/")[2].find("3Jets")!=-1 or dataset.split("/")[2].find("4Jets")!=-1:
        eventsPerJob = 1000

    config.Data.totalUnits = -1
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
eventsPerJob = 100000 #Wjets and DYJets hardoced in code above
#eventsPerJob = 200000#4Mu analysis

from datasetsSummer17 import datasets

##TEST
datasets = [
    "/GluGluHToTauTau_M125_13TeV_powheg_pythia8/RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10-v2/MINIAODSIM",    
    "/SUSYGluGluToBBHToTauTau_M-140_TuneCUETP8M1_13TeV-pythia8/RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10-v2/MINIAODSIM",
    "/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10_ext1-v2/MINIAODSIM"
]

###############
jsonFile = "https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON.txt"
########################################################
if submitJobs:
    for dataset in datasets:
        jsonFile2017 = jsonFile

        prepareCrabCfg(crabCfgName="crab3.py",
                       dataset=dataset,
                       eventsPerJob=eventsPerJob,
                       jsonFile=jsonFile2017,
                       storage_element="T2_PL_Swierk",
                       publish_data_suffix = "Summer17_ANN_v1")                  
########################################################
########################################################
## Merge output ROOT files.
########################################################
if mergeJobs:
    for dataset in datasets:
        mergeDataset(dataset=dataset, publish_data_suffix = "Summer17_ANN_v1",
                                      outputDir="/afs/cern.ch/user/t/trudnick/private/CMSSW_9_4_4/src/LLRHiggsTauTau/NtupleProducer/test/Production/TEST_OUT")

#for a in v1/*v7_SM*; do crab resubmit -d $a; done
#for a in v1/*Run2016*v7_SM*; do crab report -d $a; done

#mergeJSON.py crab_SingleMuonRun2016*23*/results/processedLumis.json crab_SingleMuonRun2016H*/results/processedLumis.json > processedLumis_SingleMuon.json
#mergeJSON.py crab_TauRun2016*23*/results/processedLumis.json crab_TauRun2016H*/results/processedLumis.json > processedLumis_Tau.json
#for a in *json; do echo $a >>  lumi.out; ~/.local/bin/brilcalc lumi --normtag /afs/cern.ch/user/l/lumipro/public/normtag_file/normtag_DATACERT.json -i $a; done >>  lumi.out
#grep -A 5 'Summary\|Run2016' lumi.out

'''

'''


