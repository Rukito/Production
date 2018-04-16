from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'DYJetsToLL_M-50'
config.General.workArea = 'v1'
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'PSet.py'
config.JobType.sendExternalFolder = True

config.section_("Data")
config.Data.inputDataset = '/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer17MiniAOD-92X_upgrade2017_realistic_v10_ext1-v2/MINIAODSIM'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 2000 #number of files per jobs
config.Data.totalUnits =  -1 #number of event
config.Data.outLFNDirBase = '/store/user/trudnick/EnrichedMiniAOD/DYJetsToLL_M-50_v1/'
config.Data.publication = False
config.Data.outputDatasetTag = 'DYJetsToLL_M-50_v1'



config.section_("Site")
config.Site.storageSite = 'T2_PL_Swierk'
config.Site.blacklist = ['T2_KR_*','T2_CN_*','T2_BR_*','T2_US_Florida','T2_US_UCSD']
