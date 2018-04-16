#-----------------------------------------
#
#Producer controller
#
#-----------------------------------------
import os, re
PyFilePath = os.environ['CMSSW_BASE']+"/src/LLRHiggsTauTau/NtupleProducer/"

#samples list (it could be moved to a cfg file for better reading
#samples = [
#]
#apply corrections?
APPLYMUCORR=False
APPLYELECORR=True
APPLYFSR=False #this is by far the slowest module (not counting SVFit so far)
#Cuts on the Objects (add more cuts with &&)
#MUCUT="(isGlobalMuon || (isTrackerMuon && numberOfMatches>0)) && abs(eta)<2.4 && pt>8"
#ELECUT="abs(eta)<2.5 && gsfTrack.trackerExpectedHitsInner.numberOfHits<=1 && pt>10"
#TAUCUT="pt>15"
#JETCUT="pt>15"

USEMVAMET=False
APPLYMETCORR=False # flag to enable (True) and disable (False) Z-recoil corrections
USE_NOHFMET = False # True to exclude HF and run on silver json

SVFITBYPASS=True # use SVFitBypass module, no SVfit computation, adds dummy userfloats for MET and SVfit mass
BUILDONLYOS=False #If true don't create the collection of SS candidates (and thus don't run SV fit on them)
APPLYTESCORRECTION=False # shift the central value of the tau energy scale before computing up/down variations
COMPUTEUPDOWNSVFIT = True # compute SVfit for up/down TES variation

IsMC=True
Is25ns=True
HLTProcessName='HLT' #Different names possible, check e.g. at https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD.                                                               
if not IsMC:
    HLTProcessName='HLT' #It always 'HLT' for real data                                                                                                                                     
print "HLTProcessName: ",HLTProcessName

#relaxed sets for testing purposes
TAUDISCRIMINATOR="byIsolationMVA3oldDMwoLTraw"
#PVERTEXCUT="!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2" #cut on good primary vertexes
PVERTEXCUT=""#No vertex selection in baseline selection HiggsToTauTauWorking2016#Vertices
MUCUT="isLooseMuon && pt>5"
ELECUT="pt>7"#"gsfTrack.hitPattern().numberOfHits(HitPattern::MISSING_INNER_HITS)<=1 && pt>10"
TAUCUT="(tauID('byCombinedIsolationDeltaBetaCorrRaw3Hits') < 1000.0 || tauID('byIsolationMVArun2v1DBoldDMwLTraw')>-0.999) && pt>18" #miniAOD tau from hpsPFTauProducer have pt>18 and decaymodefinding ID
JETCUT="pt>10"
LLCUT="mass>0"
BCUT="pt>5"

# ------------------------
DO_ENRICHED=False # do True by default, both ntuples and enriched outputs are saved!
STORE_ENRICHEMENT_ONLY=True # When True and DO_ENRICHED=True only collection additional to MiniAOD standard are stored. They can be used to reproduce ntuples when used together with orygnal MiniAOD with two-file-solution    
# ------------------------

is80X = True if 'CMSSW_8' in os.environ['CMSSW_VERSION'] else False# True to run in 80X (2016), False to run in 76X (2015)
print "is80X: " , is80X
is92X = True if 'CMSSW_9' in os.environ['CMSSW_VERSION'] else False# True to run in 9XY (2017), False to run in 76X (2015) or 80X (2016)
print "is92X: " , is92X
##
## Standard sequence
##

if is80X:
    execfile(PyFilePath+"python/HiggsTauTauProducer_80X.py")
elif is92X:
    execfile(PyFilePath+"python/HiggsTauTauProducer_92X.py")
else :
    execfile(PyFilePath+"python/HiggsTauTauProducer.py")
    
### ----------------------------------------------------------------------
### Source, better to use sample to run on batch
### ----------------------------------------------------------------------
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'file:/home/akalinow/scratch/CMS/HiggsCP/Data/VBFHToTauTau_M125_13TeV_powheg_pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM/5260BAEE-5539-E611-BE18-0025905C53F0.root'
        #'file:/home/akalinow/scratch/CMS/HiggsCP/Data/TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14_ext3-v1/MINIAODSIM/0064B539-803A-E611-BDEA-002590D0B060.root'
        #'file:/home/akalinow/scratch/CMS/HiggsCP/Data/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14_ext1-v1/MINIAODSIM/02279B81-6E54-E611-ABD9-A0000420FE80.root'
        'file:/afs/cern.ch/user/t/trudnick/private/CMSSW_9_4_4/src/LLRHiggsTauTau/NtupleProducer/test/Production/002ABFCA-A0B9-E511-B9BA-0CC47A57CD6A.root'
    )
)

#Limited nEv for testing purposes. -1 to run all events
process.maxEvents.input = 100
#process.source.eventsToProcess = cms.untracked.VEventRange('1:743343') #MET test

# JSON mask for data --> defined in the lumiMask file
# from JSON file
#if not IsMC:
#  execfile(PyFilePath+"python/lumiMask.py")
#  process.source.lumisToProcess = LUMIMASK

##
## Output file
##

process.TFileService=cms.Service('TFileService',fileName=cms.string('HTauTauAnalysis.root'))

if DO_ENRICHED:
    process.out = cms.OutputModule("PoolOutputModule",
        fileName = cms.untracked.string('Enriched_miniAOD.root'),
        outputCommands = cms.untracked.vstring('keep *'),
        fastCloning     = cms.untracked.bool(False),
        #Compression settings from MiniAOD allowing to save about 10% of disc space compared to defults ->                                                                                  
        compressionAlgorithm = cms.untracked.string('LZMA'),
        compressionLevel = cms.untracked.int32(4),
        dropMetaData = cms.untracked.string('ALL'),
        eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
        overrideInputFileSplitLevels = cms.untracked.bool(True)
        # <-                                                                                                                                                                                
    )
    if STORE_ENRICHEMENT_ONLY:
        # Store only additional collections compared to MiniAOD necessary to reproduce ntuples (basically MVAMET, lepton pairs with SVFit and corrected jets)                               
        # Size of about 10% of full EnrichedMiniAOD                                                                                                                                         
        process.out.outputCommands.append('drop *')
        process.out.outputCommands.append('keep *_SVllCand_*_*')
        process.out.outputCommands.append('keep *_SVbypass_*_*')
        process.out.outputCommands.append('keep *_barellCand_*_*')
        process.out.outputCommands.append('keep *_corrMVAMET_*_*')
        process.out.outputCommands.append('keep *_MVAMET_*_*')
        process.out.outputCommands.append('keep *_jets_*_*')
        process.out.outputCommands.append('keep *_patJetsReapplyJEC_*_*')
        process.out.outputCommands.append('keep *_softLeptons_*_*')
        process.out.outputCommands.append('keep *_genInfo_*_*')
        #process.out.fileName = 'EnrichementForMiniAOD.root' #FIXME: change name of output file?                                                                                            
    process.end = cms.EndPath(process.out)

#process.options = cms.PSet(skipEvent =  cms.untracked.vstring('ProductNotFound')),
#process.p = cms.EndPath(process.HTauTauTree)
process.p = cms.Path(process.Candidates)

# Silence output
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
#process.MessageLogger.categories.append('onlyError')
#process.MessageLogger.cerr.onlyError=cms.untracked.PSet(threshold  = cms.untracked.string('ERROR'))
#process.MessageLogger.cerr.threshold='ERROR'
#process.MessageLogger = cms.Service("MessageLogger",
#	destinations = cms.untracked.vstring('log.txt')
#)
#process.MessageLogger.threshold = cms.untracked.string('ERROR')

#processDumpFile = open('process.dump' , 'w')
#print >> processDumpFile, process.dumpPython()

if process.source.fileNames[0].find("92X_upgrade2017_realistic_v10")>-1:
    process.GlobalTag.globaltag = '92X_upgrade2017_realistic_v10'
    print "Switching GlobalTag to", process.GlobalTag.globaltag
